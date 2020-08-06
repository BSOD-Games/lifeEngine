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
	#include <luajit.h>
}

#include <stdexcept>
#include <vector>
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
	try
	{
		if ( !isExistStart )		return;
		luabridge::getGlobal( luaState, "Start" )( );
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( Exception.what() );
	}
}

// ------------------------------------------------------------------------------------ //
// Call function "Update" from script
// ------------------------------------------------------------------------------------ //
void le::Script::Update()
{
	try
	{
		if ( !isExistUpdate )		return;
		luabridge::getGlobal( luaState, "Update" )( );
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( Exception.what() );
	}
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
	try
	{
		// Create new lua VM and open libs
		luaState = luaL_newstate();
		luaJIT_setmode( luaState, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON );
		luaL_openlibs( luaState );

		// Register in lua EngineAPI
		ScriptSystem::RegisterEngineAPI( luaState );

		// Loading script
		bool							isLoaded = false;		
		std::vector< std::string >		errorMessages;

		for ( UInt32_t index = 0, count = g_resourceSystem->GetCountPaths(); index < count; ++index )
		{
			// Getting full path
			std::string			fullPath = std::string( g_resourceSystem->GetPath( index ) ) + "/" + Path;

			// Loading script to lua VM			
			int					result = luaL_loadfile( luaState, fullPath.c_str() );
			if ( result != 0 )
				switch ( result )
				{
				case LUA_ERRFILE:			
					errorMessages.push_back( fullPath + ": Cannot open or read the file" );
					continue;

				case LUA_ERRSYNTAX:			
					errorMessages.push_back( fullPath + ": Syntax error during pre-compilation" );
					continue;

				case LUA_ERRMEM:			
					errorMessages.push_back( fullPath + ": Memory allocation error" );
					continue;

				default:					
					errorMessages.push_back( fullPath + ": Unknown error :(" );
					continue;
				}

			// Initialize script
			result = lua_pcall( luaState, 0, 1, 0 );
			if ( result != 0 )
			{
				errorMessages.push_back( luabridge::LuaException( luaState, result ).what() );
				continue;
			}

			isLoaded = true;
			g_consoleSystem->PrintInfo( "Script loaded from [%s]", fullPath.c_str() );
			break;
		}

		if ( !isLoaded )
		{
			// Filling message of error
			std::string			errorMessage;
			for ( UInt32_t index = 0, count = errorMessages.size(); index < count; ++index )
			{
				errorMessage += errorMessages[ index ];
				if ( index + 1 < count )		errorMessage += "\n> ";
			}

			throw std::runtime_error( errorMessage );
		}

		isExistStart = !luabridge::getGlobal( luaState, "Start" ).isNil();
		isExistUpdate = !luabridge::getGlobal( luaState, "Update" ).isNil();
		return true;
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( Exception.what() );
		Unload();
		return false;
	}
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
