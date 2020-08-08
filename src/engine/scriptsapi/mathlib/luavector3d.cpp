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
#include "scriptsapi/mathlib/luavector3d.h"

// ------------------------------------------------------------------------------------ //
// Register vector 3d
// ------------------------------------------------------------------------------------ //
void le::LUAVector3D::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Register Vector 2D
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAVector3D>( "Vector3D" ).
		addConstructor< void (*) () >().
		addProperty( "x", &LUAVector3D::GetX, &LUAVector3D::SetX ).
		addProperty( "y", &LUAVector3D::GetY, &LUAVector3D::SetY ).
		addProperty( "z", &LUAVector3D::GetZ, &LUAVector3D::SetZ ).
		addFunction( "Normalize", &LUAVector3D::Normalize ).
		addFunction( "Dot", &LUAVector3D::Dot ).
		addFunction( "Cross", &LUAVector3D::Cross ).
		addFunction( "Set", &LUAVector3D::Set ).
		addFunction( "__tostring", &LUAVector3D::ToString ).
		addFunction( "__add", ( LUAVector3D ( LUAVector3D::* )( const LUAVector3D& ) ) &LUAVector3D::operator+ ).
		addFunction( "__sub", ( LUAVector3D ( LUAVector3D::* )( const LUAVector3D& ) ) &LUAVector3D::operator- ).
		addFunction( "__mul", ( LUAVector3D ( LUAVector3D::* )( const LUAVector3D& ) ) &LUAVector3D::operator* ).
		addFunction( "__div", ( LUAVector3D ( LUAVector3D::* )( const LUAVector3D& ) ) &LUAVector3D::operator/ ).
		addStaticFunction( "Normalize", ( LUAVector3D (*) ( const LUAVector3D& ) ) &LUAVector3D::Normalize ).
		addStaticFunction( "Dot", ( float (*) ( const LUAVector3D&, const LUAVector3D& ) ) &LUAVector3D::Dot ).
		addStaticFunction( "Cross", ( LUAVector3D (*) ( const LUAVector3D&, const LUAVector3D& ) ) &LUAVector3D::Cross ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAVector3D::LUAVector3D( Vector3D_t& Copy ) :
	object( Copy.x, Copy.y, Copy.z )
{}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAVector3D::LUAVector3D( float X, float Y, float Z ) :
	object( X, Y, Z )
{}

// ------------------------------------------------------------------------------------ //
// Normalize vector
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUAVector3D::Normalize( const LUAVector3D& Vector )
{
	return LUAVector3D( glm::normalize( Vector.object ) );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAVector3D::Dot( const LUAVector3D& Left, const LUAVector3D& Right )
{
	return glm::dot( Left.object, Right.object );
}

// ------------------------------------------------------------------------------------ //
// Cross
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUAVector3D::Cross( const LUAVector3D& Left, const LUAVector3D& Right )
{
	return glm::cross( Left.object, Right.object );
}

// ------------------------------------------------------------------------------------ //
// Normalize vector
// ------------------------------------------------------------------------------------ //
void le::LUAVector3D::Normalize()
{
	object = glm::normalize( object );
}

// ------------------------------------------------------------------------------------ //
// Dot
// ------------------------------------------------------------------------------------ //
float le::LUAVector3D::Dot( const LUAVector3D& Right )
{
	return glm::dot( object, Right.object );
}

// ------------------------------------------------------------------------------------ //
// Cross
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUAVector3D::Cross( const LUAVector3D& Right )
{
	return glm::cross( object, Right.object );
}

// ------------------------------------------------------------------------------------ //
// Set x and y
// ------------------------------------------------------------------------------------ //
void le::LUAVector3D::Set( float X, float Y, float Z )
{
	object.x = X;
	object.y = Y;
	object.z = Z;
}

// ------------------------------------------------------------------------------------ //
// Convert to string
// ------------------------------------------------------------------------------------ //
std::string le::LUAVector3D::ToString()
{
	return std::to_string( object.x ) + ", " + std::to_string( object.y ) + ", " + std::to_string( object.z );
}

// ------------------------------------------------------------------------------------ //
// Set x
// ------------------------------------------------------------------------------------ //
void le::LUAVector3D::SetX( float X )
{
	object.x = X;
}

// ------------------------------------------------------------------------------------ //
// Set y
// ------------------------------------------------------------------------------------ //
void le::LUAVector3D::SetY( float Y )
{
	object.y = Y;
}

// ------------------------------------------------------------------------------------ //
// Set x
// ------------------------------------------------------------------------------------ //
void le::LUAVector3D::SetZ( float Z )
{
	object.z = Z;
}

// ------------------------------------------------------------------------------------ //
// Get x
// ------------------------------------------------------------------------------------ //
float le::LUAVector3D::GetX() const
{
	return object.x;
}

// ------------------------------------------------------------------------------------ //
// Get y
// ------------------------------------------------------------------------------------ //
float le::LUAVector3D::GetY() const
{
	return object.y;
}

// ------------------------------------------------------------------------------------ //
// Get z
// ------------------------------------------------------------------------------------ //
float le::LUAVector3D::GetZ() const
{
	return object.z;
}

// ------------------------------------------------------------------------------------ //
// operator +
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUAVector3D::operator+( const LUAVector3D& Right )
{
	return LUAVector3D( object + Right.object );
}

// ------------------------------------------------------------------------------------ //
// operator -
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUAVector3D::operator-( const LUAVector3D& Right )
{
	return LUAVector3D( object - Right.object );
}

// ------------------------------------------------------------------------------------ //
// operator /
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUAVector3D::operator/( const LUAVector3D& Right )
{
	return LUAVector3D( object / Right.object );
}

// ------------------------------------------------------------------------------------ //
// operator *
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUAVector3D::operator*( const LUAVector3D& Right )
{
	return LUAVector3D( object * Right.object );
}
