//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "lmtparameter.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
LMTParameter::LMTParameter() :
	isDefined( false ),
	value( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Copy
// ------------------------------------------------------------------------------------ //
LMTParameter::LMTParameter( const LMTParameter& Copy )
{
	name = Copy.name;
	isDefined = Copy.isDefined;
	type = Copy.type;

	if ( isDefined && Copy.value )
		CopyValue( Copy.value, type );
	else
		value = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
LMTParameter::~LMTParameter()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void LMTParameter::Clear()
{
	DeleteValue();
	name = "";
}

// ------------------------------------------------------------------------------------ //
// Set name
// ------------------------------------------------------------------------------------ //
void LMTParameter::SetName( const std::string& Name )
{
	name = Name;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTParameter::SetValueInt( int Value )
{
	if ( isDefined && type != PT_INT )       DeleteValue();
	if ( !isDefined )                        value = new int( 0 );

	*static_cast< int* >( value ) = Value;

	type = PT_INT;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTParameter::SetValueFloat( float Value )
{
	if ( isDefined && type != PT_FLOAT )		DeleteValue();
	if ( !isDefined )							value = new float( 0.f );

	*static_cast< float* >( value ) = Value;

	type = PT_FLOAT;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTParameter::SetValueBool( bool Value )
{
	if ( isDefined && type != PT_BOOL )       DeleteValue();
	if ( !isDefined )                         value = new bool( false );

	*static_cast< bool* >( value ) = Value;

	type = PT_BOOL;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTParameter::SetValueVector2D( const LMTVector2D& Value )
{
	if ( isDefined && type != PT_VECTOR_2D )    DeleteValue();
	if ( !isDefined )							value = new LMTVector2D();

	*static_cast< LMTVector2D* >( value ) = Value;

	type = PT_VECTOR_2D;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTParameter::SetValueVector3D( const LMTVector3D& Value )
{
	if ( isDefined && type != PT_VECTOR_3D )       DeleteValue();
	if ( !isDefined )								value = new LMTVector3D();

	*static_cast< LMTVector3D* >( value ) = Value;

	type = PT_VECTOR_3D;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTParameter::SetValueVector4D( const LMTVector4D& Value )
{
	if ( isDefined && type != PT_VECTOR_4D )       DeleteValue();
	if ( !isDefined )								value = new LMTVector4D();

	*static_cast< LMTVector4D* >( value ) = Value;

	type = PT_VECTOR_4D;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTParameter::SetValueColor( const LMTColor& Value )
{
	if ( isDefined && type != PT_COLOR )			DeleteValue();
	if ( !isDefined )								value = new LMTColor();

	*static_cast< LMTColor* >( value ) = Value;

	type = PT_COLOR;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTParameter::SetValueTexture( const std::string& Path )
{
	if ( isDefined && type != PT_TEXTURE )			DeleteValue();
	if ( !isDefined )								value = new std::string();

	*static_cast< std::string* >( value ) = Path;

	type = PT_TEXTURE;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Is defined
// ------------------------------------------------------------------------------------ //
bool LMTParameter::IsDefined() const
{
	return isDefined;
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const std::string& LMTParameter::GetName() const
{
	return name;
}

// ------------------------------------------------------------------------------------ //
// Get type
// ------------------------------------------------------------------------------------ //
LMTParameter::PARAMETER_TYPE LMTParameter::GetType() const
{
	return type;
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
int LMTParameter::GetValueInt() const
{
	if ( !value || type != PT_INT )
		return 0;

	return *static_cast< int* >( value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
float LMTParameter::GetValueFloat() const
{
	if ( !value || type != PT_FLOAT )
		return 0.f;

	return *static_cast< float* >( value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
bool LMTParameter::GetValueBool() const
{
	if ( !value || type != PT_BOOL )
		return false;

	return *static_cast< bool* >( value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
const LMTVector2D LMTParameter::GetValueVector2D() const
{
	if ( !value || type != PT_VECTOR_2D )
		return LMTVector2D();

	return *static_cast< LMTVector2D* >( value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
const LMTVector3D LMTParameter::GetValueVector3D() const
{
	if ( !value || type != PT_VECTOR_3D )
		return LMTVector3D();

	return *static_cast< LMTVector3D* >( value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
const LMTVector4D LMTParameter::GetValueVector4D() const
{
	if ( !value || type != PT_VECTOR_4D )
		return LMTVector4D();

	return *static_cast< LMTVector4D* >( value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
const LMTColor LMTParameter::GetValueColor() const
{
	if ( !value || type != PT_COLOR )
		return LMTColor();

	return *static_cast< LMTColor* >( value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
const std::string LMTParameter::GetValueTexture() const
{
	if ( !value || type != PT_TEXTURE )
		return std::string();

	return *static_cast< std::string* >( value );
}

// ------------------------------------------------------------------------------------ //
// Delete value
// ------------------------------------------------------------------------------------ //
void LMTParameter::DeleteValue()
{
	if ( !isDefined ) return;

	switch ( type )
	{
	case PT_INT:            delete static_cast< int* >( value );			break;
	case PT_FLOAT:          delete static_cast< float* >( value );			break;
	case PT_BOOL:			delete static_cast< bool* >( value );			break;
	case PT_VECTOR_2D:      delete static_cast< LMTVector2D* >( value );	break;
	case PT_VECTOR_3D:      delete static_cast< LMTVector3D* >( value );	break;
	case PT_VECTOR_4D:      delete static_cast< LMTVector4D* >( value );	break;
	case PT_COLOR:          delete static_cast< LMTColor* >( value );		break;
	case PT_TEXTURE:		delete static_cast< std::string* >( value );	break;
	}

	value = nullptr;
	isDefined = false;
}

// ------------------------------------------------------------------------------------ //
// Copy value
// ------------------------------------------------------------------------------------ //
void LMTParameter::CopyValue( void* Value, PARAMETER_TYPE Type )
{
	if ( !Value ) return;

	switch ( type )
	{
	case PT_INT:            value = new int( *static_cast< int* >( Value ) );					break;
	case PT_FLOAT:			value = new float( *static_cast< float* >( Value ) );				break;
	case PT_BOOL:			value = new bool( *static_cast< bool* >( Value ) );					break;
	case PT_VECTOR_2D:		value = new LMTVector2D( *static_cast< LMTVector2D* >( Value ) );	break;
	case PT_VECTOR_3D:      value = new LMTVector3D( *static_cast< LMTVector3D* >( Value ) );	break;
	case PT_VECTOR_4D:      value = new LMTVector4D( *static_cast< LMTVector4D* >( Value ) );	break;
	case PT_COLOR:          value = new LMTColor( *static_cast< LMTColor* >( Value ) );			break;
	case PT_TEXTURE:		value = new std::string( *static_cast< std::string* >( Value ) );	break;
	}

	isDefined = true;
	type = Type;
}
