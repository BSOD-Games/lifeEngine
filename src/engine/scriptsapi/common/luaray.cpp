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
#include "common/ray.h"
#include "scriptsapi/common/luaray.h"

// ------------------------------------------------------------------------------------ //
// Register ray in LUA
// ------------------------------------------------------------------------------------ //
void le::LUARay::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Registern input system
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUARay>( "Ray" ).
		addConstructor< void (*) ( const LUAVector3D&, const LUAVector3D& ) >().
		addFunction( "Set", &LUARay::Set ).
		addData( "origin", &LUARay::origin ).
		addData( "direction", &LUARay::direction ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUARay::LUARay( Ray& Ray ) :
	origin( Ray.origin ),
	direction( Ray.direction )
{}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUARay::LUARay( const LUAVector3D& Origin, const LUAVector3D& Direction ) :
	origin( Origin ),
	direction( Direction )
{}

// ------------------------------------------------------------------------------------ //
// Set ray
// ------------------------------------------------------------------------------------ //
void le::LUARay::Set( const LUAVector3D& Origin, const LUAVector3D& Direction )
{
	origin = Origin;
	direction = Direction;
}
