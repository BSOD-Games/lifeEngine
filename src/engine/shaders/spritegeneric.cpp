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
#include "spritegeneric.h"

// ------------------------------------------------------------------------------------ //
// Initialize parameters
// ------------------------------------------------------------------------------------ //
bool le::SpriteGeneric::Initialize( UInt32_t CountParams, IShaderParameter** ShaderParameters )
{
	std::vector< const char* >			defines;
	UInt32_t							flags = SF_NONE;

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

		case SPT_VECTOR_4D:
			if ( strcmp( shaderParameter->GetName(), "textureRect" ) == 0 )
				textureRect = shaderParameter->GetValueVector4D();
			break;

		default: continue;
		}
	}

	if ( !LoadShader( "SpriteGeneric", "shaders/spritegeneric.shader", defines, flags ) )
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
void le::SpriteGeneric::OnDrawMesh( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	if ( !gpuProgram ) return;

	if ( baseTexture )			baseTexture->Bind( 0 );
	if ( normalMap )			normalMap->Bind( 1 );
	if ( specularMap )			specularMap->Bind( 2 );

	gpuProgram->Bind();
	gpuProgram->SetUniform( "textureRect", textureRect );
	gpuProgram->SetUniform( "matrix_Projection", Camera->GetProjectionMatrix() * Camera->GetViewMatrix() );
	gpuProgram->SetUniform( "matrix_Transformation", Transformation );
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::SpriteGeneric::GetName() const
{
	return "SpriteGeneric";
}

// ------------------------------------------------------------------------------------ //
// Get fallback shader
// ------------------------------------------------------------------------------------ //
const char* le::SpriteGeneric::GetFallbackShader() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::SpriteGeneric::SpriteGeneric() :
	baseTexture( nullptr ),
	normalMap( nullptr ),
	specularMap( nullptr ),
	textureRect( 0.f, 0.f, 1.f, 1.f )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::SpriteGeneric::~SpriteGeneric()
{
	ClearParameters();
}

// ------------------------------------------------------------------------------------ //
// Clear parameters
// ------------------------------------------------------------------------------------ //
void le::SpriteGeneric::ClearParameters()
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
