//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>

#include "common/shaderdescriptor.h"
#include "studiorender/gpuprogram.h"

#include "shader_lighting.h"

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ShaderLighting::ShaderLighting() :
	gpuProgram( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ShaderLighting::~ShaderLighting()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Создать шейдер
// ------------------------------------------------------------------------------------ //
bool le::ShaderLighting::Create()
{
	ShaderDescriptor		shaderDescriptor = {};
	shaderDescriptor.vertexShaderSource = " \
	#version 330 core\n \
	\n \
	layout( location = 0 ) 			in vec3 vertex_position; \n \
	\n \
		uniform struct Camera \n \
		{ 	\n \
			mat4		pvMatrix; \n \
			mat4		invProjectionMatrix;\n \
			mat4		invViewMatrix;\n \
			vec3		position; \n \
		} camera; \n \
		\n \
		uniform struct Light \n \
		{ \n \
			vec4		color; \n \
			vec4		specular; \n \
			float		intensivity; \n \
			\n \
			#ifdef POINT_LIGHT \n\
				vec3		position; \n \
				float 		radius; \n \
			#elif defined( SPOT_LIGHT ) \n\
				vec3		position; \n \
				float		radius; \n\
				float		height; \n\
				float		cutoff; \n\
				vec3		direction; \n\
			#elif defined( DIRECTIONAL_LIGHT ) \n\
				vec3		direction; \n\
			#endif \n\
		} light; \n \
	\n \
	uniform mat4 		pvtMatrix; \n \
	\n \
	void main() \n \
	{\n \
		#ifdef POINT_LIGHT \n\
			gl_Position = pvtMatrix * vec4( vertex_position * light.radius, 1.f ); \n\
		#elif defined( SPOT_LIGHT ) \n\
			gl_Position = pvtMatrix * vec4( vertex_position.x * light.radius, vertex_position.y * light.height, vertex_position.z * light.radius, 1.f ); \n \
		#elif defined( DIRECTIONAL_LIGHT ) \n\
			gl_Position = vec4( vertex_position, 1.f ); \n \
		 #endif \n\
	}";

	shaderDescriptor.fragmentShaderSource = "\
	#version 330 core\n\
	\n\
		out vec4				color;\n\
	\n\
		uniform vec2			screenSize;\n\
		uniform sampler2D		albedoSpecular;\n\
		uniform sampler2D		normalShininess;\n\
		uniform sampler2D		emission;\n\
		uniform sampler2D		depth;\n\
	\n \
		uniform struct Camera \n \
		{ 	\n \
			mat4		pvMatrix; \n \
			mat4		invProjectionMatrix;\n \
			mat4		invViewMatrix;\n \
			vec3		position; \n \
		} camera; \n \
	\n \
		uniform struct Light \n \
		{ \n \
			vec4		color; \n \
			vec4		specular; \n \
			float		intensivity; \n \
			\n \
			#ifdef POINT_LIGHT \n\
				vec3		position; \n \
				float 		radius; \n \
			#elif defined( SPOT_LIGHT ) \n\
				vec3		position; \n \
				float		radius; \n\
				float		height; \n\
				float		cutoff; \n\
				vec3		direction; \n\
			#elif defined( DIRECTIONAL_LIGHT ) \n\
				vec3		direction; \n\
			#endif \n\
		} light; \n \
	\n \
	vec3 ReconstructPosition( vec2 FragCoord ) \n\
	{ \n\
		float 		depth = texture( depth, FragCoord ).r;\n\
		\
		vec4 		position = camera.invViewMatrix * camera.invProjectionMatrix * vec4( FragCoord * 2.f - 1.f, depth * 2.f - 1.f, 1.f );\n\
		position /= position.w;\n\
		return position.xyz;\n\
		\
	}	\n\
	\n\
	void main()\n\
	{\n\
		vec2	fragCoord = gl_FragCoord.xy / screenSize;\n\
		\n\
		vec4	fragColor = texture( albedoSpecular, fragCoord ); \n\
		vec4	normal = texture( normalShininess, fragCoord ); \n\
		normal.xyz = normalize( normal.xyz ); \n\
		vec3	posFrag = ReconstructPosition( fragCoord ); \n\
		vec3	viewDirection = normalize( camera.position - posFrag ); \n\
		\n\
		#if defined( POINT_LIGHT ) || defined( SPOT_LIGHT )\n\
			vec3	lightDirection = light.position - posFrag; \n\
			float	distance = length( lightDirection ); \n\
			lightDirection = normalize( lightDirection ); \n\
		#elif defined( DIRECTIONAL_LIGHT ) \n\
			vec3	lightDirection = normalize( light.direction );\n\
		#endif \n\
		vec3	halfwayDirection = normalize( lightDirection + viewDirection ); \n\
		\n\
		float 	NdotL = max( dot( normal.xyz, lightDirection ), 0.f ); \n\
		\n\
		#ifdef POINT_LIGHT \n\
			float 	attenuation = pow( clamp( 1.f - pow( distance / light.radius, 4.f ), 0.f, 1.f ), 2.f ) / ( pow( distance, 2.f ) + 1.f );\n\
		#elif defined( SPOT_LIGHT ) \n\
			float 	attenuation = pow( clamp( 1.f - pow( distance / light.height, 4.f ), 0.f, 1.f ), 2.f ) / ( pow( distance, 2.f ) + 1.f );\n\
		#endif \n\
		float	specularFactor = pow( max( dot( normal.xyz, halfwayDirection ), 0.f ), normal.a ) * fragColor.a; \n\
		\n\
		#ifdef SPOT_LIGHT \n\
			float 		spotFactor = dot( -lightDirection, normalize( light.direction ) );\n\
			spotFactor = clamp( ( spotFactor - light.cutoff ) / ( 1.0f - light.cutoff ), 0.0f, 1.0f ); \n\
		#endif \n\
		\n\
		#ifdef POINT_LIGHT \n\
			color = ( vec4( fragColor.rgb, 1.f ) * light.color * light.intensivity * NdotL + light.specular * specularFactor ) * attenuation; \n\
		#elif defined( SPOT_LIGHT ) \n\
			color = ( vec4( fragColor.rgb, 1.f ) * light.color * light.intensivity * NdotL + light.specular * specularFactor ) * attenuation * spotFactor; \n\
		#elif defined( DIRECTIONAL_LIGHT ) \n\
			color = vec4( fragColor.rgb, 1.f ) * light.color * light.intensivity * NdotL + light.specular * specularFactor; \n\
		#endif \n\
	}\n";

	// Компилируем шейдер для точечного освещения

	std::vector< const char* >		defines = { "POINT_LIGHT" };
	GPUProgram* 		gpuProgram_pointLight = new GPUProgram();
	if ( !gpuProgram_pointLight->Compile( shaderDescriptor, defines.size(), defines.data() ) )
		return false;

	gpuProgram_pointLight->Bind();
	gpuProgram_pointLight->SetUniform( "albedoSpecular", 0 );
	gpuProgram_pointLight->SetUniform( "normalShininess", 1 );
	gpuProgram_pointLight->SetUniform( "emission", 2 );
	gpuProgram_pointLight->SetUniform( "depth", 3 );
	gpuProgram_pointLight->Unbind();

	// Компилируем шейдер для прожекторного освещения

	defines = { "SPOT_LIGHT" };
	GPUProgram*			gpuProgram_spotLight = new GPUProgram();
	if ( !gpuProgram_spotLight->Compile( shaderDescriptor, defines.size(), defines.data() ) )
		return false;

	gpuProgram_spotLight->Bind();
	gpuProgram_spotLight->SetUniform( "albedoSpecular", 0 );
	gpuProgram_spotLight->SetUniform( "normalShininess", 1 );
	gpuProgram_spotLight->SetUniform( "emission", 2 );
	gpuProgram_spotLight->SetUniform( "depth", 3 );
	gpuProgram_spotLight->Unbind();

	// Компилируем шейдер для направленого освещения

	defines = { "DIRECTIONAL_LIGHT" };
	GPUProgram*			gpuProgram_directionalLight = new GPUProgram();
	if ( !gpuProgram_directionalLight->Compile( shaderDescriptor, defines.size(), defines.data() ) )
		return false;

	gpuProgram_directionalLight->Bind();
	gpuProgram_directionalLight->SetUniform( "albedoSpecular", 0 );
	gpuProgram_directionalLight->SetUniform( "normalShininess", 1 );
	gpuProgram_directionalLight->SetUniform( "emission", 2 );
	gpuProgram_directionalLight->SetUniform( "depth", 3 );
	gpuProgram_directionalLight->Unbind();

	gpuProgram = gpuProgram_pointLight;
	gpuPrograms[ LT_POINT ] = gpuProgram_pointLight;
	gpuPrograms[ LT_SPOT ] = gpuProgram_spotLight;
	gpuPrograms[ LT_DIRECTIONAL ] = gpuProgram_directionalLight;
	
	return true;
}

// ------------------------------------------------------------------------------------ //
// Удалить шейдер
// ------------------------------------------------------------------------------------ //
void le::ShaderLighting::Delete()
{
    Unbind();

    for ( auto it = gpuPrograms.begin(), itEnd = gpuPrograms.end(); it != itEnd; ++it )
        delete it->second;

    gpuProgram = nullptr;
    gpuPrograms.clear();
}
