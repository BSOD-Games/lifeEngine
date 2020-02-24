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
#include "textgeneric.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать экземпляр шейдера
// ------------------------------------------------------------------------------------ //
bool le::TextGeneric::InitInstance( UInt32_t CountParams, IShaderParameter** ShaderParameters )
{
	ShaderDescriptor			shaderDescriptor;
	shaderDescriptor.vertexShaderSource = " \
	#version 330 core\n \
	\n \
	layout( location = 0 ) 			in vec3 vertex_position; \n \
	layout( location = 1 ) 			in vec2 vertex_texCoords; \n \
	\n \
		out vec2 				texCoords; \n \
\n\
        uniform vec3            color; \n\
		uniform mat4    		matrix_Projection; \n \
		uniform mat4			matrix_Transformation; \n \
	\n \
	void main() \n \
	{\n \
        texCoords = vertex_texCoords; \n \
		gl_Position = matrix_Projection * matrix_Transformation * vec4( vertex_position, 1.f ); \n \
	}";

	shaderDescriptor.fragmentShaderSource = " \
	#version 330 core\n\
	\n\
		layout ( location = 0 )     out vec4 out_albedoSpecular;\n\
		layout( location = 2 )      out vec4 out_emission;\n\
		\n\
		in vec2 				texCoords;\n\
		\n\
		uniform vec3            color; \n\
		uniform sampler2D		basetexture;\n\
	\n\
	void main()\n\
	{\n\
		float			alphaGlyph = texture2D( basetexture, texCoords ).r;\n\
		\n\
		if ( alphaGlyph < 0.001f )\n\
			discard;\n\
		\n\
		out_albedoSpecular = vec4( alphaGlyph * color, 0.f ); \n \
		out_emission = vec4( 1.f );\n\
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
void le::TextGeneric::OnDrawMesh( UInt32_t CountParams, IShaderParameter** ShaderParameters, const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	UInt32_t			flags = 0;
	IShaderParameter*   textColor = nullptr;

	for ( UInt32_t index = 0; index < CountParams; ++index )
    {
        IShaderParameter*           shaderParameter = ShaderParameters[ index ];
        if ( !shaderParameter->IsDefined() ) continue;

        if ( strcmp( shaderParameter->GetName(), "basetexture" ) == 0 )  
		{         
			shaderParameter->GetValueTexture()->Bind( 0 );
		}
		else if ( strcmp( shaderParameter->GetName(), "color" ) == 0  )  
			textColor = shaderParameter;   
	}

	IGPUProgram*		gpuProgram = GetGPUProgram( flags );
	if ( !gpuProgram ) return;

	gpuProgram->Bind();

	if ( !textColor )
		gpuProgram->SetUniform( "color", textColor->GetValueVector3D() );
	else
		gpuProgram->SetUniform( "color", Vector3D_t( 1.f ) );
	

	gpuProgram->SetUniform( "matrix_Transformation", Transformation );
	gpuProgram->SetUniform( "matrix_Projection", Camera->GetProjectionMatrix() * Camera->GetViewMatrix() );
}

// ------------------------------------------------------------------------------------ //
// Получить название шейдера
// ------------------------------------------------------------------------------------ //
const char* le::TextGeneric::GetName() const
{
	return "TextGeneric";
}

// ------------------------------------------------------------------------------------ //
// Получить запасной шейдер
// ------------------------------------------------------------------------------------ //
const char* le::TextGeneric::GetFallbackShader() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::TextGeneric::TextGeneric()
{
	shaderParams =
	{
		{ "basetexture",	SPT_TEXTURE		}
	};
}