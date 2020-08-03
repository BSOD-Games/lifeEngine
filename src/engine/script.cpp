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
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <stdexcept>
#include <exception>
#include <fstream>
#include <LuaBridge/LuaBridge.h>

#include "global.h"
#include "engine/resourcesystem.h"
#include "engine/consolesystem.h"
#include "engine/scriptsystem.h"
#include "engine/script.h"

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::Script::IncrementReference()
{
	++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::Script::DecrementReference()
{
	--countReferences;
}

// ------------------------------------------------------------------------------------ //
// Release
// ------------------------------------------------------------------------------------ //
void le::Script::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Script::GetCountReferences() const
{
	return countReferences;
}

// ------------------------------------------------------------------------------------ //
// Call function "Start" from script
// ------------------------------------------------------------------------------------ //
void le::Script::Start()
{
	if ( !isExistStart )		return;
	luabridge::getGlobal( luaState, "Start" )( );
}

// ------------------------------------------------------------------------------------ //
// Call function "Update" from script
// ------------------------------------------------------------------------------------ //
void le::Script::Update()
{
	if ( !isExistUpdate )		return;
	luabridge::getGlobal( luaState, "Update" )( );
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::Script::Script() :
	countReferences( 0 ),
	isExistStart( false ),
	isExistUpdate( false ),
	luaState( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::Script::~Script()
{
	Unload();
}

// ------------------------------------------------------------------------------------ //
// Load script
// ------------------------------------------------------------------------------------ //
bool le::Script::Load( const char* Path )
{
	// Create new lua VM and open libs
	luaState = luaL_newstate();
	luaL_openlibs( luaState );

	// Register in lua EngineAPI
	ScriptSystem::RegisterEngineAPI( luaState );
	
	// Loading script
	bool			isLoaded = false;
	std::ifstream	file;

	for ( UInt32_t index = 0, count = g_resourceSystem->GetCountPaths(); index < count; ++index )
	{
		// Getting full path
		std::string			fullPath = std::string( g_resourceSystem->GetPath( index ) ) + "/" + Path;

		// Open file
		file.clear();
		file.open( fullPath );
		if ( !file.is_open() )		continue;

		// If file exists - put all code from file in lua VM
		std::string			code;
		std::getline( file, code, '\0' );
		
		luaL_dostring( luaState, code.c_str() );
		isLoaded = true;
		break;
	}

	if ( !isLoaded )
	{
		Unload();
		return false;
	}

	isExistStart = !luabridge::getGlobal( luaState, "Start" ).isNil();
	isExistUpdate = !luabridge::getGlobal( luaState, "Update" ).isNil();
	return true;
}

// ------------------------------------------------------------------------------------ //
// Unload script
// ------------------------------------------------------------------------------------ //
void le::Script::Unload()
{
	if ( luaState )
	{
		lua_close( luaState );
		luaState = nullptr;
	}
}
