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
#include <fstream>
#include <string>
#include <iostream>

#include "engine/icamera.h"
#include "engine/iengineinternal.h"
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
	std::string					engineDir = static_cast< IEngineInternal* >( g_engine )->GetEngineDirectory();
	std::ifstream				fileVS( engineDir + "/shaders/vs_unlitgeneric.glsl" );
	std::ifstream				filePS( engineDir + "/shaders/ps_unlitgeneric.glsl" );
	std::string					vs, ps;

	std::getline( fileVS, vs, '\0' );
	std::getline( filePS, ps, '\0' );

	shaderDescriptor.vertexShaderSource = vs.c_str();
	shaderDescriptor.fragmentShaderSource = ps.c_str();

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
