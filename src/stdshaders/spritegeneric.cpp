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
	\n \
        uniform vec4            textureRect; \n\
		uniform mat4    		matrix_Projection; \n \
		uniform mat4			matrix_Transformation; \n \
	\n \
	void main() \n \
	{\n \
        texCoords = textureRect.xy + ( vertex_texCoords * textureRect.zw ); \n \
        normal = ( matrix_Transformation * vec4( vertex_normal, 0.f ) ).xyz; \n\
		gl_Position = matrix_Projection * matrix_Transformation * vec4( vertex_position, 1.f ); \n \
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
		uniform sampler2D		basetexture;\n\
	\n\
	void main()\n\
	{\n\
		out_albedoSpecular = vec4( texture2D( basetexture, texCoords ).rgb, 0.f ); \n \
		out_normalShininess = vec4( normalize( normal ), 1.f );\n\
		out_emission = vec4( 0.f );\n\
	}\n";

	std::vector< const char* >			defines;
	UInt32_t							flags = 0;

	if ( !LoadShader( shaderDescriptor, defines, flags ) )
		return false;

	IGPUProgram*		gpuProgram = GetGPUProgram( flags );
	if ( !gpuProgram ) return false;

	gpuProgram->Bind();
	gpuProgram->SetUniform( "basetexture", 0 );
	gpuProgram->Unbind();

	return true;
}

// ------------------------------------------------------------------------------------ //
// Подготовка к отрисовке элементов
// ------------------------------------------------------------------------------------ //
void le::SpriteGeneric::OnDrawMesh( UInt32_t CountParams, IShaderParameter** ShaderParameters, const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	IGPUProgram*		gpuProgram = GetGPUProgram( 0 );
	if ( !gpuProgram ) return;
    gpuProgram->Bind();

    for ( UInt32_t index = 0; index < CountParams; ++index )
    {
        IShaderParameter*           shaderParameter = ShaderParameters[ index ];
        if ( !shaderParameter->IsDefined() ) continue;

        if ( strcmp( shaderParameter->GetName(), "basetexture" ) == 0 )           shaderParameter->GetValueTexture()->Bind();
        else if ( strcmp( shaderParameter->GetName(), "textureRect" ) == 0  )     gpuProgram->SetUniform( "textureRect", shaderParameter->GetValueVector4D() );
    }
	
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