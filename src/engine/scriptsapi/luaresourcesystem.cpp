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
#include "common/image.h"
#include "engine/lifeengine.h"
#include "engine/resourcesystem.h"
#include "scriptsapi/luaresourcesystem.h"

// ------------------------------------------------------------------------------------ //
// Register resource system
// ------------------------------------------------------------------------------------ //
void le::LUAResourceSystem::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;
	
	LIFEENGINE_ASSERT( g_resourceSystem );

	// Register window
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAResourceSystem>( "ResourceSystem" ).
		addStaticFunction( "LoadImage", &LUAResourceSystem::LoadImage ).
		addStaticFunction( "UnloadImage", &LUAResourceSystem::UnloadImage ).
		addStaticFunction( "UnloadAll", &LUAResourceSystem::UnloadAll ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Load image
// ------------------------------------------------------------------------------------ //
le::Image le::LUAResourceSystem::LoadImage( const char* Path, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels )
{
	if ( !g_resourceSystem )		return Image();
	
	bool							isError = false;
	return g_resourceSystem->LoadImage( Path, isError, IsFlipVertical, IsSwitchRedAndBlueChannels );
}

// ------------------------------------------------------------------------------------ //
// Unload image
// ------------------------------------------------------------------------------------ //
void le::LUAResourceSystem::UnloadImage( Image& Image )
{
	if ( !g_resourceSystem ) return;
	g_resourceSystem->UnloadImage( Image );
}

// ------------------------------------------------------------------------------------ //
// Unload all
// ------------------------------------------------------------------------------------ //
void le::LUAResourceSystem::UnloadAll()
{
	if ( !g_resourceSystem ) return;
	g_resourceSystem->UnloadAll();
}
