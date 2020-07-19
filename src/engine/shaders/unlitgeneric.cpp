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

#include "common/shaderparaminfo.h"
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
	ClearParameters();
	std::vector< const char* >			defines;	

	for ( UInt32_t index = 0; index < CountParams; ++index )
	{
		IShaderParameter*			shaderParameter = ShaderParameters[ index ];
		if ( !shaderParameter->IsDefined() )		continue;

		switch ( shaderParameter->GetType() )
		{
		case SPT_TEXTURE:
			if ( !( flags & SF_BASETEXTURE ) && strcmp( shaderParameter->GetName(), "basetexture" ) == 0 )
			{
				flags |= SF_BASETEXTURE;
				defines.push_back( "BASETEXTURE" );

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
			else if ( !( flags & SF_ALPHA_MAP ) && strcmp( shaderParameter->GetName(), "alphamap" ) == 0 )
			{
				flags |= SF_ALPHA_MAP;
				defines.push_back( "ALPHA_MAP" );

				alphamap = shaderParameter->GetValueTexture();
				alphamap->IncrementReference();
			}
			break;

		case SPT_COLOR:
			if ( strcmp( shaderParameter->GetName(), "color" ) == 0 )
				color = shaderParameter->GetValueColor();
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
	if ( alphamap ) 		gpuProgram->SetUniform( "alphamap", 3 );
	gpuProgram->Unbind();

	return true;
}

// ------------------------------------------------------------------------------------ //
// Event: draw text
// ------------------------------------------------------------------------------------ //
void le::UnlitGeneric::OnDrawText( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Glyph )
{
	OnDrawStaticModel( Transformation, Camera, nullptr );
}

// ------------------------------------------------------------------------------------ //
// Event: draw sprite
// ------------------------------------------------------------------------------------ //
void le::UnlitGeneric::OnDrawSprite( const Matrix4x4_t& Transformation, ICamera* Camera )
{
	OnDrawStaticModel( Transformation, Camera, nullptr );
}

// ------------------------------------------------------------------------------------ //
// Event: draw static model
// ------------------------------------------------------------------------------------ //
void le::UnlitGeneric::OnDrawStaticModel( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	if ( !gpuProgram ) return;

	if ( baseTexture )			baseTexture->Bind( 0 );
	if ( normalMap )			normalMap->Bind( 1 );
	if ( specularMap )			specularMap->Bind( 2 );
	if ( alphamap )				alphamap->Bind( 3 );

	gpuProgram->Bind();
	gpuProgram->SetUniform( "color", color );
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
	specularMap( nullptr ),
	alphamap( nullptr ),
	color( 1.f, 1.f, 1.f )
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

	if ( alphamap )
	{
		if ( alphamap->GetCountReferences() <= 1 )
			alphamap->Release();
		else
			alphamap->DecrementReference();

		alphamap = nullptr;
	}

	flags = SF_NONE;
	color = Vector3D_t( 1.f, 1.f, 1.f );
}

// ------------------------------------------------------------------------------------ //
// Get descriptor
// ------------------------------------------------------------------------------------ //
le::ShaderDescriptor le::UnlitGeneric::GetDescriptor()
{
	static std::vector< ShaderParamInfo >			parametersInfo =
	{
		{ "basetexture", SPT_TEXTURE },
		{ "normalmap", SPT_TEXTURE },
		{ "specularmap", SPT_TEXTURE },
		{ "alphamap", SPT_TEXTURE },
		{ "color", SPT_COLOR }
	};

	ShaderDescriptor			shaderDescriptor;
	shaderDescriptor.name = "UnlitGeneric";
	shaderDescriptor.CreateShaderFn = []() -> IShader* { return new UnlitGeneric(); };
	shaderDescriptor.countParameters = parametersInfo.size();
	shaderDescriptor.parametersInfo = parametersInfo.data();
	return shaderDescriptor;
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
		color == shader->color &&
		baseTexture == shader->baseTexture &&
		normalMap == shader->normalMap &&
		specularMap == shader->specularMap &&
		alphamap == shader->alphamap;
}