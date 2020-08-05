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
#include "scriptsapi/luavector4d.h"

// ------------------------------------------------------------------------------------ //
// Register vector 3d
// ------------------------------------------------------------------------------------ //
void le::LUAVector4D::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Register Vector 4D
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<Vector4D_t>( "Vector4D" ).
		addConstructor< void (*) ( const float&, const float&, const float&, const float& ) >().
		addData( "x", &Vector4D_t::x ).
		addData( "y", &Vector4D_t::y ).
		addData( "z", &Vector4D_t::z ).
		addData( "w", &Vector4D_t::w ).
		addFunction( "Set", &LUAVector4D::Set ).
		addFunction( "Normalize", &LUAVector4D::Normalize ).
		addFunction( "Dot", ( float (*) ( Vector4D_t*, const Vector4D_t& ) ) &LUAVector4D::Dot ).
		addFunction( "__add", ( Vector4D_t& ( Vector4D_t::* )( const Vector4D_t& ) ) &Vector4D_t::operator+= ).
		addFunction( "__sub", ( Vector4D_t& ( Vector4D_t::* )( const Vector4D_t& ) ) &Vector4D_t::operator-= ).
		addFunction( "__mul", ( Vector4D_t& ( Vector4D_t::* )( const Vector4D_t& ) ) &Vector4D_t::operator*= ).
		addFunction( "__div", ( Vector4D_t& ( Vector4D_t::* )( const Vector4D_t& ) ) &Vector4D_t::operator/= ).
		addFunction( "__tostring", &LUAVector4D::ToString ).
		addStaticFunction( "Dot", ( float (*) ( const Vector4D_t&, const Vector4D_t& ) ) &LUAVector4D::Dot ).
		addStaticFunction( "Normalize", ( Vector4D_t(*) ( const Vector4D_t& ) ) &LUAVector4D::Normalize ).
		endClass();

	// Register VectorInt 4D
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<Vector4DInt_t>( "Vector4DInt" ).
		addConstructor< void (*) ( const int&, const int&, const int&, const int& ) >().
		addData( "x", &Vector4DInt_t::x ).
		addData( "y", &Vector4DInt_t::y ).
		addData( "z", &Vector4DInt_t::z ).
		addData( "w", &Vector4DInt_t::w ).
		addFunction( "Set", &LUAVector4D::Set ).
		addFunction( "__add", ( Vector4DInt_t& ( Vector4DInt_t::* )( const Vector4DInt_t& ) ) &Vector4DInt_t::operator+= ).
		addFunction( "__sub", ( Vector4DInt_t& ( Vector4DInt_t::* )( const Vector4DInt_t& ) ) &Vector4DInt_t::operator-= ).
		addFunction( "__mul", ( Vector4DInt_t& ( Vector4DInt_t::* )( const Vector4DInt_t& ) ) &Vector4DInt_t::operator*= ).
		addFunction( "__div", ( Vector4DInt_t& ( Vector4DInt_t::* )( const Vector4DInt_t& ) ) &Vector4DInt_t::operator/= ).
		addFunction( "__tostring", ( std::string (*) ( Vector4DInt_t* ) ) &LUAVector4D::ToString ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Normalize
// ------------------------------------------------------------------------------------ //
le::Vector4D_t le::LUAVector4D::Normalize( const Vector4D_t& Vector )
{
	return glm::normalize( Vector );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAVector4D::Dot( const Vector4D_t& Left, const Vector4D_t& Right )
{
	return glm::dot( Left, Right );
}

// ------------------------------------------------------------------------------------ //
// Normalize
// ------------------------------------------------------------------------------------ //
void le::LUAVector4D::Normalize( Vector4D_t* Object )
{
	if ( !Object ) return;
	*Object = glm::normalize( *Object );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAVector4D::Dot( Vector4D_t* Object, const Vector4D_t& Right )
{
	if ( !Object )		return 0.f;
	return glm::dot( *Object, Right );
}

// ------------------------------------------------------------------------------------ //
// Set values
// ------------------------------------------------------------------------------------ //
void le::LUAVector4D::Set( Vector4D_t* Object, float X, float Y, float Z, float W )
{
	if ( !Object ) return;
	Object->x = X;
	Object->y = Y;
	Object->z = Z;
	Object->w = W;
}

// ------------------------------------------------------------------------------------ //
// Set values
// ------------------------------------------------------------------------------------ //
void le::LUAVector4D::Set( Vector4DInt_t* Object, int X, int Y, int Z, int W )
{
	if ( !Object ) return;
	Object->x = X;
	Object->y = Y;
	Object->z = Z;
	Object->w = W;
}

// ------------------------------------------------------------------------------------ //
// Convert vector to string
// ------------------------------------------------------------------------------------ //
std::string le::LUAVector4D::ToString( Vector4D_t* Object )
{
	if ( !Object )		return "nil";
	return std::to_string( Object->x ) + ", " + std::to_string( Object->y ) + ", " + std::to_string( Object->z ) + ", " + std::to_string( Object->w );
}

// ------------------------------------------------------------------------------------ //
// Convert vector to string
// ------------------------------------------------------------------------------------ //
std::string le::LUAVector4D::ToString( Vector4DInt_t* Object )
{
	if ( !Object )		return "nil";
	return std::to_string( Object->x ) + ", " + std::to_string( Object->y ) + ", " + std::to_string( Object->z ) + ", " + std::to_string( Object->w );
}
