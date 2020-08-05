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
#include "scriptsapi/luavector3d.h"

// ------------------------------------------------------------------------------------ //
// Register vector 3d
// ------------------------------------------------------------------------------------ //
void le::LUAVector3D::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Register Vector 3D
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<Vector3D_t>( "Vector3D" ).
		addConstructor< void (*) ( const float&, const float&, const float& ) >().
		addData( "x", &Vector3D_t::x ).
		addData( "y", &Vector3D_t::y ).
		addData( "z", &Vector3D_t::z ).
		addFunction( "Set", &LUAVector3D::Set ).
		addFunction( "Normalize", &LUAVector3D::Normalize ).
		addFunction( "Dot", ( float (*) ( Vector3D_t*, const Vector3D_t& ) ) &LUAVector3D::Dot ).
		addFunction( "Cross", &LUAVector3D::Cross ).
		addFunction( "__add", ( Vector3D_t& ( Vector3D_t::* )( const Vector3D_t& ) ) &Vector3D_t::operator+= ).
		addFunction( "__sub", ( Vector3D_t& ( Vector3D_t::* )( const Vector3D_t& ) ) &Vector3D_t::operator-= ).
		addFunction( "__mul", ( Vector3D_t& ( Vector3D_t::* )( const Vector3D_t& ) ) &Vector3D_t::operator*= ).
		addFunction( "__div", ( Vector3D_t& ( Vector3D_t::* )( const Vector3D_t& ) ) &Vector3D_t::operator/= ).
		addFunction( "__tostring", &LUAVector3D::ToString ).
		addStaticFunction( "Dot", ( float (*) ( const Vector3D_t&, const Vector3D_t& ) ) &LUAVector3D::Dot ).
		addStaticFunction( "Normalize", ( Vector3D_t (*) ( const Vector3D_t& ) ) &LUAVector3D::Normalize ).
		addStaticFunction( "Cross", ( Vector3D_t (*) ( const Vector3D_t&, const Vector3D_t& ) ) &LUAVector3D::Cross ).
		endClass();

	// Register VectorInt 3D
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<Vector3DInt_t>( "Vector3DInt" ).
		addConstructor< void (*) ( const int&, const int&, const int& ) >().
		addData( "x", &Vector3DInt_t::x ).
		addData( "y", &Vector3DInt_t::y ).
		addData( "z", &Vector3DInt_t::z ).
		addFunction( "Set", &LUAVector3D::Set ).
		addFunction( "__add", ( Vector3DInt_t& ( Vector3DInt_t::* )( const Vector3DInt_t& ) ) &Vector3DInt_t::operator+= ).
		addFunction( "__sub", ( Vector3DInt_t& ( Vector3DInt_t::* )( const Vector3DInt_t& ) ) &Vector3DInt_t::operator-= ).
		addFunction( "__mul", ( Vector3DInt_t& ( Vector3DInt_t::* )( const Vector3DInt_t& ) ) &Vector3DInt_t::operator*= ).
		addFunction( "__div", ( Vector3DInt_t& ( Vector3DInt_t::* )( const Vector3DInt_t& ) ) &Vector3DInt_t::operator/= ).
		addFunction( "__tostring", ( std::string (*) ( Vector3DInt_t* ) ) &LUAVector3D::ToString ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Normalize
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::LUAVector3D::Normalize( const Vector3D_t& Vector )
{
	return glm::normalize( Vector );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAVector3D::Dot( const Vector3D_t& Left, const Vector3D_t& Right )
{
	return glm::dot( Left, Right );
}

// ------------------------------------------------------------------------------------ //
// Cross
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::LUAVector3D::Cross( const Vector3D_t& Left, const Vector3D_t& Right )
{
	return glm::cross( Left, Right );
}

// ------------------------------------------------------------------------------------ //
// Normalize
// ------------------------------------------------------------------------------------ //
void le::LUAVector3D::Normalize( Vector3D_t* Object )
{
	if ( !Object ) return;
	*Object = glm::normalize( *Object );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAVector3D::Dot( Vector3D_t* Object, const Vector3D_t& Right )
{
	if ( !Object )		return 0.f;
	return glm::dot( *Object, Right );
}

// ------------------------------------------------------------------------------------ //
// Cross
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::LUAVector3D::Cross( Vector3D_t* Object, const Vector3D_t& Right )
{
	if ( !Object )	return Vector3D_t( 0.f, 0.f, 0.f );
	return glm::cross( *Object, Right );
}

// ------------------------------------------------------------------------------------ //
// Set values
// ------------------------------------------------------------------------------------ //
void le::LUAVector3D::Set( Vector3D_t* Object, float X, float Y, float Z )
{
	if ( !Object ) return;
	Object->x = X;
	Object->y = Y;
	Object->z = Z;
}

// ------------------------------------------------------------------------------------ //
// Set values
// ------------------------------------------------------------------------------------ //
void le::LUAVector3D::Set( Vector3DInt_t* Object, int X, int Y, int Z )
{
	if ( !Object ) return;
	Object->x = X;
	Object->y = Y;
	Object->z = Z;
}

// ------------------------------------------------------------------------------------ //
// Convert vector to string
// ------------------------------------------------------------------------------------ //
std::string le::LUAVector3D::ToString( Vector3D_t* Object )
{
	if ( !Object )		return "nil";
	return std::to_string( Object->x ) + ", " + std::to_string( Object->y ) + ", " + std::to_string( Object->z );
}

// ------------------------------------------------------------------------------------ //
// Convert vector to string
// ------------------------------------------------------------------------------------ //
std::string le::LUAVector3D::ToString( Vector3DInt_t* Object )
{
	if ( !Object )		return "nil";
	return std::to_string( Object->x ) + ", " + std::to_string( Object->y ) + ", " + std::to_string( Object->z );
}
