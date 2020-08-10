//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

extern "C"
{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#include "global.h"
#include "common/types.h"
#include "scriptsapi/mathlib/luavector3d.h"
#include "scriptsapi/mathlib/luaquaternion.h"

// ------------------------------------------------------------------------------------ //
// Register quaternion
// ------------------------------------------------------------------------------------ //
void le::LUAQuaternion::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Register Quaternion
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAQuaternion>( "Quaternion" ).
		addConstructor< void (*) () >().
		addProperty( "x", &LUAQuaternion::GetX, &LUAQuaternion::SetX ).
		addProperty( "y", &LUAQuaternion::GetY, &LUAQuaternion::SetY ).
		addProperty( "z", &LUAQuaternion::GetZ, &LUAQuaternion::SetZ ).
		addProperty( "w", &LUAQuaternion::GetW, &LUAQuaternion::SetW ).
		addFunction( "Normalize", &LUAQuaternion::Normalize ).
		addFunction( "Dot", &LUAQuaternion::Dot ).
		addFunction( "Cross", ( LUAQuaternion ( LUAQuaternion::* ) ( const LUAQuaternion& ) ) &LUAQuaternion::Cross ).
		addFunction( "Cross", ( LUAQuaternion ( LUAQuaternion::* ) ( const LUAVector3D& ) ) & LUAQuaternion::Cross ).
		addFunction( "Set", &LUAQuaternion::Set ).
		addFunction( "__tostring", &LUAQuaternion::ToString ).
		addFunction( "__add", ( LUAQuaternion ( LUAQuaternion::* )( const LUAQuaternion& ) ) & LUAQuaternion::operator+ ).
		addFunction( "__sub", ( LUAQuaternion ( LUAQuaternion::* )( const LUAQuaternion& ) ) & LUAQuaternion::operator- ).
		addFunction( "__mul", ( LUAQuaternion ( LUAQuaternion::* )( const LUAQuaternion& ) ) & LUAQuaternion::operator* ).
		addStaticFunction( "Normalize", ( LUAQuaternion(*) ( const LUAQuaternion& ) ) & LUAQuaternion::Normalize ).
		addStaticFunction( "Dot", ( float (*) ( const LUAQuaternion&, const LUAQuaternion& ) ) & LUAQuaternion::Dot ).
		addStaticFunction( "Cross", ( LUAQuaternion (*) ( const LUAQuaternion&, const LUAQuaternion& ) ) &LUAQuaternion::Cross ).
		addStaticFunction( "Cross", ( LUAQuaternion (*) ( const LUAQuaternion&, const LUAVector3D& ) ) & LUAQuaternion::Cross ).
		addStaticFunction( "Cross", ( LUAQuaternion (*) ( const LUAVector3D&, const LUAQuaternion& ) ) & LUAQuaternion::Cross ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAQuaternion::LUAQuaternion( const Quaternion_t& Copy ) :
	object( Copy )
{}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAQuaternion::LUAQuaternion( float X, float Y, float Z, float W ) :
	object( W, X, Y, Z )
{}

// ------------------------------------------------------------------------------------ //
// Normalize
// ------------------------------------------------------------------------------------ //
le::LUAQuaternion le::LUAQuaternion::Normalize( const LUAQuaternion& Quternion )
{
	return LUAQuaternion( glm::normalize( Quternion.GetHandle() ) );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAQuaternion::Dot( const LUAQuaternion& Left, const LUAQuaternion& Right )
{
	return glm::dot( Left.GetHandle(), Right.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Cross
// ------------------------------------------------------------------------------------ //
le::LUAQuaternion le::LUAQuaternion::Cross( const LUAQuaternion& Left, const LUAQuaternion& Right )
{
	return LUAQuaternion( glm::cross( Left.GetHandle(), Right.GetHandle() ) );
}

// ------------------------------------------------------------------------------------ //
// Cross
// ------------------------------------------------------------------------------------ //
le::LUAQuaternion le::LUAQuaternion::Cross( const LUAQuaternion& Left, const LUAVector3D& Right )
{
	return LUAQuaternion( glm::cross( Left.GetHandle(), Right.GetHandle() ) );
}

// ------------------------------------------------------------------------------------ //
// Cross
// ------------------------------------------------------------------------------------ //
le::LUAQuaternion le::LUAQuaternion::Cross( const LUAVector3D& Left, const LUAQuaternion& Right )
{
	return LUAQuaternion( glm::cross( Left.GetHandle(), Right.GetHandle() ) );
}

// ------------------------------------------------------------------------------------ //
// Normalize
// ------------------------------------------------------------------------------------ //
void le::LUAQuaternion::Normalize()
{
	glm::normalize( object );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAQuaternion::Dot( const LUAQuaternion& Right )
{
	return glm::dot( object, Right.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set
// ------------------------------------------------------------------------------------ //
void le::LUAQuaternion::Set( float X, float Y, float Z, float W )
{
	object.x = X;
	object.y = Y;
	object.z = Z;
	object.w = W;
}

// ------------------------------------------------------------------------------------ //
// Cross
// ------------------------------------------------------------------------------------ //
le::LUAQuaternion le::LUAQuaternion::Cross( const LUAQuaternion& Right )
{
	return LUAQuaternion( Right.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Cross
// ------------------------------------------------------------------------------------ //
le::LUAQuaternion le::LUAQuaternion::Cross( const LUAVector3D& Right )
{
	return LUAQuaternion( Right.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// To string
// ------------------------------------------------------------------------------------ //
std::string le::LUAQuaternion::ToString()
{
	return std::to_string( object.x ) + ", " + std::to_string( object.y ) + ", " + std::to_string( object.z ) + ", " + std::to_string( object.w );
}

// ------------------------------------------------------------------------------------ //
// Set x
// ------------------------------------------------------------------------------------ //
void le::LUAQuaternion::SetX( float X )
{
	object.x = X;
}

// ------------------------------------------------------------------------------------ //
// Set y
// ------------------------------------------------------------------------------------ //
void le::LUAQuaternion::SetY( float Y )
{
	object.y = Y;
}

// ------------------------------------------------------------------------------------ //
// Set z
// ------------------------------------------------------------------------------------ //
void le::LUAQuaternion::SetZ( float Z )
{
	object.z = Z;
}

// ------------------------------------------------------------------------------------ //
// Set w
// ------------------------------------------------------------------------------------ //
void le::LUAQuaternion::SetW( float W )
{
	object.w = W;
}

// ------------------------------------------------------------------------------------ //
// Get x
// ------------------------------------------------------------------------------------ //
float le::LUAQuaternion::GetX() const
{
	return object.x;
}

// ------------------------------------------------------------------------------------ //
// Get y
// ------------------------------------------------------------------------------------ //
float le::LUAQuaternion::GetY() const
{
	return object.y;
}

// ------------------------------------------------------------------------------------ //
// Get z
// ------------------------------------------------------------------------------------ //
float le::LUAQuaternion::GetZ() const
{
	return object.z;
}

// ------------------------------------------------------------------------------------ //
// Get w
// ------------------------------------------------------------------------------------ //
float le::LUAQuaternion::GetW() const
{
	return object.w;
}

// ------------------------------------------------------------------------------------ //
// Operator +
// ------------------------------------------------------------------------------------ //
le::LUAQuaternion le::LUAQuaternion::operator+( const LUAQuaternion& Right )
{
	return LUAQuaternion( object + Right.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Operator -
// ------------------------------------------------------------------------------------ //
le::LUAQuaternion le::LUAQuaternion::operator-( const LUAQuaternion& Right )
{
	return LUAQuaternion( object - Right.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Operator *
// ------------------------------------------------------------------------------------ //
le::LUAQuaternion le::LUAQuaternion::operator*( const LUAQuaternion& Right )
{
	return LUAQuaternion( object * Right.GetHandle() );
}
