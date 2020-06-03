//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-Games/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/ifactory.h"
#include "engine/iengine.h"
#include "engine/iresourcesystem.h"
#include "engine/ishaderparameter.h"
#include "studiorender/itexture.h"

#include "engineapi.h"
#include "errors.h"
#include "shaderparameter.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
ShaderParameter::ShaderParameter() :
	shaderParameter( nullptr ),
	value( nullptr )
{
	shaderParameter = ( le::IShaderParameter* ) EngineAPI::GetInstance()->GetEngine()->GetFactory()->Create( SHADERPARAMETER_INTERFACE_VERSION );
	if ( !shaderParameter )	Error_Critical( "Interface le::IShaderParameter version[" SHADERPARAMETER_INTERFACE_VERSION "] not found in core" );
	shaderParameter->IncrementReference();
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
ShaderParameter::~ShaderParameter()
{	
	ResetValue();

	if ( shaderParameter )
	{
		if ( shaderParameter->GetCountReferences() <= 1 )		shaderParameter->Release();
		else													shaderParameter->DecrementReference();
	}
}

// ------------------------------------------------------------------------------------ //
// Constructor Texture
// ------------------------------------------------------------------------------------ //
ShaderParameter::Texture::Texture( le::ITexture* Texture, QString Path ) :
	texture( Texture ),
	path( Path )
{
	if ( texture ) texture->IncrementReference();
}

// ------------------------------------------------------------------------------------ //
// Destructor Texture
// ------------------------------------------------------------------------------------ //
ShaderParameter::Texture::~Texture()
{
	if ( texture )
	{
		if ( texture->GetCountReferences() <= 1 )		texture->Release();
		else											texture->DecrementReference();
	}
}

// ------------------------------------------------------------------------------------ //
// Reset value
// ------------------------------------------------------------------------------------ //
void ShaderParameter::ResetValue()
{
	if ( !shaderParameter ) return;
	shaderParameter->Clear();

	switch ( type )
	{
	case le::SPT_TEXTURE:
		delete static_cast< Texture* >( value );
		EngineAPI::GetInstance()->GetResourceSystem()->UnloadTexture( shaderParameter->GetName() );
		break;
	}

	value = nullptr;	
}

// ------------------------------------------------------------------------------------ //
// Set name
// ------------------------------------------------------------------------------------ //
void ShaderParameter::SetName( const QString& Name, le::SHADER_PARAMETER_TYPE Type )
{
	if ( !shaderParameter ) return;
	shaderParameter->SetName( Name.toLocal8Bit().data() );
	type = Type;
}

// ------------------------------------------------------------------------------------ //
// Set value texture
// ------------------------------------------------------------------------------------ //
void ShaderParameter::SetValueTexture( const Texture& Texture )
{
	if ( !shaderParameter || type != le::SPT_TEXTURE ) return;
	
	shaderParameter->SetValueTexture( Texture.texture );
	value = new ShaderParameter::Texture( Texture.texture, Texture.path );
}

// ------------------------------------------------------------------------------------ //
// Get value texture
// ------------------------------------------------------------------------------------ //
ShaderParameter::Texture ShaderParameter::GetValueTexture() const
{
	if ( !value || type != le::SPT_TEXTURE ) 
		return Texture();

	Texture*			texture = static_cast< Texture* >( value );
	return Texture( texture->texture, texture->path );
}