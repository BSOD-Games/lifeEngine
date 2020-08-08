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
#include "engine/resourcesystem.h"
#include "scriptsapi/common/luaimage.h"
#include "scriptsapi/studiorender/luamesh.h"
#include "scriptsapi/engine/luaresourcesystem.h"

// ------------------------------------------------------------------------------------ //
// Register resource system in LUA
// ------------------------------------------------------------------------------------ //
void le::LUAResourceSystem::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Registern input system
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAResourceSystem>( "ResourceSystem" ).
		addStaticFunction( "LoadImage", &LUAResourceSystem::LoadImage ).
		addStaticFunction( "LoadMesh", &LUAResourceSystem::LoadMesh ).
		addStaticFunction( "UnloadImage", &LUAResourceSystem::UnloadImage ).
		addStaticFunction( "UnloadMesh", &LUAResourceSystem::UnloadMesh ).
		addStaticFunction( "UnloadAll", &LUAResourceSystem::UnloadAll ).
		addStaticFunction( "GetMesh", &LUAResourceSystem::GetMesh ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Load image
// ------------------------------------------------------------------------------------ //
le::LUAImage le::LUAResourceSystem::LoadImage( const char* Path, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels )
{
	bool		isError = false;
	return g_resourceSystem->LoadImage( Path, isError, IsFlipVertical, IsSwitchRedAndBlueChannels );
}

// ------------------------------------------------------------------------------------ //
// Load mesh
// ------------------------------------------------------------------------------------ //
le::LUAMesh le::LUAResourceSystem::LoadMesh( const char* Name, const char* Path )
{
	return LUAMesh( g_resourceSystem->LoadMesh( Name, Path ) );
}

// ------------------------------------------------------------------------------------ //
// Unload image
// ------------------------------------------------------------------------------------ //
void le::LUAResourceSystem::UnloadImage( LUAImage& Image )
{
	g_resourceSystem->UnloadImage( ( le::Image& ) Image.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Unload mesh
// ------------------------------------------------------------------------------------ //
void le::LUAResourceSystem::UnloadMesh( const char* Name )
{
	g_resourceSystem->UnloadMesh( Name );
}

// ------------------------------------------------------------------------------------ //
// Unload meshes
// ------------------------------------------------------------------------------------ //
void le::LUAResourceSystem::UnloadMeshes()
{
	g_resourceSystem->UnloadMeshes();
}

// ------------------------------------------------------------------------------------ //
// Unload all
// ------------------------------------------------------------------------------------ //
void le::LUAResourceSystem::UnloadAll()
{
	g_resourceSystem->UnloadAll();
}

// ------------------------------------------------------------------------------------ //
// Get mesh
// ------------------------------------------------------------------------------------ //
le::LUAMesh le::LUAResourceSystem::GetMesh( const char* Name )
{
	return LUAMesh( g_resourceSystem->GetMesh( Name ) );
}
