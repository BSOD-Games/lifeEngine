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
#include "engine/ishaderparameter.h"
#include "studiorender/itexture.h"
#include "studiorender/igpuprogram.h"

#include "global.h"
#include "unlitgeneric.h"

// ------------------------------------------------------------------------------------ //
// Initialize parameters
// ------------------------------------------------------------------------------------ //
bool le::UnlitGeneric::Initialize( UInt32_t CountParams, IShaderParameter** ShaderParameters )
{
	std::vector< const char* >			defines;
	flags = SF_NONE;

	for ( UInt32_t index = 0; index < CountParams; ++index )
	{
		IShaderParameter*			shaderParameter = ShaderParameters[ index ];
		if ( !shaderParameter->IsDefined() )		continue;

		switch ( shaderParameter->GetType() )
		{
		case SPT_TEXTURE:
			if ( strcmp( shaderParameter->GetName(), "basetexture" ) == 0 )
			{
				baseTexture = shaderParameter->GetValueTexture();
				baseTexture->IncrementReference();
			}
			else if ( !( flags & SF_NORMAL_MAP ) && strcmp( shaderParameter->GetName(), "normalmap" ) == 0 )
			{
				flags |= SF_NORMAL_MAP;
				defines.push_back( "NORMAL_MAP" );

				normalMap = shaderParameter->GetValueTexture();
				normalMap->IncrementReference();
			}
			else if ( !( flags & SF_SPECULAR_MAP ) && strcmp( shaderParameter->GetName(), "specularmap" ) == 0 )
			{
				flags |= SF_SPECULAR_MAP;
				defines.push_back( "SPECULAR_MAP" );

				specularMap = shaderParameter->GetValueTexture();
				specularMap->IncrementReference();
			}
			break;

		default: continue;
		}
	}

	if ( !LoadShader( "UnlitGeneric", "shaders/unlitgeneric.shader", defines, flags ) )
	{
		ClearParameters();
		return false;
	}

	gpuProgram->Bind();
	gpuProgram->SetUniform( "basetexture", 0 );
	if ( normalMap ) 		gpuProgram->SetUniform( "normalmap", 1 );
	if ( specularMap ) 		gpuProgram->SetUniform( "specularmap", 2 );
	gpuProgram->Unbind();

	return true;
}

// ------------------------------------------------------------------------------------ //
// Event: draw mesh
// ------------------------------------------------------------------------------------ //
void le::UnlitGeneric::OnDrawMesh( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	if ( baseTexture )			baseTexture->Bind( 0 );
	if ( normalMap )			normalMap->Bind( 1 );
	if ( specularMap )			specularMap->Bind( 2 );

	gpuProgram->Bind();
	gpuProgram->SetUniform( "matrix_Projection", Camera->GetProjectionMatrix() * Camera->GetViewMatrix() );
	gpuProgram->SetUniform( "matrix_Transformation", Transformation );
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::UnlitGeneric::GetName() const
{
	return "UnlitGeneric";
}

// ------------------------------------------------------------------------------------ //
// Get fallback shader
// ------------------------------------------------------------------------------------ //
const char* le::UnlitGeneric::GetFallbackShader() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::UnlitGeneric::UnlitGeneric() :
	flags( SF_NONE ),
	baseTexture( nullptr ),
	normalMap( nullptr ),
	specularMap( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::UnlitGeneric::~UnlitGeneric()
{
	ClearParameters();
}

// ------------------------------------------------------------------------------------ //
// Clear parameters
// ------------------------------------------------------------------------------------ //
void le::UnlitGeneric::ClearParameters()
{
	if ( baseTexture )
	{
		if ( baseTexture->GetCountReferences() <= 1 )
			baseTexture->Release();
		else
			baseTexture->DecrementReference();

		baseTexture = nullptr;
	}

	if ( normalMap )
	{
		if ( normalMap->GetCountReferences() <= 1 )
			normalMap->Release();
		else
			normalMap->DecrementReference();

		normalMap = nullptr;
	}

	if ( specularMap )
	{
		if ( specularMap->GetCountReferences() <= 1 )
			specularMap->Release();
		else
			specularMap->DecrementReference();

		specularMap = nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Is equal
// ------------------------------------------------------------------------------------ //
bool le::UnlitGeneric::IsEuqal( IShader* Shader ) const
{
	UnlitGeneric*			shader = ( UnlitGeneric* ) Shader;

	return
		strcmp( GetName(), Shader->GetName() ) == 0 &&
		flags == shader->flags &&
		baseTexture == shader->baseTexture &&
		normalMap == shader->normalMap &&
		specularMap == shader->specularMap;
}