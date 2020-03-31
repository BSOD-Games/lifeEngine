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
#include "shaders/spritegeneric_vertex.h"
#include "shaders/spritegeneric_fragment.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать экземпляр шейдера
// ------------------------------------------------------------------------------------ //
bool le::SpriteGeneric::InitInstance( UInt32_t CountParams, IShaderParameter** ShaderParameters )
{
	ShaderDescriptor			shaderDescriptor;
	shaderDescriptor.vertexShaderSource = shader_spritegeneric_vertex;
	shaderDescriptor.fragmentShaderSource = shader_spritegeneric_fragment;

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

    if ( textureRect )
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
