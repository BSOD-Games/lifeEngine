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
#include "engine/ishaderparameter.h"
#include "studiorender/itexture.h"
#include "studiorender/igpuprogram.h"

#include "global.h"
#include "lightmappedgeneric.h"

// ------------------------------------------------------------------------------------ //
// Initialize shader and parameters
// ------------------------------------------------------------------------------------ //
bool le::LightmappedGeneric::Initialize( UInt32_t CountParams, IShaderParameter** ShaderParameters )
{
	ClearParameters();

	std::vector< const char* >			defines;
	if ( !LoadShader( "LightmappedGeneric", "shaders/lightmappedgeneric.shader", defines, 0 ) )
		return false;

	for ( UInt32_t index = 0; index < CountParams; ++index )
	{
		IShaderParameter*			shaderParameter = ShaderParameters[ index ];
		if ( !shaderParameter->IsDefined() ) continue;

		if ( shaderParameter->GetType() == SPT_TEXTURE && strcmp( "basetexture", shaderParameter->GetName() ) == 0 )
		{
			baseTexture = shaderParameter->GetValueTexture();
			baseTexture->IncrementReference();
		}
	}

	gpuProgram->Bind();
	gpuProgram->SetUniform( "basetexture", 0 );
	gpuProgram->SetUniform( "lightmap", 1 );
	gpuProgram->Unbind();

	return true;
}

// ------------------------------------------------------------------------------------ //
// Event: draw mesh
// ------------------------------------------------------------------------------------ //
void le::LightmappedGeneric::OnDrawStaticModel( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	if ( !gpuProgram ) return;

	if ( baseTexture )		baseTexture->Bind( 0 );
	if ( Lightmap )			Lightmap->Bind( 1 );

	gpuProgram->Bind();
	gpuProgram->SetUniform( "matrix_Projection", Camera->GetProjectionMatrix() * Camera->GetViewMatrix() );
	gpuProgram->SetUniform( "matrix_Transformation", Transformation );
}

// ------------------------------------------------------------------------------------ //
// Get shader name
// ------------------------------------------------------------------------------------ //
const char* le::LightmappedGeneric::GetName() const
{
	return "LightmappedGeneric";
}

// ------------------------------------------------------------------------------------ //
// Get fallback shader
// ------------------------------------------------------------------------------------ //
const char* le::LightmappedGeneric::GetFallbackShader() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LightmappedGeneric::LightmappedGeneric() :
	baseTexture( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::LightmappedGeneric::~LightmappedGeneric()
{
	ClearParameters();
}

// ------------------------------------------------------------------------------------ //
// Clear parameters
// ------------------------------------------------------------------------------------ //
void le::LightmappedGeneric::ClearParameters()
{
	if ( baseTexture )
	{
		if ( baseTexture->GetCountReferences() <= 1 )
			baseTexture->Release();
		else
			baseTexture->DecrementReference();

		baseTexture = nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Is equal
// ------------------------------------------------------------------------------------ //
bool le::LightmappedGeneric::IsEuqal( IShader* Shader ) const
{
	LightmappedGeneric*			shader = ( LightmappedGeneric* ) Shader;

	return
		strcmp( GetName(), Shader->GetName() ) == 0 &&
		baseTexture == shader->baseTexture;
}