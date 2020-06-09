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

	case le::SPT_INT:			delete static_cast< int* >( value );				break;
	case le::SPT_FLOAT:			delete static_cast< float* >( value );				break;
	case le::SPT_SHADER_FLAG:	delete static_cast< bool* >( value );				break;
	case le::SPT_COLOR:			delete static_cast< QColor* >( value );				break;
	case le::SPT_VECTOR_4D:		delete static_cast< le::Vector4D_t* >( value );		break;
	case le::SPT_VECTOR_3D:		delete static_cast< le::Vector3D_t* >( value );		break;
	case le::SPT_VECTOR_2D:		delete static_cast< le::Vector2D_t* >( value );		break;
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

// ------------------------------------------------------------------------------------ //
// Set value Vector4D
// ------------------------------------------------------------------------------------ //
void ShaderParameter::SetValueVector4D( const le::Vector4D_t& Value )
{
	if ( !shaderParameter || type != le::SPT_VECTOR_4D ) return;

	le::Vector4D_t*			vector = new le::Vector4D_t( Value );
	value = vector;

	shaderParameter->SetValueVector4D( *vector );
}

// ------------------------------------------------------------------------------------ //
// Get value Vector4D
// ------------------------------------------------------------------------------------ //
le::Vector4D_t ShaderParameter::GetValueVector4D() const
{
	if ( !value || type != le::SPT_VECTOR_4D )
		return le::Vector4D_t();

	return *static_cast< le::Vector4D_t* >( value );
}

// ------------------------------------------------------------------------------------ //
// Set value Vector3D
// ------------------------------------------------------------------------------------ //
void ShaderParameter::SetValueVector3D( const le::Vector3D_t& Value )
{
	if ( !shaderParameter || type != le::SPT_VECTOR_3D ) return;

	le::Vector3D_t* vector = new le::Vector3D_t( Value );
	value = vector;

	shaderParameter->SetValueVector3D( *vector );
}

// ------------------------------------------------------------------------------------ //
// Get value Vector3D
// ------------------------------------------------------------------------------------ //
le::Vector3D_t ShaderParameter::GetValueVector3D() const
{
	if ( !value || type != le::SPT_VECTOR_3D )
		return le::Vector3D_t();

	return *static_cast< le::Vector3D_t* >( value );
}

// ------------------------------------------------------------------------------------ //
// Set value Vector2D
// ------------------------------------------------------------------------------------ //
void ShaderParameter::SetValueVector2D( const le::Vector2D_t& Value )
{
	if ( !shaderParameter || type != le::SPT_VECTOR_2D ) return;

	le::Vector2D_t* vector = new le::Vector2D_t( Value );
	value = vector;

	shaderParameter->SetValueVector2D( *vector );
}

// ------------------------------------------------------------------------------------ //
// Get value Vector2D
// ------------------------------------------------------------------------------------ //
le::Vector2D_t ShaderParameter::GetValueVector2D() const
{
	if ( !value || type != le::SPT_VECTOR_2D )
		return le::Vector2D_t();

	return *static_cast< le::Vector2D_t* >( value );
}

// ------------------------------------------------------------------------------------ //
// Set value int
// ------------------------------------------------------------------------------------ //
void ShaderParameter::SetValueInt( int Value )
{
	if ( !shaderParameter || type != le::SPT_INT ) return;

	int* vector = new int( Value );
	value = vector;

	shaderParameter->SetValueInt( *vector );
}

// ------------------------------------------------------------------------------------ //
// Get value int
// ------------------------------------------------------------------------------------ //
int ShaderParameter::GetValueInt() const
{
	if ( !value || type != le::SPT_INT )
		return 0;

	return *static_cast< int* >( value );
}

// ------------------------------------------------------------------------------------ //
// Set value float
// ------------------------------------------------------------------------------------ //
void ShaderParameter::SetValueFloat( float Value )
{
	if ( !shaderParameter || type != le::SPT_FLOAT ) return;

	float* vector = new float( Value );
	value = vector;

	shaderParameter->SetValueFloat( *vector );
}

// ------------------------------------------------------------------------------------ //
// Get value float
// ------------------------------------------------------------------------------------ //
float ShaderParameter::GetValueFloat() const
{
	if ( !value || type != le::SPT_FLOAT )
		return 0.f;

	return *static_cast< float* >( value );
}

// ------------------------------------------------------------------------------------ //
// Set value bool
// ------------------------------------------------------------------------------------ //
void ShaderParameter::SetValueBool( bool Value )
{
	if ( !shaderParameter || type != le::SPT_SHADER_FLAG ) return;

	bool* vector = new bool( Value );
	value = vector;

	shaderParameter->SetValueShaderFlag( *vector );
}

// ------------------------------------------------------------------------------------ //
// Get value bool
// ------------------------------------------------------------------------------------ //
bool ShaderParameter::GetValueBool() const
{
	if ( !value || type != le::SPT_SHADER_FLAG )
		return 0.f;

	return *static_cast< bool* >( value );
}