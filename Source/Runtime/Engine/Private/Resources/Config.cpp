// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Logging/LogMacros.h"
#include "System/FileSystem.h"
#include "Resources/Parsers/IParserConfig.h"
#include "Resources/Parsers/ParsersConfigFactory.h"
#include "Resources/Config.h"

/**
 * Serialize resource
 */
bool le::Config::Serialize( const Path& InPath )
{
	LIFEENGINE_LOG_WARNING( "Engine", "le::Config::Serialize( const Path& InPath ) :: Not implemented" );
	return false;
}

/**
 * Deserialize resource
 */
bool le::Config::Deserialize( const Path& InPath )
{
	if ( InPath.IsEmpty() )		return false;

	IParserConfig*			parser = GParsersConfigFactory->Get( InPath.GetExtension() );
	FFileHandle				fileHandle = GFileSystem->OpenFile( InPath );
	if ( !fileHandle || !parser )
	{
		if ( parser )			parser->ReleaseRef();
		if ( fileHandle )		GFileSystem->CloseFile( fileHandle );
		return false;
	}

	parser->SetFile( fileHandle );
	groups = parser->GetGroups();

	parser->ReleaseRef();
	GFileSystem->CloseFile( fileHandle );
	return true;
}

/**
 * Get type resource
 */
le::EResourceType le::Config::GetType() const
{
	return RT_Config;
}

/**
 * Constructor
 */
le::Config::Var::Var() :
	type( VT_Unknown ),
	value( nullptr )
{}

/**
 * Constructor of copy
 */
le::Config::Var::Var( const Var& InCopy ) :
	name( InCopy.name )
{
	switch ( InCopy.type )
	{
	case VT_Int:		SetValueInt( InCopy.GetValueInt() );		break;
	case VT_Float:		SetValueFloat( InCopy.GetValueFloat() );	break;
	case VT_String:		SetValueString( InCopy.GetValueString() );	break;
	}
}

/**
 * Destructor
 */
le::Config::Var::~Var()
{
	DestroyValue();
}

/**
 * Set value int
 */
void le::Config::Var::SetValueInt( int InValue )
{
	if ( !IsEmpty() && type != VT_Int )		DestroyValue();
	if ( IsEmpty() )		value = new int;

	*static_cast< int* >( value ) = InValue;
	type = VT_Int;
}

/**
 * Set value float
 */
void le::Config::Var::SetValueFloat( float InValue )
{
	if ( !IsEmpty() && type != VT_Float )		DestroyValue();
	if ( IsEmpty() )		value = new float;

	*static_cast< float* >( value ) = InValue;
	type = VT_Float;
}

/**
 * Set value string
 */
void le::Config::Var::SetValueString( const std::string& InValue )
{
	if ( !IsEmpty() && type != VT_String )		DestroyValue();
	if ( IsEmpty() )		value = new std::string();

	*static_cast< std::string* >( value ) = InValue;
	type = VT_String;
}

/**
 * Get value int
 */
int le::Config::Var::GetValueInt() const
{
	if ( type != VT_Int )	return 0;
	return *static_cast< int* >( value );
}

/**
 * Get value float
 */
float le::Config::Var::GetValueFloat() const
{
	if ( type != VT_Float )		return 0.f;
	return *static_cast< float* >( value );
}

/**
 * Get value string
 */
std::string le::Config::Var::GetValueString() const
{
	if ( type != VT_String )		return std::string();
	return *static_cast< std::string* >( value );
}

/**
 * Destroy value
 */
void le::Config::Var::DestroyValue()
{
	if ( IsEmpty() )		return;

	switch ( type )
	{
	case VT_Int:		delete static_cast< int* >( value );			break;
	case VT_Float:		delete static_cast< float* >( value );			break;
	case VT_String:		delete static_cast< std::string* >( value );	break;
	}

	value = nullptr;
	type = VT_Unknown;
}
