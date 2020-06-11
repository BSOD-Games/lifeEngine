//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "lmtproxyparameter.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
LMTProxyParameter::LMTProxyParameter() :
	isDefined( false ),
	value( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Copy
// ------------------------------------------------------------------------------------ //
LMTProxyParameter::LMTProxyParameter( const LMTProxyParameter& Copy )
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
LMTProxyParameter::~LMTProxyParameter()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::Clear()
{
	DeleteValue();
	name = "";
}

// ------------------------------------------------------------------------------------ //
// Set name
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::SetName( const std::string& Name )
{
	name = Name;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::SetValueInt( int Value )
{
	if ( isDefined && type != PT_INT )			DeleteValue();
	if ( !isDefined )							value = new int();

	*static_cast< int* >( value ) = Value;

	type = PT_INT;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::SetValueFloat( float Value )
{
	if ( isDefined && type != PT_FLOAT )			DeleteValue();
	if ( !isDefined )								value = new float();

	*static_cast< float* >( value ) = Value;

	type = PT_FLOAT;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::SetValueBool( bool Value )
{
	if ( isDefined && type != PT_BOOL )			DeleteValue();
	if ( !isDefined )							value = new bool();

	*static_cast< bool* >( value ) = Value;

	type = PT_BOOL;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::SetValueVector2D( const LMTVector2D & Value )
{
	if ( isDefined && type != PT_VECTOR_2D )			DeleteValue();
	if ( !isDefined )									value = new LMTVector2D();

	*static_cast< LMTVector2D* >( value ) = Value;

	type = PT_VECTOR_2D;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::SetValueVector3D( const LMTVector3D & Value )
{
	if ( isDefined && type != PT_VECTOR_3D )			DeleteValue();
	if ( !isDefined )									value = new LMTVector3D();

	*static_cast< LMTVector3D* >( value ) = Value;

	type = PT_VECTOR_3D;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::SetValueVector4D( const LMTVector4D & Value )
{
	if ( isDefined && type != PT_VECTOR_4D )			DeleteValue();
	if ( !isDefined )									value = new LMTVector4D();

	*static_cast< LMTVector4D* >( value ) = Value;

	type = PT_VECTOR_4D;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::SetValueShaderParameter( const std::string& Value )
{
	if ( isDefined && type != PT_SHADER_PARAMETER )			DeleteValue();
	if ( !isDefined )										value = new std::string();

	*static_cast< std::string* >( value ) = Value;

	type = PT_SHADER_PARAMETER;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::SetValueArrayFloat( const std::vector<float> & Array )
{
	if ( isDefined && type != PT_ARRAY_FLOAT )      DeleteValue();
	if ( !isDefined )                               value = new std::vector< float >();

	auto            valueArray = static_cast< std::vector< float >* >( value );
	valueArray->resize( Array.size() );
	memcpy( valueArray->data(), Array.data(), Array.size() * sizeof( float ) );

	type = PT_ARRAY_FLOAT;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::SetValueArrayInt( const std::vector<int> & Array )
{
	if ( isDefined && type != PT_ARRAY_INT )        DeleteValue();
	if ( !isDefined )                               value = new std::vector< int >();

	auto            valueArray = static_cast< std::vector< int >* >( value );
	valueArray->resize( Array.size() );
	memcpy( valueArray->data(), Array.data(), Array.size() * sizeof( int ) );

	type = PT_ARRAY_INT;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::SetValueArrayVector2D( const std::vector<LMTVector2D> & Array )
{
	if ( isDefined && type != PT_ARRAY_VECTOR_2D )        DeleteValue();
	if ( !isDefined )                                     value = new std::vector< LMTVector2D >();

	auto            valueArray = static_cast< std::vector< LMTVector2D >* >( value );
	valueArray->resize( Array.size() );
	memcpy( valueArray->data(), Array.data(), Array.size() * sizeof( LMTVector2D ) );

	type = PT_ARRAY_VECTOR_2D;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::SetValueArrayVector3D( const std::vector<LMTVector3D>& Array )
{
	if ( isDefined && type != PT_ARRAY_VECTOR_3D )        DeleteValue();
	if ( !isDefined )                                     value = new std::vector< LMTVector3D >();

	auto            valueArray = static_cast< std::vector< LMTVector3D >* >( value );
	valueArray->resize( Array.size() );
	memcpy( valueArray->data(), Array.data(), Array.size() * sizeof( LMTVector3D ) );

	type = PT_ARRAY_VECTOR_3D;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::SetValueArrayVector4D( const std::vector<LMTVector4D>& Array )
{
	if ( isDefined && type != PT_ARRAY_VECTOR_4D )        DeleteValue();
	if ( !isDefined )                                     value = new std::vector< LMTVector4D >();

	auto            valueArray = static_cast< std::vector< LMTVector4D >* >( value );
	valueArray->resize( Array.size() );
	memcpy( valueArray->data(), Array.data(), Array.size() * sizeof( LMTVector4D ) );

	type = PT_ARRAY_VECTOR_4D;
	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Is defined
// ------------------------------------------------------------------------------------ //
bool LMTProxyParameter::IsDefined() const
{
	return isDefined;
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const std::string& LMTProxyParameter::GetName() const
{
	return name;
}

// ------------------------------------------------------------------------------------ //
// Get type
// ------------------------------------------------------------------------------------ //
LMTProxyParameter::PARAMETER_TYPE LMTProxyParameter::GetType() const
{
	return type;
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
int LMTProxyParameter::GetValueInt() const
{
	if ( type != PT_INT )
		return 0;

	return *static_cast< int* >( this->value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
float LMTProxyParameter::GetValueFloat() const
{
	if ( type != PT_FLOAT )
		return 0.f;

	return *static_cast< float* >( this->value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
bool LMTProxyParameter::GetValueBool() const
{
	if ( type != PT_BOOL )
		return false;

	return *static_cast< bool* >( this->value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
LMTVector2D LMTProxyParameter::GetValueVector2D() const
{
	if ( type != PT_VECTOR_2D )
		return LMTVector2D();

	return *static_cast< LMTVector2D* >( this->value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
LMTVector3D LMTProxyParameter::GetValueVector3D() const
{
	if ( type != PT_VECTOR_3D )
		return LMTVector3D();

	return *static_cast< LMTVector3D* >( this->value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
LMTVector4D LMTProxyParameter::GetValueVector4D() const
{
	if ( type != PT_VECTOR_4D )
		return LMTVector4D();

	return *static_cast< LMTVector4D* >( this->value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
std::string LMTProxyParameter::GetValueShaderParameter() const
{
	if ( type != PT_SHADER_PARAMETER )
		return std::string();

	return *static_cast< std::string* >( this->value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
std::vector<float> LMTProxyParameter::GetValueArrayFloat() const
{
	if ( type != PT_ARRAY_FLOAT )
		return std::vector<float>();

	return *static_cast< std::vector< float >* >( this->value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
std::vector<int> LMTProxyParameter::GetValueArrayInt() const
{
	if ( type != PT_ARRAY_INT )
		return std::vector<int>();

	return *static_cast< std::vector< int >* >( this->value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
std::vector<LMTVector2D> LMTProxyParameter::GetValueArrayVector2D() const
{
	if ( type != PT_ARRAY_VECTOR_2D )
		return std::vector<LMTVector2D>();

	return *static_cast< std::vector< LMTVector2D >* >( this->value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
std::vector<LMTVector3D> LMTProxyParameter::GetValueArrayVector3D() const
{
	if ( type != PT_ARRAY_VECTOR_3D )
		return std::vector<LMTVector3D>();

	return *static_cast< std::vector< LMTVector3D >* >( this->value );
}

// ------------------------------------------------------------------------------------ //
// Get value
// ------------------------------------------------------------------------------------ //
std::vector<LMTVector4D> LMTProxyParameter::GetValueArrayVector4D() const
{
	if ( type != PT_ARRAY_VECTOR_4D )
		return std::vector<LMTVector4D>();

	return *static_cast< std::vector< LMTVector4D >* >( this->value );
}

// ------------------------------------------------------------------------------------ //
// Delete value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::DeleteValue()
{
	if ( !isDefined ) return;

	switch ( type )
	{
	case PT_INT:					delete static_cast< int* >( value );							break;
	case PT_FLOAT:					delete static_cast< float* >( value );							break;
	case PT_BOOL:					delete static_cast< bool* >( value );							break;
	case PT_VECTOR_2D:				delete static_cast< LMTVector2D* >( value );					break;
	case PT_VECTOR_3D:				delete static_cast< LMTVector3D* >( value );					break;
	case PT_VECTOR_4D:				delete static_cast< LMTVector4D* >( value );					break;
	case PT_ARRAY_FLOAT:			delete static_cast< std::vector< float >* >( value );			break;
	case PT_ARRAY_INT:				delete static_cast< std::vector< int >* >( value );				break;
	case PT_ARRAY_VECTOR_2D:		delete static_cast< std::vector< LMTVector2D >* >( value );		break;
	case PT_ARRAY_VECTOR_3D:		delete static_cast< std::vector< LMTVector3D >* >( value );		break;
	case PT_ARRAY_VECTOR_4D:		delete static_cast< std::vector< LMTVector4D >* >( value );		break;
	case PT_SHADER_PARAMETER:		delete static_cast< std::string* >( value );					break;
	}

	type = PT_NONE;
	isDefined = false;
	value = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Copy value
// ------------------------------------------------------------------------------------ //
void LMTProxyParameter::CopyValue( void* Value, PARAMETER_TYPE Type )
{
	if ( !Value ) return;

	switch ( type )
	{
	case PT_INT:					value = new int( *static_cast< int* >( Value ) );													break;
	case PT_FLOAT:					value = new float( *static_cast< float* >( Value ) );												break;
	case PT_BOOL:					value = new bool( *static_cast< bool* >( Value ) );													break;
	case PT_VECTOR_2D:				value = new LMTVector2D( *static_cast< LMTVector2D* >( Value ) );									break;
	case PT_VECTOR_3D:				value = new LMTVector3D( *static_cast< LMTVector3D* >( Value ) );									break;
	case PT_VECTOR_4D:				value = new LMTVector4D( *static_cast< LMTVector4D* >( Value ) );									break;
	case PT_ARRAY_FLOAT:			value = new std::vector< float >( *static_cast< std::vector< float >* >( Value ) );					break;
	case PT_ARRAY_INT:				value = new std::vector< int >( *static_cast< std::vector< int >* >( Value ) );						break;
	case PT_ARRAY_VECTOR_2D:		value = new std::vector< LMTVector2D >( *static_cast< std::vector< LMTVector2D >* >( Value ) );		break;
	case PT_ARRAY_VECTOR_3D:		value = new std::vector< LMTVector3D >( *static_cast< std::vector< LMTVector3D >* >( Value ) );		break;
	case PT_ARRAY_VECTOR_4D:		value = new std::vector< LMTVector4D >( *static_cast< std::vector< LMTVector4D >* >( Value ) );		break;
	case PT_SHADER_PARAMETER:		value = new std::string( *static_cast< std::string* >( Value ) );									break;
	}

	isDefined = true;
	type = Type;
}
