//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string>
#include <stdarg.h>
#include <sstream>

#include "../../global.h"
#include "engine/consolesystem.h"

namespace scripts_api
{
	#include "consolesystem.h"
}

// ------------------------------------------------------------------------------------ //
// Parse message
// ------------------------------------------------------------------------------------ //
inline void ConsoleSystem_ParseMessage( const char* Format, va_list ArgList, std::string& Message )
{
	std::stringstream		strStream;
	for ( const char* var = Format; *var; var++ )
	{
		if ( *var != '%' )
		{
			strStream << *var;
			continue;
		}

		switch ( *++var )
		{
		case 's':
			strStream << va_arg( ArgList, const char* );
			break;

		case 'i':
		case 'b':
			strStream << va_arg( ArgList, int );
			break;

		case 'p':
			strStream << va_arg( ArgList, void* );
			break;

		case 'c':
			strStream << ( char ) va_arg( ArgList, int );
			break;

		case 'f':
		case 'd':
			strStream << va_arg( ArgList, double );
			break;
		}
	}

	Message = strStream.str();
}

// ------------------------------------------------------------------------------------ //
// Print info to console
// ------------------------------------------------------------------------------------ //
void scripts_api::ConsoleSystem_Info( const char* Format, ... )
{
	va_list			argList = {};
	va_start( argList, Format );

	std::string			message;
	ConsoleSystem_ParseMessage( Format, argList, message );
	le::g_consoleSystem->PrintInfo(message.c_str() );

	va_end( argList );
}

// ------------------------------------------------------------------------------------ //
// Print warning to console
// ------------------------------------------------------------------------------------ //
void scripts_api::ConsoleSystem_Warning( const char* Format, ... )
{
	va_list			argList = {};
	va_start( argList, Format );

	std::string			message;
	ConsoleSystem_ParseMessage( Format, argList, message );
	le::g_consoleSystem->PrintWarning(message.c_str() );

	va_end( argList );
}

// ------------------------------------------------------------------------------------ //
// Print error to console
// ------------------------------------------------------------------------------------ //
void scripts_api::ConsoleSystem_Error( const char* Format, ... )
{
	va_list			argList = {};
	va_start( argList, Format );

	std::string			message;
	ConsoleSystem_ParseMessage( Format, argList, message );
	le::g_consoleSystem->PrintError(message.c_str() );

	va_end( argList );
}

// ------------------------------------------------------------------------------------ //
// Execute console command
// ------------------------------------------------------------------------------------ //
void scripts_api::ConsoleSystem_Exec( const char* Command )
{
	le::g_consoleSystem->Exec( Command );
}