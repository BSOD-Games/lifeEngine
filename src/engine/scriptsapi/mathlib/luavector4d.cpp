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
#include "engine/inputsystem.h"
#include "scriptsapi/mathlib/luavector4d.h"

// ------------------------------------------------------------------------------------ //
// Register vector 4d
// ------------------------------------------------------------------------------------ //
void le::LUAVector4D::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Register Vector 4D
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAVector4D>( "Vector4D" ).
		addConstructor< void (*) () >().
		addProperty( "x", &LUAVector4D::GetX, &LUAVector4D::SetX ).
		addProperty( "y", &LUAVector4D::GetY, &LUAVector4D::SetY ).
		addProperty( "z", &LUAVector4D::GetZ, &LUAVector4D::SetZ ).
		addProperty( "w", &LUAVector4D::GetW, &LUAVector4D::SetW ).
		addFunction( "Normalize", &LUAVector4D::Normalize ).
		addFunction( "Dot", &LUAVector4D::Dot ).
		addFunction( "Set", &LUAVector4D::Set ).
		addFunction( "__tostring", &LUAVector4D::ToString ).
		addFunction( "__add", ( LUAVector4D ( LUAVector4D::* )( const LUAVector4D& ) ) &LUAVector4D::operator+ ).
		addFunction( "__sub", ( LUAVector4D ( LUAVector4D::* )( const LUAVector4D& ) ) &LUAVector4D::operator- ).
		addFunction( "__mul", ( LUAVector4D ( LUAVector4D::* )( const LUAVector4D& ) ) &LUAVector4D::operator* ).
		addFunction( "__div", ( LUAVector4D ( LUAVector4D::* )( const LUAVector4D& ) ) &LUAVector4D::operator/ ).
		addStaticFunction( "Normalize", ( LUAVector4D (*) ( const LUAVector4D& ) ) &LUAVector4D::Normalize ).
		addStaticFunction( "Dot", ( float (*) ( const LUAVector4D&, const LUAVector4D& ) ) &LUAVector4D::Dot ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAVector4D::LUAVector4D( Vector4D_t& Copy ) :
	object( Copy.x, Copy.y, Copy.z, Copy.w )
{}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAVector4D::LUAVector4D( float X, float Y, float Z, float W ) :
	object( X, Y, Z, W )
{}

// ------------------------------------------------------------------------------------ //
// Normalize vector
// ------------------------------------------------------------------------------------ //
le::LUAVector4D le::LUAVector4D::Normalize( const LUAVector4D& Vector )
{
	return LUAVector4D( glm::normalize( Vector.object ) );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAVector4D::Dot( const LUAVector4D& Left, const LUAVector4D& Right )
{
	return glm::dot( Left.object, Right.object );
}

// ------------------------------------------------------------------------------------ //
// Normalize vector
// ------------------------------------------------------------------------------------ //
void le::LUAVector4D::Normalize()
{
	object = glm::normalize( object );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAVector4D::Dot( const LUAVector4D& Right )
{
	return glm::dot( object, Right.object );
}

// ------------------------------------------------------------------------------------ //
// Set x and y
// ------------------------------------------------------------------------------------ //
void le::LUAVector4D::Set( float X, float Y, float Z, float W )
{
	object.x = X;
	object.y = Y;
	object.z = Z;
	object.w = W;
}

// ------------------------------------------------------------------------------------ //
// Convert to string
// ------------------------------------------------------------------------------------ //
std::string le::LUAVector4D::ToString()
{
	return std::to_string( object.x ) + ", " + std::to_string( object.y ) + ", " + std::to_string( object.z ) + ", " + std::to_string( object.w );
}

// ------------------------------------------------------------------------------------ //
// Set x
// ------------------------------------------------------------------------------------ //
void le::LUAVector4D::SetX( float X )
{
	object.x = X;
}

// ------------------------------------------------------------------------------------ //
// Set y
// ------------------------------------------------------------------------------------ //
void le::LUAVector4D::SetY( float Y )
{
	object.y = Y;
}

// ------------------------------------------------------------------------------------ //
// Set x
// ------------------------------------------------------------------------------------ //
void le::LUAVector4D::SetZ( float Z )
{
	object.z = Z;
}

// ------------------------------------------------------------------------------------ //
// Set x
// ------------------------------------------------------------------------------------ //
void le::LUAVector4D::SetW( float W )
{
	object.w = W;
}

// ------------------------------------------------------------------------------------ //
// Get x
// ------------------------------------------------------------------------------------ //
float le::LUAVector4D::GetX() const
{
	return object.x;
}

// ------------------------------------------------------------------------------------ //
// Get y
// ------------------------------------------------------------------------------------ //
float le::LUAVector4D::GetY() const
{
	return object.y;
}

// ------------------------------------------------------------------------------------ //
// Get z
// ------------------------------------------------------------------------------------ //
float le::LUAVector4D::GetZ() const
{
	return object.z;
}

// ------------------------------------------------------------------------------------ //
// Get w
// ------------------------------------------------------------------------------------ //
float le::LUAVector4D::GetW() const
{
	return object.w;
}

// ------------------------------------------------------------------------------------ //
// operator +
// ------------------------------------------------------------------------------------ //
le::LUAVector4D le::LUAVector4D::operator+( const LUAVector4D& Right )
{
	return LUAVector4D( object + Right.object );
}

// ------------------------------------------------------------------------------------ //
// operator -
// ------------------------------------------------------------------------------------ //
le::LUAVector4D le::LUAVector4D::operator-( const LUAVector4D& Right )
{
	return LUAVector4D( object - Right.object );
}

// ------------------------------------------------------------------------------------ //
// operator /
// ------------------------------------------------------------------------------------ //
le::LUAVector4D le::LUAVector4D::operator/( const LUAVector4D& Right )
{
	return LUAVector4D( object / Right.object );
}

// ------------------------------------------------------------------------------------ //
// operator *
// ------------------------------------------------------------------------------------ //
le::LUAVector4D le::LUAVector4D::operator*( const LUAVector4D& Right )
{
	return LUAVector4D( object * Right.object );
}
