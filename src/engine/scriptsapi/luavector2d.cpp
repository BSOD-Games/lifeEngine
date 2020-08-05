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
#include "scriptsapi/luavector2d.h"

// ------------------------------------------------------------------------------------ //
// Set values
// ------------------------------------------------------------------------------------ //
void le::LUAVector2D::Set( Vector2D_t* Object, float X, float Y )
{
	if ( !Object ) return;
	Object->x = X;
	Object->y = Y;
}

// ------------------------------------------------------------------------------------ //
// Set values
// ------------------------------------------------------------------------------------ //
void le::LUAVector2D::Set( Vector2DInt_t* Object, int X, int Y )
{
	if ( !Object ) return;
	Object->x = X;
	Object->y = Y;
}

// ------------------------------------------------------------------------------------ //
// To string
// ------------------------------------------------------------------------------------ //
std::string le::LUAVector2D::ToString( Vector2D_t* Object )
{
	if ( !Object )		return "nil";
	return std::to_string( Object->x ) + ", " + std::to_string( Object->y );
}

// ------------------------------------------------------------------------------------ //
// To string
// ------------------------------------------------------------------------------------ //
std::string le::LUAVector2D::ToString( Vector2DInt_t* Object )
{
	if ( !Object )		return "nil";
	return std::to_string( Object->x ) + ", " + std::to_string( Object->y );
}

// ------------------------------------------------------------------------------------ //
// Normalize
// ------------------------------------------------------------------------------------ //
void le::LUAVector2D::Normalize( Vector2D_t* Object )
{
	if ( !Object ) return;
	*Object = glm::normalize( *Object );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAVector2D::Dot( Vector2D_t* Object, const Vector2D_t& Right )
{
	if ( !Object ) return 0.f;
	return glm::dot( *Object, Right );
}

// ------------------------------------------------------------------------------------ //
// Normalize
// ------------------------------------------------------------------------------------ //
le::Vector2D_t le::LUAVector2D::Normalize( const Vector2D_t& Vector )
{
	return glm::normalize( Vector );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAVector2D::Dot( const Vector2D_t& Left, const Vector2D_t& Right )
{
	return glm::dot( Left, Right );
}

// ------------------------------------------------------------------------------------ //
// Register vector 2d
// ------------------------------------------------------------------------------------ //
void le::LUAVector2D::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Register Vector 2D
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<Vector2D_t>( "Vector2D" ).
		addConstructor< void (*) ( const float&, const float& ) >().
		addData( "x", &Vector2D_t::x ).
		addData( "y", &Vector2D_t::y ).
		addFunction( "Set", &LUAVector2D::Set ).
		addFunction( "Normalize", &LUAVector2D::Normalize ).
		addFunction( "Dot", ( float (*) ( Vector2D_t*, const Vector2D_t& ) ) &LUAVector2D::Dot ).
		addFunction( "__add", ( Vector2D_t& ( Vector2D_t::* )( const Vector2D_t& ) ) &Vector2D_t::operator+= ).
		addFunction( "__sub", ( Vector2D_t& ( Vector2D_t::* )( const Vector2D_t& ) ) &Vector2D_t::operator-= ).
		addFunction( "__mul", ( Vector2D_t& ( Vector2D_t::* )( const Vector2D_t& ) ) &Vector2D_t::operator*= ).
		addFunction( "__div", ( Vector2D_t& ( Vector2D_t::* )( const Vector2D_t& ) ) &Vector2D_t::operator/= ).
		addFunction( "__tostring", &LUAVector2D::ToString ).
		addStaticFunction( "Dot", ( float (*) ( const Vector2D_t&, const Vector2D_t& ) ) &LUAVector2D::Dot ).
		addStaticFunction( "Normalize", ( Vector2D_t (*) ( const Vector2D_t& ) ) &LUAVector2D::Normalize ).
		endClass();

	// Register VectorInt 2D
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<Vector2DInt_t>( "Vector2DInt" ).
		addConstructor< void (*) ( const int&, const int& ) >().
		addData( "x", &Vector2DInt_t::x ).
		addData( "y", &Vector2DInt_t::y ).
		addFunction( "Set", &LUAVector2D::Set ).
		addFunction( "__add", ( Vector2DInt_t& ( Vector2DInt_t::* )( const Vector2DInt_t& ) ) &Vector2DInt_t::operator+= ).
		addFunction( "__sub", ( Vector2DInt_t& ( Vector2DInt_t::* )( const Vector2DInt_t& ) ) &Vector2DInt_t::operator-= ).
		addFunction( "__mul", ( Vector2DInt_t& ( Vector2DInt_t::* )( const Vector2DInt_t& ) ) &Vector2DInt_t::operator*= ).
		addFunction( "__div", ( Vector2DInt_t& ( Vector2DInt_t::* )( const Vector2DInt_t& ) ) &Vector2DInt_t::operator/= ).
		addFunction( "__tostring", ( std::string (*) ( Vector2DInt_t* ) ) &LUAVector2D::ToString ).
		endClass();
}
