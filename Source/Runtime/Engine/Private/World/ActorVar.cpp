// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "World/ActorVar.h"
#include "Resources/Material.h"

/**
 * Constructor
 */
le::ActorVar::ActorVar() :
	type( AVT_Unknown ),
	value( nullptr )
{}

/**
 * Constructor of copy
 */
le::ActorVar::ActorVar( const ActorVar& InCopy ) :
	name( InCopy.name )
{
	switch ( InCopy.type )
	{
	case AVT_Int:		SetValueInt( InCopy.GetValueInt() );				break;
	case AVT_Float:		SetValueFloat( InCopy.GetValueFloat() );			break;
	case AVT_Bool:		SetValueBool( InCopy.GetValueBool() );				break;
	case AVT_Vector2D:	SetValueVector2D( InCopy.GetValueVector2D() );		break;
	case AVT_Vector3D:	SetValueVector3D( InCopy.GetValueVector3D() );		break;
	case AVT_Vector4D:	SetValueVector4D( InCopy.GetValueVector4D() );		break;
	case AVT_RectInt:	SetValueRectInt( InCopy.GetValueRectInt() );		break;
	case AVT_RectFloat:	SetValueRectFloat( InCopy.GetValueRectFloat() );	break;
	case AVT_Color:		SetValueColor( InCopy.GetValueColor() );			break;
	case AVT_Material:	SetValueMaterial( InCopy.GetValueMaterial() );		break;

	default:
		type = AVT_Unknown;
		value = nullptr;
		break;
	}
}

/**
 * Destructor
 */
le::ActorVar::~ActorVar()
{
	Clear();
}

/**
 * Clear value
 */
void le::ActorVar::Clear()
{
	if ( !value )		return;

	switch ( type )
	{
	case AVT_Int:		delete static_cast< int* >( value );			break;
	case AVT_Float:		delete static_cast< float* >( value );			break;
	case AVT_Bool:		delete static_cast< bool* >( value );			break;
	case AVT_Vector2D:	delete static_cast< FVector2D* >( value );		break;
	case AVT_Vector3D:	delete static_cast< FVector3D* >( value );		break;
	case AVT_Vector4D:	delete static_cast< FVector4D* >( value );		break;
	case AVT_RectInt:	delete static_cast< FSRectInt* >( value );		break;
	case AVT_RectFloat:	delete static_cast< FSRectFloat* >( value );	break;
	case AVT_Color:		delete static_cast< SColor* >( value );			break;
	case AVT_Material:
	{
		Material*		material = static_cast< Material* >( value );
		material->ReleaseRef();
	}
	}

	value = nullptr;
	type = AVT_Unknown;
}

/**
 * Set value int
 */
void le::ActorVar::SetValueInt( int InValue )
{
	if ( value && type != AVT_Int )			Clear();
	if ( !value )							value = new int;

	*static_cast< int* >( value ) = InValue;
	type = AVT_Int;
}

/**
 * Set value float
 */
void le::ActorVar::SetValueFloat( float InValue )
{
	if ( value && type != AVT_Float )		Clear();
	if ( !value )							value = new float;

	*static_cast< float* >( value ) = InValue;
	type = AVT_Float;
}

/**
 * Set value bool
 */
void le::ActorVar::SetValueBool( bool InValue )
{
	if ( value && type != AVT_Bool )		Clear();
	if ( !value )							value = new bool;

	*static_cast< bool* >( value ) = InValue;
	type = AVT_Bool;
}

/**
 * Set value vector 2D
 */
void le::ActorVar::SetValueVector2D( const FVector2D& InValue )
{
	if ( value && type != AVT_Vector2D )	Clear();
	if ( !value )							value = new FVector2D();

	*static_cast< FVector2D* >( value ) = InValue;
	type = AVT_Vector2D;
}

/**
 * Set value vector 3D
 */
void le::ActorVar::SetValueVector3D( const FVector3D& InValue )
{
	if ( value && type != AVT_Vector3D )	Clear();
	if ( !value )							value = new FVector3D();

	*static_cast< FVector3D* >( value ) = InValue;
	type = AVT_Vector3D;
}

/**
 * Set value vector 4D
 */
void le::ActorVar::SetValueVector4D( const FVector4D& InValue )
{
	if ( value && type != AVT_Vector4D )	Clear();
	if ( !value )							value = new FVector4D();

	*static_cast< FVector4D* >( value ) = InValue;
	type = AVT_Vector4D;
}

/**
 * Set value rect int
 */
void le::ActorVar::SetValueRectInt( const FSRectInt& InValue )
{
	if ( value && type != AVT_RectInt )		Clear();
	if ( !value )							value = new FSRectInt();

	*static_cast< FSRectInt* >( value ) = InValue;
	type = AVT_RectInt;
}

/**
 * Set value rect float
 */
void le::ActorVar::SetValueRectFloat( const FSRectFloat& InValue )
{
	if ( value && type != AVT_RectFloat )	Clear();
	if ( !value )							value = new FSRectFloat();

	*static_cast< FSRectFloat* >( value ) = InValue;
	type = AVT_RectFloat;
}

/**
 * Set value color
 */
void le::ActorVar::SetValueColor( const SColor& InValue )
{
	if ( value && type != AVT_Color )		Clear();
	if ( !value )							value = new SColor();

	*static_cast< SColor* >( value ) = InValue;
	type = AVT_Color;
}

/**
 * Set value material
 */
void le::ActorVar::SetValueMaterial( Material* InValue )
{
	if ( value )		Clear();
	if ( InValue )		InValue->AddRef();

	value = InValue;
	type = AVT_Material;
}

/**
 * Get value int
 */
int le::ActorVar::GetValueInt() const
{
	if ( type != AVT_Int )			return 0;
	return *static_cast< int* >( value );
}

/**
 * Get value float
 */
float le::ActorVar::GetValueFloat() const
{
	if ( type != AVT_Float )		return 0.f;
	return *static_cast< float* >( value );
}

/**
 * Get value bool
 */
bool le::ActorVar::GetValueBool() const
{
	if ( type != AVT_Bool )			return false;
	return *static_cast< bool* >( value );
}

/**
 * Get value vector 2D
 */
le::FVector2D le::ActorVar::GetValueVector2D() const
{
	if ( type != AVT_Vector2D )		return FVector2D();
	return *static_cast< FVector2D* >( value );
}

/**
 * Get value vector 3D
 */
le::FVector3D le::ActorVar::GetValueVector3D() const
{
	if ( type != AVT_Vector3D )		return FVector3D();
	return *static_cast< FVector3D* >( value );
}

/**
 * Get value vector 4D
 */
le::FVector4D le::ActorVar::GetValueVector4D() const
{
	if ( type != AVT_Vector4D )		return FVector4D();
	return *static_cast< FVector4D* >( value );
}

/**
 * Get value rect int
 */
le::FSRectInt le::ActorVar::GetValueRectInt() const
{
	if ( type != AVT_RectInt )		return FSRectInt();
	return *static_cast< FSRectInt* >( value );
}

/**
 * Get value rect float
 */
le::FSRectFloat le::ActorVar::GetValueRectFloat() const
{
	if ( type != AVT_RectFloat )	return FSRectFloat();
	return *static_cast< FSRectFloat* >( value );
}

/**
 * Get value color
 */
le::SColor le::ActorVar::GetValueColor() const
{
	if ( type != AVT_Color )		return SColor();
	return *static_cast< SColor* >( value );
}

/**
 * Get value material
 */
le::Material* le::ActorVar::GetValueMaterial() const
{
	if ( type != AVT_Material )		return nullptr;
	return static_cast< Material* >( value );
}