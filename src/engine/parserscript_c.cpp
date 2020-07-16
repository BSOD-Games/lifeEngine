//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <fstream>

#include "engine/ifactory.h"
#include "engine/iscript.h"

#include "global.h"
#include "consolesystem.h"
#include "parserscript_c.h"

// ------------------------------------------------------------------------------------ //
// Get version
// ------------------------------------------------------------------------------------ //
const char* le::ParserScriptC::GetVersion() const
{
	return "1.0";
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::ParserScriptC::GetName() const
{
	return "C code";
}

// ------------------------------------------------------------------------------------ //
// Get count file extensions
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ParserScriptC::GetCountFileExtensions() const
{
	return 1;
}

// ------------------------------------------------------------------------------------ //
// Get file extensions
// ------------------------------------------------------------------------------------ //
const char** le::ParserScriptC::GetFileExtensions() const
{
	static const char*			extensions[ 1 ] = { "c" };
	return extensions;
}

// ------------------------------------------------------------------------------------ //
// Get author
// ------------------------------------------------------------------------------------ //
const char* le::ParserScriptC::GetAuthor() const
{
	return "Egor Pogulyaka";
}

// ------------------------------------------------------------------------------------ //
// Read
// ------------------------------------------------------------------------------------ //
le::IScript* le::ParserScriptC::Read( const char* Path, UInt32_t CountFunctions, ScriptDescriptor::Symbol* Functions, UInt32_t CountVars, ScriptDescriptor::Symbol* Vars, IFactory* ScriptSystemFactory )
{
	std::ifstream			file( Path );
	if ( !file.is_open() )
	{
		g_consoleSystem->PrintError( "Failed open script" );
		return nullptr;
	}

	std::string			code;
	std::getline( file, code, '\0' );
	if ( code.empty() )
	{
		g_consoleSystem->PrintError( "Script is empty" );
		return nullptr;
	}

	IScript*			script = ( IScript* ) ScriptSystemFactory->Create( SCRIPT_INTERFACE_VERSION );
	if ( !script )
	{
		g_consoleSystem->PrintError( "Interface le::IScript [%s] not founded in script system", SCRIPT_INTERFACE_VERSION );
		return nullptr;
	}

	ScriptDescriptor		scriptDescriptor;
	scriptDescriptor.code = code.c_str();
	scriptDescriptor.countFunctions = CountFunctions;
	scriptDescriptor.functions = Functions;
	scriptDescriptor.countVars = CountVars;
	scriptDescriptor.vars = Vars;

	if ( !script->Load( scriptDescriptor ) )
	{
		g_consoleSystem->PrintError( "Failed loading script" );

		script->Release();
		return nullptr;
	}

	return script;
}
