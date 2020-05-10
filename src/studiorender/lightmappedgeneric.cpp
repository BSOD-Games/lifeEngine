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
#include <stdexcept>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>

#include "engine/iengineinternal.h"
#include "engine/icamera.h"
#include "studiorender/igpuprogram.h"
#include "studiorender/itexture.h"
#include "engine\iconsolesystem.h"
#include "global.h"
#include "lightmappedgeneric.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать экземпляр шейдера
// ------------------------------------------------------------------------------------ //
bool le::LightmappedGeneric::InitInstance( UInt32_t CountParams, IShaderParameter** ShaderParameters )
{
	ShaderDescriptor			shaderDescriptor;
	std::string					engineDir = static_cast< IEngineInternal* >( g_engine )->GetEngineDirectory();
	std::ifstream				fileVS( engineDir + "/shaders/vs_lightmappedgeneric.glsl" );
	std::ifstream				filePS( engineDir + "/shaders/ps_lightmappedgeneric.glsl" );
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
	gpuProgram->SetUniform( "lightmap", 1 );
	gpuProgram->Unbind();

	return true;
}

// ------------------------------------------------------------------------------------ //
// Подготовка к отрисовке элементов
// ------------------------------------------------------------------------------------ //
void le::LightmappedGeneric::OnDrawMesh( UInt32_t CountParams, IShaderParameter** ShaderParameters, const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	IGPUProgram*		gpuProgram = GetGPUProgram( 0 );
	if ( !gpuProgram ) return;

	if ( ShaderParameters[ 0 ]->IsDefined() )		ShaderParameters[ 0 ]->GetValueTexture()->Bind( 0 );
	if ( Lightmap )			Lightmap->Bind( 1 );

	gpuProgram->Bind();
	gpuProgram->SetUniform( "matrix_Transformation", Transformation );
	gpuProgram->SetUniform( "matrix_Projection", Camera->GetProjectionMatrix() * Camera->GetViewMatrix() );
}

// ------------------------------------------------------------------------------------ //
// Получить название шейдера
// ------------------------------------------------------------------------------------ //
const char* le::LightmappedGeneric::GetName() const
{
	return "LightmappedGeneric";
}

// ------------------------------------------------------------------------------------ //
// Получить запасной шейдер
// ------------------------------------------------------------------------------------ //
const char* le::LightmappedGeneric::GetFallbackShader() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::LightmappedGeneric::LightmappedGeneric()
{
	shaderParams =
	{
		{ "basetexture",	SPT_TEXTURE		}
	};
}
