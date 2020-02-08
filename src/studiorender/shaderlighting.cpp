//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>

#include "common/shaderdescriptor.h"
#include "studiorender/gpuprogram.h"

#include "shaderlighting.h"

// ------------------------------------------------------------------------------------ //
// �����������
// ------------------------------------------------------------------------------------ //
le::ShaderLighting::ShaderLighting() :
	gpuProgram( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// ����������
// ------------------------------------------------------------------------------------ //
le::ShaderLighting::~ShaderLighting()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// ������� ��������� �������
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
			\n \
			float		near;\n \
			float 		far; \n \
		} camera; \n \
		\n \
		uniform struct Light \n \
		{ \n \
			vec3		position; \n \
			vec4		color; \n \
			vec4		specular; \n \
			float		intensivity; \n \
			\n \
			#ifdef POINT_LIGHT \n\
				float 		radius; \n \
			#elif defined( SPOT_LIGHT ) \n\
				float		radius; \n\
				float		height; \n\
			#endif \n\
		} light; \n \
	\n \
	void main() \n \
	{\n \
		#ifdef POINT_LIGHT \n\
			gl_Position = camera.pvMatrix * vec4( vertex_position * light.radius + light.position, 1.f ); \n\
		#elif defined( SPOT_LIGHT ) \n\
			gl_Position = vec4( vertex_position.x * light.radius, vertex_position.y * light.height, vertex_position.z * light.radius, 1.f ); \n \
		#elif \n\
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
			\n \
			float		near;\n \
			float 		far; \n \
		} camera; \n \
	\n \
		uniform struct Light \n \
		{ \n \
			vec3		position; \n \
			vec4		color; \n \
			vec4		specular; \n \
			float		intensivity; \n \
			\n \
			#ifdef POINT_LIGHT \n\
				float 		radius; \n \
			#endif \n\
		} light; \n \
	\n \
	float LinearizeDepth( float depth ) \n\
	{ \n\
  		return ( 2.0f * camera.near ) / ( camera.far + camera.near - depth * ( camera.far - camera.near ) ); \n\
	} \n\
	\n\
	vec3 ReconstructPosition( vec2 FragCoord ) \n\
	{ \n\
		float 		depth = texture( depth, FragCoord ).r;\n\
		vec3 		positionNDC = vec3( FragCoord, depth ) * 2.0 - 1.0;\n\
	\n\
		vec4 		positionCS;\n\
		positionCS.w = ( 2.0f * camera.near * camera.far ) / ( camera.near + camera.far + positionNDC.z * ( camera.near - camera.far ) );\n\
		positionCS.xyz = positionNDC.xyz * positionCS.w;\n\
	\n\
		vec4 		positionVS = camera.invViewMatrix * camera.invProjectionMatrix * positionCS;\n\
		return positionVS.xyz;\n\
	}	\n\
	\n\
	void main()\n\
	{\n\
		vec2	fragCoord = gl_FragCoord.xy / screenSize;\n\
		\n\
		vec4	fragColor = texture( albedoSpecular, fragCoord ); \n\
		vec4	normal = normalize( texture( normalShininess, fragCoord ) ); \n\
		vec3	posFrag = ReconstructPosition( fragCoord ); \n\
		vec3	viewDirection = normalize( camera.position - posFrag ); \n\
		\n\
		vec3	lightDirection = light.position - posFrag; \n\
		float	distance = length( lightDirection ); \n\
		vec3	halfwayDirection = normalize( lightDirection + viewDirection ); \n\
		lightDirection = normalize( lightDirection ); \n\
		\n\
		float 	NdotL = max( dot( lightDirection, normal.xyz ), 0.f ); \n\
		float 	attenuation = pow( clamp( 1.f - pow( distance / light.radius, 4.f ), 0.f, 1.f ), 2.f ) / ( pow( distance, 2.f ) + 1.f );\n\
		float	specularFactor = pow( max( dot( normal.rgb, halfwayDirection ), 0.f ), normal.a ) * fragColor.a; \n\
		\n\
		color = vec4( fragColor.rgb, 1.f ) * texture( emission, fragCoord ); \n\
		color = ( vec4( fragColor.rgb, 1.f ) * light.color * light.intensivity * NdotL + light.specular * specularFactor ) * attenuation; //( light.color * NdotL * light.intensivity + light.specular * specularFactor ) * attenuation * vec4( fragColor.rgb, 1.f ); \n\
	}\n";

	std::vector< const char* >		defines = { "POINT_LIGHT" };

	gpuProgram = new GPUProgram();
	if ( !gpuProgram->Compile( shaderDescriptor, defines.size(), defines.data() ) )
		return false;

	gpuProgram->Bind();
	gpuProgram->SetUniform( "albedoSpecular", 0 );
	gpuProgram->SetUniform( "normalShininess", 1 );
	gpuProgram->SetUniform( "emission", 2 );
	gpuProgram->SetUniform( "depth", 3 );
	gpuProgram->Unbind();

	return true;
}

// ------------------------------------------------------------------------------------ //
// ������� ��������� �������
// ------------------------------------------------------------------------------------ //
void le::ShaderLighting::Delete()
{
	if ( gpuProgram )		delete gpuProgram;
}
