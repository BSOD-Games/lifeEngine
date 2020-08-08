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
#include "scriptsapi/common/luacolor.h"

// ------------------------------------------------------------------------------------ //
// Register color
// ------------------------------------------------------------------------------------ //
void le::LUAColor::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Register Vector 4D
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAColor>( "Color" ).
		addConstructor< void (*) () >().
		addProperty( "x", &LUAColor::GetR, &LUAColor::SetR ).
		addProperty( "y", &LUAColor::GetG, &LUAColor::SetG ).
		addProperty( "z", &LUAColor::GetB, &LUAColor::SetB ).
		addProperty( "w", &LUAColor::GetA, &LUAColor::SetA ).
		addFunction( "Set", &LUAColor::Set ).
		addFunction( "__tostring", &LUAColor::ToString ).
		addFunction( "__add", ( LUAColor ( LUAColor::* )( const LUAColor& ) ) &LUAColor::operator+ ).
		addFunction( "__sub", ( LUAColor ( LUAColor::* )( const LUAColor& ) ) &LUAColor::operator- ).
		addFunction( "__mul", ( LUAColor ( LUAColor::* )( const LUAColor& ) ) &LUAColor::operator* ).
		addFunction( "__div", ( LUAColor ( LUAColor::* )( const LUAColor& ) ) &LUAColor::operator/ ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAColor::LUAColor( const Color_t& Copy ) :
	object( Copy.r, Copy.g, Copy.b, Copy.a )
{}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAColor::LUAColor( float R, float G, float B, float A ) :
	object( R, G, B, A )
{}

// ------------------------------------------------------------------------------------ //
// Set x and y
// ------------------------------------------------------------------------------------ //
void le::LUAColor::Set( float R, float G, float B, float A )
{
	object.r = R;
	object.g = G;
	object.b = B;
	object.a = A;
}

// ------------------------------------------------------------------------------------ //
// Convert to string
// ------------------------------------------------------------------------------------ //
std::string le::LUAColor::ToString()
{
	return std::to_string( object.r ) + ", " + std::to_string( object.g ) + ", " + std::to_string( object.b ) + ", " + std::to_string( object.a );
}

// ------------------------------------------------------------------------------------ //
// Set r
// ------------------------------------------------------------------------------------ //
void le::LUAColor::SetR( float R )
{
	object.r = R;
}

// ------------------------------------------------------------------------------------ //
// Set G
// ------------------------------------------------------------------------------------ //
void le::LUAColor::SetG( float G )
{
	object.g = G;
}

// ------------------------------------------------------------------------------------ //
// Set b
// ------------------------------------------------------------------------------------ //
void le::LUAColor::SetB( float B )
{
	object.b = B;
}

// ------------------------------------------------------------------------------------ //
// Set a
// ------------------------------------------------------------------------------------ //
void le::LUAColor::SetA( float A )
{
	object.a = A;
}

// ------------------------------------------------------------------------------------ //
// Get r
// ------------------------------------------------------------------------------------ //
float le::LUAColor::GetR() const
{
	return object.r;
}

// ------------------------------------------------------------------------------------ //
// Get g
// ------------------------------------------------------------------------------------ //
float le::LUAColor::GetG() const
{
	return object.g;
}

// ------------------------------------------------------------------------------------ //
// Get b
// ------------------------------------------------------------------------------------ //
float le::LUAColor::GetB() const
{
	return object.b;
}

// ------------------------------------------------------------------------------------ //
// Get a
// ------------------------------------------------------------------------------------ //
float le::LUAColor::GetA() const
{
	return object.a;
}

// ------------------------------------------------------------------------------------ //
// operator +
// ------------------------------------------------------------------------------------ //
le::LUAColor le::LUAColor::operator+( const LUAColor& Right )
{
	return LUAColor( object + Right.object );
}

// ------------------------------------------------------------------------------------ //
// operator -
// ------------------------------------------------------------------------------------ //
le::LUAColor le::LUAColor::operator-( const LUAColor& Right )
{
	return LUAColor( object - Right.object );
}

// ------------------------------------------------------------------------------------ //
// operator /
// ------------------------------------------------------------------------------------ //
le::LUAColor le::LUAColor::operator/( const LUAColor& Right )
{
	return LUAColor( object / Right.object );
}

// ------------------------------------------------------------------------------------ //
// operator *
// ------------------------------------------------------------------------------------ //
le::LUAColor le::LUAColor::operator*( const LUAColor& Right )
{
	return LUAColor( object * Right.object );
}
