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
// Initialize parameters
// ------------------------------------------------------------------------------------ //
bool le::TextGeneric::Initialize( UInt32_t CountParams, IShaderParameter** ShaderParameters )
{
	std::vector< const char* >			defines;
	UInt32_t							flags = 0;

	if ( !LoadShader( "TextGeneric", "shaders/textgeneric.shader", defines, flags ) )
		return false;

	for ( UInt32_t index = 0; index < CountParams; ++index )
	{
		IShaderParameter*			shaderParameter = ShaderParameters[ index ];
		if ( !shaderParameter->IsDefined() )		continue;

		switch ( shaderParameter->GetType() )
		{
		case SPT_VECTOR_3D:
			if ( strcmp( shaderParameter->GetName(), "color" ) == 0 )
				color = shaderParameter->GetValueVector3D();
			break;

		default:		continue;
		}
	}

	gpuProgram->Bind();
	gpuProgram->SetUniform( "basetexture", 0 );
	gpuProgram->Unbind();

	return true;
}

// ------------------------------------------------------------------------------------ //
// Event: draw text
// ------------------------------------------------------------------------------------ //
void le::TextGeneric::OnDrawText( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Glyph )
{
	if ( !gpuProgram ) return;

	if ( Glyph )			Glyph->Bind( 0 );

	gpuProgram->Bind();
	gpuProgram->SetUniform( "color", color );
	gpuProgram->SetUniform( "matrix_Projection", Camera->GetProjectionMatrix() * Camera->GetViewMatrix() );
	gpuProgram->SetUniform( "matrix_Transformation", Transformation );
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::TextGeneric::GetName() const
{
	return "TextGeneric";
}

// ------------------------------------------------------------------------------------ //
// Get fallback shader
// ------------------------------------------------------------------------------------ //
const char* le::TextGeneric::GetFallbackShader() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::TextGeneric::TextGeneric() :
	color( 1.f, 1.f, 1.f )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::TextGeneric::~TextGeneric()
{
	ClearParameters();
}

// ------------------------------------------------------------------------------------ //
// Clear parameters
// ------------------------------------------------------------------------------------ //
void le::TextGeneric::ClearParameters()
{
	color = Vector3D_t( 1.f, 1.f, 1.f );
}

// ------------------------------------------------------------------------------------ //
// Get descriptor
// ------------------------------------------------------------------------------------ //
le::ShaderDescriptor le::TextGeneric::GetDescriptor()
{
	static std::vector< ShaderParamInfo >			parametersInfo =
	{
		{ "color", SPT_VECTOR_4D }
	};

	ShaderDescriptor			shaderDescriptor;
	shaderDescriptor.name = "TextGeneric";
	shaderDescriptor.CreateShaderFn = []() -> IShader* { return new TextGeneric(); };
	shaderDescriptor.countParameters = parametersInfo.size();
	shaderDescriptor.parametersInfo = parametersInfo.data();
	return shaderDescriptor;
}

// ------------------------------------------------------------------------------------ //
// Is equal
// ------------------------------------------------------------------------------------ //
bool le::TextGeneric::IsEuqal( IShader* Shader ) const
{
	TextGeneric*			shader = ( TextGeneric* ) Shader;
	return strcmp( GetName(), Shader->GetName() ) == 0;
}