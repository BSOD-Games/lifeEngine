//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <vector>

#include "engine/icamera.h"
#include "studiorender/igpuprogram.h"
#include "studiorender/itexture.h"

#include "global.h"
#include "unlitgeneric.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать экземпляр шейдера
// ------------------------------------------------------------------------------------ //
bool le::UnlitGeneric::InitInstance( UInt32_t CountParams, IShaderParameter** ShaderParameters )
{
	ShaderDescriptor			shaderDescriptor;
	shaderDescriptor.vertexShaderSource = " \
	#version 330 core\n \
	\n \
	layout( location = 0 ) 			in vec3 vertex_position; \n \
	layout( location = 1 ) 			in vec3 vertex_normal; \n \
	layout( location = 2 ) 			in vec2 vertex_texCoords; \n \
	\n\
	#ifdef NORMAL_MAP \n\
		layout( location = 3 ) 			in vec3 vertex_tangent; \n \
		layout( location = 4 ) 			in vec3 vertex_bitangent; \n \
	#endif \n\
	\n \
		out vec2 				texCoords; \n \
	\n\
	#ifdef NORMAL_MAP \n\
		out mat3				tbnMatrix; \n \
	#else \n\
		out vec3				normal; \n\
	#endif \n\
	\n \
		uniform mat4    		matrix_Projection; \n \
		uniform mat4			matrix_Transformation; \n \
	\n \
	void main() \n \
	{\n \
		texCoords = vertex_texCoords; \n \
		\n\
		#ifdef NORMAL_MAP \n\
			vec3 normal = ( matrix_Transformation * vec4( vertex_normal, 0.f ) ).xyz; \n \
			vec3 tangent = ( matrix_Transformation * vec4( vertex_tangent, 0.f ) ).xyz; \n \
			vec3 bitangent = ( matrix_Transformation * vec4( vertex_bitangent, 0.f ) ).xyz; \n \
			tbnMatrix = mat3( tangent, bitangent, normal );\n\
		#else \n\
			normal = ( matrix_Transformation * vec4( vertex_normal, 0.f ) ).xyz; \n \
		#endif \n\
		\n\
		gl_Position = matrix_Projection * matrix_Transformation * vec4( vertex_position, 1.f ); \n \
	}";

	shaderDescriptor.fragmentShaderSource = " \
	#version 330 core\n\
	\n\
		layout ( location = 0 ) out vec4 out_albedoSpecular;\n\
		layout( location = 1 ) out vec4 out_normalShininess;\n\
		layout( location = 2 ) out vec4 out_emission;\n\
		\n\
		in vec2 				texCoords;\n\
		\n\
		#ifdef NORMAL_MAP \n\
			in mat3				tbnMatrix; \n \
		#else \n\
			in vec3				normal; \n\
		#endif \n\
	\n\
		uniform sampler2D		basetexture;\n\
		\n\
		#ifdef NORMAL_MAP \n\
			uniform sampler2D		normalmap;\n\
		#endif \n\
		\n\
		#ifdef SPECULAR_MAP\n\
			uniform sampler2D		specularmap;\n\
		#endif \n\
	\n\
	void main()\n\
	{\n\
		\n\
		#ifdef SPECULAR_MAP\n\
			out_albedoSpecular = vec4( texture2D( basetexture, texCoords ).rgb, texture2D( specularmap, texCoords ).r ); \n \
		#else \n\
			out_albedoSpecular = vec4( texture2D( basetexture, texCoords ).rgb, 0.f ); \n \
		#endif \n\
		\n\
		#ifdef NORMAL_MAP \n\
			vec3 normal = texture2D( normalmap, texCoords ).rgb * 2.0 - 1.0;\n\
			out_normalShininess = vec4( normalize( tbnMatrix * normal ), 32.f );\n\
		#else \n\
			out_normalShininess = vec4( normalize( normal ), 32.f );\n\
		#endif \n\
		\n\
		out_emission = vec4( 0.f );\n\
	}\n";

	std::vector< const char* >			defines;
	UInt32_t							flags = SF_NONE;

	for ( UInt32_t index = 0; index < CountParams; ++index )
	{
		IShaderParameter*			shaderParameter = ShaderParameters[ index ];
		if ( !( flags & SF_NORMAL_MAP ) && strcmp( shaderParameter->GetName(), "normalmap" ) == 0 )
		{
			flags |= SF_NORMAL_MAP;
			defines.push_back( "NORMAL_MAP" );
		}
		else if ( !( flags & SF_SPECULAR_MAP ) && strcmp( shaderParameter->GetName(), "specularmap" ) == 0 )
		{
			flags |= SF_SPECULAR_MAP;
			defines.push_back( "SPECULAR_MAP" );			
		}
	}

	if ( !LoadShader( shaderDescriptor, defines, flags ) )
		return false;

	IGPUProgram*		gpuProgram = GetGPUProgram( flags );
	if ( !gpuProgram ) return false;

	gpuProgram->Bind();
	gpuProgram->SetUniform( "basetexture", 0 );
	if ( flags & SF_NORMAL_MAP ) 		gpuProgram->SetUniform( "normalmap", 1 );
	if ( flags & SF_SPECULAR_MAP ) 		gpuProgram->SetUniform( "specularmap", 2 );
	gpuProgram->Unbind();

	return true;
}

// ------------------------------------------------------------------------------------ //
// Подготовка к отрисовке элементов
// ------------------------------------------------------------------------------------ //
void le::UnlitGeneric::OnDrawMesh( UInt32_t CountParams, IShaderParameter** ShaderParameters, const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	UInt32_t			flags = 0;
	
	for ( UInt32_t index = 0; index < CountParams; ++index )
    {
        IShaderParameter*           shaderParameter = ShaderParameters[ index ];
        if ( !shaderParameter->IsDefined() ) continue;

        if ( strcmp( shaderParameter->GetName(), "basetexture" ) == 0 )  
		{         
			shaderParameter->GetValueTexture()->Bind( 0 );
		}
        else if ( strcmp( shaderParameter->GetName(), "normalmap" ) == 0  )  
		{   
			flags |= SF_NORMAL_MAP;	
			shaderParameter->GetValueTexture()->Bind( 1 );
		}
		else if ( strcmp( shaderParameter->GetName(), "specularmap" ) == 0  )  
		{   
			flags |= SF_SPECULAR_MAP;	
			shaderParameter->GetValueTexture()->Bind( 2 );
		}
	}

	IGPUProgram*		gpuProgram = GetGPUProgram( flags );
	if ( !gpuProgram ) return;

	gpuProgram->Bind();
	gpuProgram->SetUniform( "matrix_Transformation", Transformation );
	gpuProgram->SetUniform( "matrix_Projection", Camera->GetProjectionMatrix() * Camera->GetViewMatrix() );
}

// ------------------------------------------------------------------------------------ //
// Получить название шейдера
// ------------------------------------------------------------------------------------ //
const char* le::UnlitGeneric::GetName() const
{
	return "UnlitGeneric";
}

// ------------------------------------------------------------------------------------ //
// Получить запасной шейдер
// ------------------------------------------------------------------------------------ //
const char* le::UnlitGeneric::GetFallbackShader() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::UnlitGeneric::UnlitGeneric()
{
	shaderParams =
	{
		{ "basetexture",	SPT_TEXTURE		}
	};
}
