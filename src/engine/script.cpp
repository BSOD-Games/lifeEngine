//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <stdexcept>
#include <exception>
#include <libtcc.h>

#include "engine/lifeengine.h"
#include "engine/engine.h"
#include "engine/scriptdescriptor.h"
#include "engine/consolesystem.h"
#include "engine/scriptsystem.h"
#include "engine/script.h"
#include "global.h"

// ------------------------------------------------------------------------------------ //
// Error callback
// ------------------------------------------------------------------------------------ //
void ErrorCallback( void* Obeject, const char* Message )
{
	le::g_consoleSystem->PrintError( Message );
}

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
// Load script
// ------------------------------------------------------------------------------------ //
bool le::Script::Load( const le::ScriptDescriptor& ScriptDescriptor )
{
	if ( tccContext )	Unload();

	try
	{
		if ( !ScriptDescriptor.code || ScriptDescriptor.code == "" )
			throw  std::runtime_error( "Script code is empty" );

		tccContext = tcc_new();
		if ( !tccContext )		throw std::runtime_error( "TCC Context is not possible to create" );

		tcc_set_error_func( tccContext, tccContext, ErrorCallback);
		tcc_set_output_type( tccContext, TCC_OUTPUT_MEMORY );
		tcc_add_include_path( tccContext, g_engine->GetGameInfo().gameDir );
		tcc_add_sysinclude_path( tccContext, ( g_engine->GetEngineDirectory() + "/scripts" ).c_str() );
		tcc_set_options( tccContext, "-nostdlib" );

		if ( tcc_compile_string( tccContext, ScriptDescriptor.code ) == -1 )
			throw std::runtime_error( "The script could not be compiled" );

		LIFEENGINE_ASSERT( g_scriptSystem );
		auto&			globalFunctions = g_scriptSystem->GetFunctions();
		auto&			globalVars = g_scriptSystem->GetVars();

		// Register global functions and vars
		for ( auto it = globalFunctions.begin(), itEnd = globalFunctions.end(); it != itEnd; ++it )
			tcc_add_symbol( tccContext, it->first.c_str(), it->second );

		for ( auto it = globalVars.begin(), itEnd = globalVars.end(); it != itEnd; ++it )
			tcc_add_symbol( tccContext, it->first.c_str(), it->second );

		// Register local functions and vars
		for ( UInt32_t index = 0; index < ScriptDescriptor.countFunctions; ++index )
		{
			const ScriptDescriptor::Symbol&		symbol = ScriptDescriptor.functions[ index ];
			tcc_add_symbol( tccContext, symbol.name, symbol.value );
		}

		for ( UInt32_t index = 0; index < ScriptDescriptor.countVars; ++index )
		{
			const ScriptDescriptor::Symbol&		symbol = ScriptDescriptor.vars[ index ];
			tcc_add_symbol( tccContext, symbol.name, symbol.value );
		}

		if ( tcc_relocate( tccContext, TCC_RELOCATE_AUTO ) < 0 )
			throw std::runtime_error( "The script could not be relocated" );
	}
	catch ( const std::exception& Exception )
	{
		g_consoleSystem->PrintError( Exception.what() );
		Unload();
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// Unload script
// ------------------------------------------------------------------------------------ //
void le::Script::Unload()
{
	if ( !tccContext )	return;

	tcc_delete( tccContext );
	tccContext = nullptr;
	functionsCache.clear();
	varsCache.clear();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::Script::Script() :
	countReferences( 0 ),
	tccContext( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::Script::~Script()
{
	Unload();
}

// ------------------------------------------------------------------------------------ //
// Is loaded script
// ------------------------------------------------------------------------------------ //
bool le::Script::IsLoaded() const
{
	return tccContext;
}

// ------------------------------------------------------------------------------------ //
// Get function
// ------------------------------------------------------------------------------------ //
void* le::Script::GetFunction( const char* Name )
{
	if ( !tccContext )		return nullptr;

	auto		itFunction = functionsCache.find( Name );
	if ( itFunction != functionsCache.end() )
		return itFunction->second;

	void*		function = tcc_get_symbol( tccContext, Name );
	functionsCache.insert( std::make_pair( Name, function ) );

	return function;
}

// ------------------------------------------------------------------------------------ //
// Get var
// ------------------------------------------------------------------------------------ //
void* le::Script::GetVar( const char* Name )
{
	if ( !tccContext )		return nullptr;

	auto		itVar = varsCache.find( Name );
	if ( itVar != varsCache.end() )
		return itVar->second;

	void*		var = tcc_get_symbol( tccContext, Name );
	varsCache.insert( std::make_pair( Name, var ) );

	return var;
}
