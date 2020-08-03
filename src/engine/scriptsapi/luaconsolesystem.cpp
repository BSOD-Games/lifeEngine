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
#include "engine/consolesystem.h"
#include "scriptsapi/luaconsolesystem.h"

// ------------------------------------------------------------------------------------ //
// Execute command
// ------------------------------------------------------------------------------------ //
void le::LUAConsoleSystem::Exec( const std::string& Command )
{
	g_consoleSystem->Exec( Command.c_str() );
}

// ------------------------------------------------------------------------------------ //
// Print info
// ------------------------------------------------------------------------------------ //
void le::LUAConsoleSystem::PrintInfo( const std::string& Message )
{
	g_consoleSystem->PrintInfo( Message.c_str() );
}

// ------------------------------------------------------------------------------------ //
// Print warning
// ------------------------------------------------------------------------------------ //
void le::LUAConsoleSystem::PrintWarning( const std::string& Message )
{
	g_consoleSystem->PrintWarning( Message.c_str() );
}

// ------------------------------------------------------------------------------------ //
// Print error
// ------------------------------------------------------------------------------------ //
void le::LUAConsoleSystem::PrintError( const std::string& Message )
{
	g_consoleSystem->PrintError( Message.c_str() );
}

// ------------------------------------------------------------------------------------ //
// Register console system in LUA
// ------------------------------------------------------------------------------------ //
void le::LUAConsoleSystem::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAConsoleSystem>( "ConsoleSystem" ).
		addStaticFunction( "Exec", &LUAConsoleSystem::Exec ).
		addStaticFunction( "PrintInfo", &LUAConsoleSystem::PrintInfo ).
		addStaticFunction( "PrintWarning", &LUAConsoleSystem::PrintWarning ).
		addStaticFunction( "PrintError", &LUAConsoleSystem::PrintError ).
		endClass();
}
