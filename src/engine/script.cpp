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

#define START_FUNCTION		"Start"
#define UPDATE_FUNCTION		"Update"

//---------------------------------------------------------------------//

typedef void				( *StartFn_t )();
typedef	void				( *UpdateFn_t )();

//---------------------------------------------------------------------//

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
		auto&			symbols = g_scriptSystem->GetSymbols();

		// Register global symbols
		for ( auto it = symbols.begin(), itEnd = symbols.end(); it != itEnd; ++it )
			tcc_add_symbol( tccContext, it->first.c_str(), it->second );

		// Register local symbols
		for ( UInt32_t index = 0; index < ScriptDescriptor.countSymbols; ++index )
		{
			const ScriptDescriptor::Symbol&		symbol = ScriptDescriptor.symbols[ index ];
			tcc_add_symbol( tccContext, symbol.name, symbol.value );
		}

		if ( tcc_relocate( tccContext, TCC_RELOCATE_AUTO ) < 0 )
			throw std::runtime_error( "The script could not be relocated" );

		StartFn_t		Start = ( StartFn_t ) GetSymbol( START_FUNCTION );
		if ( Start )	Start();

		updateFn = GetSymbol( UPDATE_FUNCTION );
		if ( !updateFn )	throw std::runtime_error( "The script does not contain a function " UPDATE_FUNCTION );
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
	updateFn = nullptr;
	symbolsCache.clear();
}

// ------------------------------------------------------------------------------------ //
// Update script
// ------------------------------------------------------------------------------------ //
void le::Script::Update()
{
	if ( !tccContext ) return;
	( ( UpdateFn_t ) updateFn )();
}

// ------------------------------------------------------------------------------------ //
// Get symbol from script
// ------------------------------------------------------------------------------------ //
void* le::Script::GetSymbol( const char* Name )
{
	if ( !tccContext ) return nullptr;

	auto		itSymbol = symbolsCache.find( Name );
	if ( itSymbol != symbolsCache.end() )
		return itSymbol->second;

	void*		symbol = tcc_get_symbol( tccContext, Name );
	symbolsCache.insert( std::make_pair( Name, symbol ) );

	return symbol;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::Script::Script() :
	countReferences( 0 ),
	tccContext( nullptr ),
	updateFn( nullptr )
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
