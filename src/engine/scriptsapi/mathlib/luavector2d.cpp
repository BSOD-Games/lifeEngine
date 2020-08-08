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
#include "scriptsapi/mathlib/luavector2d.h"

// ------------------------------------------------------------------------------------ //
// Register vector 2d
// ------------------------------------------------------------------------------------ //
void le::LUAVector2D::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Register Vector 2D
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAVector2D>( "Vector2D" ).
		addConstructor< void (*) () >().
		addProperty( "x", &LUAVector2D::GetX, &LUAVector2D::SetX ).
		addProperty( "y", &LUAVector2D::GetY, &LUAVector2D::SetY ).
		addFunction( "Normalize", &LUAVector2D::Normalize ).
		addFunction( "Dot", &LUAVector2D::Dot ).
		addFunction( "Set", &LUAVector2D::Set ).
		addFunction( "__tostring", &LUAVector2D::ToString ).
		addFunction( "__add", ( LUAVector2D ( LUAVector2D::* )( const LUAVector2D& ) ) &LUAVector2D::operator+ ).
		addFunction( "__sub", ( LUAVector2D ( LUAVector2D::* )( const LUAVector2D& ) ) &LUAVector2D::operator- ).
		addFunction( "__mul", ( LUAVector2D ( LUAVector2D::* )( const LUAVector2D& ) ) &LUAVector2D::operator* ).
		addFunction( "__div", ( LUAVector2D ( LUAVector2D::* )( const LUAVector2D& ) ) &LUAVector2D::operator/ ).
		addStaticFunction( "Normalize", ( LUAVector2D (*) ( const LUAVector2D& ) ) &LUAVector2D::Normalize ).
		addStaticFunction( "Dot", ( float (*) ( const LUAVector2D&, const LUAVector2D& ) ) &LUAVector2D::Dot ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAVector2D::LUAVector2D( Vector2D_t& Copy ) :
	object( Copy.x, Copy.y )
{}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAVector2D::LUAVector2D( float X, float Y ) :
	object( X, Y )
{}

// ------------------------------------------------------------------------------------ //
// Normalize vector
// ------------------------------------------------------------------------------------ //
le::LUAVector2D le::LUAVector2D::Normalize( const LUAVector2D& Vector )
{
	return LUAVector2D( glm::normalize( Vector.object ) );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAVector2D::Dot( const LUAVector2D& Left, const LUAVector2D& Right )
{
	return glm::dot( Left.object, Right.object );
}

// ------------------------------------------------------------------------------------ //
// Normalize vector
// ------------------------------------------------------------------------------------ //
void le::LUAVector2D::Normalize()
{
	object = glm::normalize( object );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAVector2D::Dot( const LUAVector2D& Right )
{
	return glm::dot( object, Right.object );
}

// ------------------------------------------------------------------------------------ //
// Set x and y
// ------------------------------------------------------------------------------------ //
void le::LUAVector2D::Set( float X, float Y )
{
	object.x = X;
	object.y = Y;
}

// ------------------------------------------------------------------------------------ //
// Convert to string
// ------------------------------------------------------------------------------------ //
std::string le::LUAVector2D::ToString()
{
	return std::to_string( object.x ) + ", " + std::to_string( object.y );
}

// ------------------------------------------------------------------------------------ //
// Set x
// ------------------------------------------------------------------------------------ //
void le::LUAVector2D::SetX( float X )
{
	object.x = X;
}

// ------------------------------------------------------------------------------------ //
// Set y
// ------------------------------------------------------------------------------------ //
void le::LUAVector2D::SetY( float Y )
{
	object.y = Y;
}

// ------------------------------------------------------------------------------------ //
// Get x
// ------------------------------------------------------------------------------------ //
float le::LUAVector2D::GetX() const
{
	return object.x;
}

// ------------------------------------------------------------------------------------ //
// Get y
// ------------------------------------------------------------------------------------ //
float le::LUAVector2D::GetY() const
{
	return object.y;
}

// ------------------------------------------------------------------------------------ //
// operator +
// ------------------------------------------------------------------------------------ //
le::LUAVector2D le::LUAVector2D::operator+( const LUAVector2D& Right )
{
	return LUAVector2D( object + Right.object );
}

// ------------------------------------------------------------------------------------ //
// operator -
// ------------------------------------------------------------------------------------ //
le::LUAVector2D le::LUAVector2D::operator-( const LUAVector2D& Right )
{
	return LUAVector2D( object - Right.object );
}

// ------------------------------------------------------------------------------------ //
// operator /
// ------------------------------------------------------------------------------------ //
le::LUAVector2D le::LUAVector2D::operator/( const LUAVector2D& Right )
{
	return LUAVector2D( object / Right.object );
}

// ------------------------------------------------------------------------------------ //
// operator *
// ------------------------------------------------------------------------------------ //
le::LUAVector2D le::LUAVector2D::operator*( const LUAVector2D& Right )
{
	return LUAVector2D( object * Right.object );
}
