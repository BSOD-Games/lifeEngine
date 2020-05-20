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
#include "engine/ishaderparameter.h"
#include "studiorender/itexture.h"
#include "studiorender/igpuprogram.h"

#include "global.h"
#include "textgeneric.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать экземпляр шейдера
// ------------------------------------------------------------------------------------ //
bool le::TextGeneric::InitInstance( UInt32_t CountParams, IShaderParameter** ShaderParameters )
{
	std::vector< const char* >			defines;
	UInt32_t							flags = 0;

	if ( !LoadShader( "TextGeneric", "shaders/textgeneric.shader", defines, flags ) )
		return false;

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
