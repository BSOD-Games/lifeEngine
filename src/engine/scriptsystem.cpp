//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

extern "C"
{
	#include <lua.h>
	#include <luajit.h>
}

#include "global.h"
#include "engine/consolesystem.h"
#include "engine/scriptsystem.h"
#include "engine/script.h"
#include "scriptsapi/scriptsapi.h"

// ------------------------------------------------------------------------------------ //
// Create script
// ------------------------------------------------------------------------------------ //
le::IScript* le::ScriptSystem::CreateScript( const char* Path ) const
{
	Script*			script = new Script();
	if ( !script->Load( Path ) )
	{
		delete script;
		g_consoleSystem->PrintError( "Script [%s] not created", Path );
		return nullptr;
	}

	return script;
}

// ------------------------------------------------------------------------------------ //
// Initialize script system
// ------------------------------------------------------------------------------------ //
bool le::ScriptSystem::Initialize( le::IEngine* Engine )
{	
	g_consoleSystem->PrintInfo( "Lua version: " LUA_VERSION );
	g_consoleSystem->PrintInfo( "LuaJIT version: " LUAJIT_VERSION );
	return true;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::ScriptSystem::ScriptSystem()
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::ScriptSystem::~ScriptSystem()
{}

// ------------------------------------------------------------------------------------ //
// Register engine API
// ------------------------------------------------------------------------------------ //
void le::ScriptSystem::RegisterEngineAPI( lua_State* LuaVM )
{
	if ( !LuaVM ) return;
	
	LUAConsoleSystem::Register( LuaVM );
	LUAVector2D::Register( LuaVM );
	LUAVector3D::Register( LuaVM );
	LUAVector4D::Register( LuaVM );
	LUAInputSystem::Register( LuaVM );
	LUAConCmd::Register( LuaVM );
	LUAImage::Register( LuaVM );
	LUAWindow::Register( LuaVM );
	LUARay::Register( LuaVM );
	LUACamera::Register( LuaVM );
	LUAShaderParameter::Register( LuaVM );
	LUAColor::Register( LuaVM );
	LUAResourceSystem::Register( LuaVM );
	LUAMesh::Register( LuaVM );
	LUAModel::Register( LuaVM );
	LUAStudioRender::Register( LuaVM );
}
