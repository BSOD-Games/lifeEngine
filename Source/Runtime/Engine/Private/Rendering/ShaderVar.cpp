// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Math/Color.h"
#include "Math/Vector2D.h"
#include "Rendering/ShaderVar.h"
#include "Resources/Texture2D.h"

/**
 * Constructor
 */
le::ShaderVar::ShaderVar() :
	type( SVT_Unknown ),
	value( nullptr )
{}

/**
 * Constructor of copy
 */
le::ShaderVar::ShaderVar( const ShaderVar& InCopy ) :
	name( InCopy.name )
{
	switch ( InCopy.type )
	{
	case SVT_Int:		SetValueInt( InCopy.GetValueInt() );				break;
	case SVT_Float:		SetValueFloat( InCopy.GetValueFloat() );			break;
	case SVT_Bool:		SetValueBool( InCopy.GetValueBool() );				break;
	case SVT_Vector2D:	SetValueVector2D( InCopy.GetValueVector2D() );		break;
	case SVT_Color:		SetValueColor( InCopy.GetValueColor() );			break;
	case SVT_Texture2D:	SetValueTexture2D( InCopy.GetValueTexture2D() );	break;

	default:
		type = SVT_Unknown;
		value = nullptr;
		break;
	}
}

/**
 * Destructor
 */
le::ShaderVar::~ShaderVar()
{
	Clear();
}

/**
 * Clear value
 */
void le::ShaderVar::Clear()
{
	if ( !value )		return;

	switch ( type )
	{
	case SVT_Int:		delete static_cast< int* >( value );		break;
	case SVT_Float:		delete static_cast< float* >( value );		break;
	case SVT_Bool:		delete static_cast< bool* >( value );		break;
	case SVT_Vector2D:	delete static_cast< SVector2D* >( value );	break;
	case SVT_Color:		delete static_cast< SColor* >( value );		break;
	case SVT_Texture2D:
	{
		Texture2D*			texture2D = static_cast< Texture2D* >( value );
		texture2D->ReleaseRef();
	}
	}

	value = nullptr;
}

/**
 * Set value int
 */
void le::ShaderVar::SetValueInt( int InValue )
{
	if ( value && type != SVT_Int )			Clear();
	if ( !value )							value = new int;

	*static_cast< int* >( value ) = InValue;
	type = SVT_Int;
}

/**
 * Set value float
 */
void le::ShaderVar::SetValueFloat( float InValue )
{
	if ( value && type != SVT_Float )		Clear();
	if ( !value )							value = new float;

	*static_cast< float* >( value ) = InValue;
	type = SVT_Float;
}

/**
 * Set value bool
 */
void le::ShaderVar::SetValueBool( bool InValue )
{
	if ( value && type != SVT_Bool )		Clear();
	if ( !value )							value = new bool;

	*static_cast< bool* >( value ) = InValue;
	type = SVT_Bool;
}

/**
 * Set value vector 2D
 */
void le::ShaderVar::SetValueVector2D( const SVector2D& InValue )
{
	if ( value && type != SVT_Vector2D )	Clear();
	if ( !value )							value = new SVector2D();

	*static_cast< SVector2D* >( value ) = InValue;
	type = SVT_Vector2D;
}

/**
 * Set value color
 */
void le::ShaderVar::SetValueColor( const SColor& InValue )
{
	if ( value && type != SVT_Color )		Clear();
	if ( !value )							value = new SColor();

	*static_cast< SColor* >( value ) = InValue;
	type = SVT_Color;
}

/**
 * Set value texture 2D
 */
void le::ShaderVar::SetValueTexture2D( Texture2D* InValue )
{
	if ( value )		Clear();
	if ( InValue )		InValue->AddRef();

	value = InValue;
	type = SVT_Texture2D;
}

/**
 * Get value int
 */
int le::ShaderVar::GetValueInt() const
{
	if ( type != SVT_Int )			return 0;
	return *static_cast< int* >( value );
}

/**
 * Get value float
 */
float le::ShaderVar::GetValueFloat() const
{
	if ( type != SVT_Float )		return 0.f;
	return *static_cast< float* >( value );
}

/**
 * Get value bool
 */
bool le::ShaderVar::GetValueBool() const
{
	if ( type != SVT_Bool )			return false;
	return *static_cast< bool* >( value );
}

/**
 * Get value vector
 */
le::SVector2D le::ShaderVar::GetValueVector2D() const
{
	if ( type != SVT_Vector2D )		return SVector2D();
	return *static_cast< SVector2D* >( value );
}

/**
 * Get value color
 */
le::SColor le::ShaderVar::GetValueColor() const
{
	if ( type != SVT_Color )		return SColor();
	return *static_cast< SColor* >( value );
}

/**
 * Get value texture 2D
 */
le::Texture2D* le::ShaderVar::GetValueTexture2D() const
{
	if ( type != SVT_Texture2D )	return nullptr;
	return static_cast< Texture2D* >( value );
}
