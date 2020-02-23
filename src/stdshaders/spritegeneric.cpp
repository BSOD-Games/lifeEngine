//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "engine/icamera.h"
#include "engine/iconsolesystem.h"
#include "studiorender/igpuprogram.h"
#include "studiorender/itexture.h"

#include "global.h"
#include "spritegeneric.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать экземпляр шейдера
// ------------------------------------------------------------------------------------ //
bool le::SpriteGeneric::InitInstance( UInt32_t CountParams, IShaderParameter** ShaderParameters )
{
	ShaderDescriptor			shaderDescriptor;
	shaderDescriptor.vertexShaderSource = " \
	#version 330 core\n \
	\n \
	layout( location = 0 ) 			in vec3 vertex_position; \n \
    layout( location = 1 ) 			in vec3 vertex_normal; \n \
	layout( location = 2 ) 			in vec2 vertex_texCoords; \n \
	\n \
		out vec2 				texCoords; \n \
        out vec3 				normal; \n \
		\n\
		#ifdef NORMAL_MAP \n\
			out vec3				fragPos; \n\
		#endif \n\
	\n \
        uniform vec4            textureRect; \n\
		uniform mat4    		matrix_Projection; \n \
		uniform mat4			matrix_Transformation; \n \
	\n \
	void main() \n \
	{\n \
        texCoords = textureRect.xy + ( vertex_texCoords * textureRect.zw ); \n \
        normal = ( matrix_Transformation * vec4( vertex_normal, 0.f ) ).xyz; \n\
		\n\
		#ifdef NORMAL_MAP \n\
			fragPos = ( matrix_Transformation * vec4( vertex_position, 1.f ) ).xyz; \n\
			gl_Position = matrix_Projection * vec4( fragPos, 1.f ); \n \
		#else \n\
			gl_Position = matrix_Projection * matrix_Transformation * vec4( vertex_position, 1.f ); \n \
		#endif \n\
	}";

	shaderDescriptor.fragmentShaderSource = " \
	#version 330 core\n\
	\n\
		layout ( location = 0 )     out vec4 out_albedoSpecular;\n\
		layout( location = 1 )      out vec4 out_normalShininess;\n\
		layout( location = 2 )      out vec4 out_emission;\n\
		\n\
		in vec2 				texCoords;\n\
        in vec3 				normal;\n\
		\n\
		#ifdef NORMAL_MAP \n\
			in vec3					fragPos; \n\
		#endif \n\
	\n\
		uniform sampler2D		basetexture;\n\
		\n\
		#ifdef NORMAL_MAP \n\
			uniform sampler2D		normalmap;\n\
		#endif \n\
		\n\
		#ifdef SPECULAR_MAP \n\
			uniform sampler2D		specularmap;\n\
		#endif \n\
	\n\
	#ifdef NORMAL_MAP \n\
		vec3 perturbNormal()\n\
		{\n\
			vec3 tangentNormal = texture( normalmap, texCoords ).xyz * 2.0 - 1.0;\n\
	\n\
			vec3 q1 = dFdx( fragPos );\n\
			vec3 q2 = dFdy( fragPos );\n\
			vec2 st1 = dFdx( texCoords );\n\
			vec2 st2 = dFdy( texCoords );\n\
	\n\
			vec3 N = normalize( normal );\n\
			vec3 T = normalize( q1 * st2.t - q2 * st1.t );\n\
			vec3 B = -normalize( cross( N, T ) );\n\
			mat3 TBN = mat3( T, B, N );\n\
	\n\
			return normalize( TBN * tangentNormal );\n\
		}\n\
	#endif \n\
	\n\
	void main()\n\
	{\n\
		#ifdef SPECULAR_MAP\n\
			out_albedoSpecular = vec4( texture2D( basetexture, texCoords ).rgb, texture2D( specularmap, texCoords ).r ); \n \
		#else\n\
			out_albedoSpecular = vec4( texture2D( basetexture, texCoords ).rgb, 0.f ); \n \
		#endif \n\
		\n\
		#ifdef NORMAL_MAP \n\
			out_normalShininess = vec4( perturbNormal(), 32.f );\n\
		#else\n\
			out_normalShininess = vec4( normal, 32.f );\n\
		#endif\n\
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
void le::SpriteGeneric::OnDrawMesh( UInt32_t CountParams, IShaderParameter** ShaderParameters, const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	UInt32_t			flags = 0;
	IShaderParameter*   textureRect = nullptr;

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
		else if ( strcmp( shaderParameter->GetName(), "textureRect" ) == 0  )  
			textureRect = shaderParameter;   
	}

	IGPUProgram*		gpuProgram = GetGPUProgram( flags );
	if ( !gpuProgram ) return;

	gpuProgram->Bind();

	if ( !textureRect )
		gpuProgram->SetUniform( "textureRect", textureRect->GetValueVector4D() );
	else
		gpuProgram->SetUniform( "textureRect", Vector4D_t( 0.f, 0.f, 1.f, 1.f ) );
	

	gpuProgram->SetUniform( "matrix_Transformation", Transformation );
	gpuProgram->SetUniform( "matrix_Projection", Camera->GetProjectionMatrix() * Camera->GetViewMatrix() );
}

// ------------------------------------------------------------------------------------ //
// Получить название шейдера
// ------------------------------------------------------------------------------------ //
const char* le::SpriteGeneric::GetName() const
{
	return "SpriteGeneric";
}

// ------------------------------------------------------------------------------------ //
// Получить запасной шейдер
// ------------------------------------------------------------------------------------ //
const char* le::SpriteGeneric::GetFallbackShader() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::SpriteGeneric::SpriteGeneric()
{
	shaderParams =
	{
		{ "basetexture",	SPT_TEXTURE		}
	};
}