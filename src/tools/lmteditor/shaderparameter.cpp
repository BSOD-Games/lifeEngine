//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <qcolor.h>

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

	case le::SPT_COLOR:
		delete static_cast< QColor* >( value );
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
// Set value color
// ------------------------------------------------------------------------------------ //
void ShaderParameter::SetValueColor( const QColor& Value )
{
	if ( !shaderParameter || type != le::SPT_COLOR ) return;

	QColor*			color = new QColor( Value );
	value = color;

	qreal		color_r, color_g, color_b, color_a;
	color->getRgbF( &color_r, &color_g, &color_b, &color_a );
	shaderParameter->SetValueColor( le::Color_t( color_r, color_g, color_b, color_a ) );
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

// ------------------------------------------------------------------------------------ //
// Get value color
// ------------------------------------------------------------------------------------ //
QColor ShaderParameter::GetValueColor() const
{
	if ( !value || type != le::SPT_COLOR )
		return QColor();

	return *static_cast< QColor* >( value );
}