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
#include <iostream>
#include <fstream>
#include <string>

#include "engine/icamera.h"
#include "engine/iengineinternal.h"
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
	std::string					engineDir = static_cast< IEngineInternal* >( g_engine )->GetEngineDirectory();
	std::ifstream				fileVS( engineDir + "/shaders/vs_textgeneric.glsl" );
	std::ifstream				filePS( engineDir + "/shaders/ps_textgeneric.glsl" );
	std::string					vs, ps;

	std::getline( fileVS, vs, '\0' );
	std::getline( filePS, ps, '\0' );

	shaderDescriptor.vertexShaderSource = vs.c_str();
	shaderDescriptor.fragmentShaderSource = ps.c_str();

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
