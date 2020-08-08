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
#include "scriptsapi/engine/luaconcmd.h"
#include "engine/iconcmd.h"
// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAConCmd::LUAConCmd( const char* Name, const char* HelpText, luabridge::LuaRef ExecCallback ) :
	name( Name ),
	helpText( HelpText ),
	execCallback( ExecCallback )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::LUAConCmd::~LUAConCmd()
{}

// ------------------------------------------------------------------------------------ //
// Register ConCmd in LUA
// ------------------------------------------------------------------------------------ //
void le::LUAConCmd::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Registern input system
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAConCmd>( "ConCmd" ).
		addConstructor< void (*) ( const char*, const char*, luabridge::LuaRef ) >().
		addFunction( "Exec", &LUAConCmd::Exec ).
		addFunction( "SetExecCallback", &LUAConCmd::SetExecCallback ).
		addFunction( "SetName", &LUAConCmd::SetName ).
		addFunction( "GetName", &LUAConCmd::GetName ).
		addFunction( "GetHelpText", &LUAConCmd::GetHelpText ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Exec command
// ------------------------------------------------------------------------------------ //
void le::LUAConCmd::Exec( luabridge::LuaRef Arguments )
{
	if ( !execCallback.isFunction() )		return;
	execCallback( Arguments );
}

// ------------------------------------------------------------------------------------ //
// Set execute callback
// ------------------------------------------------------------------------------------ //
void le::LUAConCmd::SetExecCallback( luabridge::LuaRef ExecCallback )
{
	execCallback = ExecCallback;
}

// ------------------------------------------------------------------------------------ //
// Set name command
// ------------------------------------------------------------------------------------ //
void le::LUAConCmd::SetName( const char* Name )
{
	name = Name;
}

// ------------------------------------------------------------------------------------ //
// Get name command
// ------------------------------------------------------------------------------------ //
const char* le::LUAConCmd::GetName() const
{
	return name.c_str();
}

// ------------------------------------------------------------------------------------ //
// Get help text
// ------------------------------------------------------------------------------------ //
const char* le::LUAConCmd::GetHelpText() const
{
	return helpText.c_str();
}
