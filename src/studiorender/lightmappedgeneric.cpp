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

#include "studiorender/itexture.h"

#include "global.h"
#include "lightmappedgeneric.h"
#include "gpuprogram.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать экземпляр шейдера
// ------------------------------------------------------------------------------------ //
bool le::LightmappedGeneric::InitInstance( UInt32_t CountParams, IShaderParameter** ShaderParameters )
{
	std::vector< const char* >			defines;
	UInt32_t							flags = 0;

	if ( !LoadShader( "LightmappedGeneric", "shaders/lightmappedgeneric.shader", defines, flags ) )
		return false;

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
