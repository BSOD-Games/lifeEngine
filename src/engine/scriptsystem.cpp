//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

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
	
	LUAButtonCode::Register( LuaVM );
	LUAConsoleSystem::Register( LuaVM );
	LUAInputSystem::Register( LuaVM );
	LUAVector2D::Register( LuaVM );
	LUAVector3D::Register( LuaVM );
	LUAVector4D::Register( LuaVM );
}
