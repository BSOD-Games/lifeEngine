// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Math/Color.h"
#include "Rendering/ShaderVar.h"
#include "Resources/Texture2D.h"
#include "Resources/Material.h"

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
	case SVT_Vector3D:	SetValueVector3D( InCopy.GetValueVector3D() );		break;
	case SVT_Vector4D:	SetValueVector4D( InCopy.GetValueVector4D() );		break;
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
	case SVT_Int:		delete static_cast< int* >( value );			break;
	case SVT_Float:		delete static_cast< float* >( value );			break;
	case SVT_Bool:		delete static_cast< bool* >( value );			break;
	case SVT_Vector2D:	delete static_cast< FVector2D* >( value );		break;
	case SVT_Vector3D:	delete static_cast< FVector3D* >( value );		break;
	case SVT_Vector4D:	delete static_cast< FVector4D* >( value );		break;
	case SVT_Color:		delete static_cast< SColor* >( value );			break;
	case SVT_Texture2D:	delete static_cast< FTexture2DRef* >( value );	break;
	}

	value = nullptr;
	NotifyMaterials();
}

/**
 * Subscribe material
 */
void le::ShaderVar::SubscribeMaterial( FMaterialConstRef& InMaterial )
{
	LIFEENGINE_ASSERT( InMaterial );
	materials.push_back( InMaterial );
}

/**
 * Unsubscribe material
 */
void le::ShaderVar::UnsubscribeMaterial( FMaterialConstRef& InMaterial )
{
	LIFEENGINE_ASSERT( InMaterial );
	for ( uint32 index = 0, count = static_cast< uint32 >( materials.size() ); index < count; ++index )
		if ( materials[ index ] == InMaterial )
		{
			materials.erase( index + materials.begin() );
			return;
		}
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

	NotifyMaterials();	
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

	NotifyMaterials();
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

	NotifyMaterials();
}

/**
 * Set value vector 2D
 */
void le::ShaderVar::SetValueVector2D( const FVector2D& InValue )
{
	if ( value && type != SVT_Vector2D )	Clear();
	if ( !value )							value = new FVector2D();

	*static_cast< FVector2D* >( value ) = InValue;
	type = SVT_Vector2D;

	NotifyMaterials();
}

/**
 * Set value vector 3D
 */
void le::ShaderVar::SetValueVector3D( const FVector3D& InValue )
{
	if ( value && type != SVT_Vector3D )	Clear();
	if ( !value )							value = new FVector3D();

	*static_cast< FVector3D* >( value ) = InValue;
	type = SVT_Vector3D;

	NotifyMaterials();
}

/**
 * Set value vector 4D
 */
void le::ShaderVar::SetValueVector4D( const FVector4D& InValue )
{
	if ( value && type != SVT_Vector4D )	Clear();
	if ( !value )							value = new FVector4D();

	*static_cast< FVector4D* >( value ) = InValue;
	type = SVT_Vector4D;

	NotifyMaterials();
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

	NotifyMaterials();
}

/**
 * Set value texture 2D
 */
void le::ShaderVar::SetValueTexture2D( FTexture2DConstRef& InValue )
{
	if ( value && type != SVT_Texture2D )	Clear();
	if ( !value )							value = new FTexture2DRef();

	*static_cast< FTexture2DRef* >( value ) = InValue;
	type = SVT_Texture2D;

	NotifyMaterials();
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
 * Get value vector 2D
 */
le::FVector2D le::ShaderVar::GetValueVector2D() const
{
	if ( type != SVT_Vector2D )		return FVector2D();
	return *static_cast< FVector2D* >( value );
}

/**
 * Get value vector 3D
 */
le::FVector3D le::ShaderVar::GetValueVector3D() const
{
	if ( type != SVT_Vector3D )		return FVector3D();
	return *static_cast< FVector3D* >( value );
}

/**
 * Get value vector 4D
 */
le::FVector4D le::ShaderVar::GetValueVector4D() const
{
	if ( type != SVT_Vector4D )		return FVector4D();
	return *static_cast< FVector4D* >( value );
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
le::FTexture2DRef le::ShaderVar::GetValueTexture2D() const
{
	if ( type != SVT_Texture2D )	return FTexture2DRef();
	return *static_cast< FTexture2DRef* >( value );
}

/**
 * Notify materials
 */
void le::ShaderVar::NotifyMaterials()
{
	for ( uint32 index = 0, count = static_cast< uint32 >( materials.size() ); index < count; ++index )
		materials[ index ]->NeadUpdateShader();
}
