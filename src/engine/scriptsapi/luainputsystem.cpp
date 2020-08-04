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
#include "engine/inputsystem.h"
#include "scriptsapi/luainputsystem.h"

// ------------------------------------------------------------------------------------ //
// Is key down
// ------------------------------------------------------------------------------------ //
bool le::LUAInputSystem::IsKeyDown( UInt32_t Key )
{
    return g_inputSystem->IsKeyDown( ( BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is key up
// ------------------------------------------------------------------------------------ //
bool le::LUAInputSystem::IsKeyUp( UInt32_t Key )
{
	return g_inputSystem->IsKeyUp( ( BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is mouse key down
// ------------------------------------------------------------------------------------ //
bool le::LUAInputSystem::IsMouseKeyDown( UInt32_t Key )
{
	return g_inputSystem->IsMouseKeyDown( ( BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is mouse key up
// ------------------------------------------------------------------------------------ //
bool le::LUAInputSystem::IsMouseKeyUp( UInt32_t Key )
{
	return g_inputSystem->IsMouseKeyUp( ( BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is mouse wheel
// ------------------------------------------------------------------------------------ //
bool le::LUAInputSystem::IsMouseWheel( UInt32_t Wheel )
{
	return g_inputSystem->IsMouseWheel( ( BUTTON_CODE ) Wheel );
}

// ------------------------------------------------------------------------------------ //
// Get mouse sensitivity
// ------------------------------------------------------------------------------------ //
float le::LUAInputSystem::GetMouseSensitivity()
{
	return g_inputSystem->GetMouseSensitivity();
}

// ------------------------------------------------------------------------------------ //
// Register input system
// ------------------------------------------------------------------------------------ //
void le::LUAInputSystem::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAInputSystem>( "InputSystem" ).
		addStaticFunction( "IsKeyDown", &LUAInputSystem::IsKeyDown ).
		addStaticFunction( "IsKeyUp", &LUAInputSystem::IsKeyUp ).
		addStaticFunction( "IsMouseKeyDown", &LUAInputSystem::IsMouseKeyDown ).
		addStaticFunction( "IsMouseKeyUp", &LUAInputSystem::IsMouseKeyUp ).
		addStaticFunction( "IsMouseWheel", &LUAInputSystem::IsMouseWheel ).
		addStaticProperty( "sensitivity", &LUAInputSystem::GetMouseSensitivity ).
		endClass();
}
