//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <stdarg.h>
#include <string>
#include <sstream>
#include <vector>

#include "engine/lifeengine.h"
#include "engine.h"
#include "convar.h"
#include "concmd.h"
#include "consolesystem.h"
#include "global.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать систему консоли
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::Initialize()
{
	fileLog = fopen( "engine.log", "w" );
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать переменную в консоли
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::RegisterVar( IConVar* ConVar )
{
	LIFEENGINE_ASSERT( ConVar );
	
	if ( vars.find( ConVar->GetName() ) != vars.end() ) return;
	vars[ ConVar->GetName() ] = ConVar;
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать команду в консоли
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::RegisterCommand( IConCmd* ConCmd )
{
	LIFEENGINE_ASSERT( ConCmd );

	if ( commands.find( ConCmd->GetName() ) != commands.end() ) return;
	commands[ ConCmd->GetName() ] = ConCmd;
}

// ------------------------------------------------------------------------------------ //
// Удалить переменную из консоли
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::UnregisterVar( const char* Name )
{
	auto			it = vars.find( Name );

	if ( it == vars.end() ) return;
	vars.erase( it );
}

// ------------------------------------------------------------------------------------ //
// Удалить команду из консоли
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::UnregisterCommand( const char* Name )
{
	auto			it = commands.find( Name );

	if ( it == commands.end() ) return;
	commands.erase( it );
}

// ------------------------------------------------------------------------------------ //
// Выполнить команду
// ------------------------------------------------------------------------------------ //
bool le::ConsoleSystem::Exec( const char* Command )
{
	std::string			command( Command );
	if ( command.empty() ) return false;

	// Находим название комманды или переменной в строке

	UInt32_t		startIndexName = command.find_first_not_of( ' ' );
	UInt32_t		endIndexName = command.find_first_of( ' ', startIndexName );
	std::string		name = command.substr( startIndexName, endIndexName - startIndexName );
	std::string		arguments = endIndexName != std::string::npos ? command.substr( endIndexName + 1, command.size() - endIndexName ) : "";

	// Ищем по названию переменную, если нашли то меняем ее значение

	auto		itVar = vars.find( name );
	if ( itVar != vars.end() )
	{
		IConVar*		var = itVar->second;

		if ( !arguments.empty() && arguments.find_first_not_of( ' ' ) != std::string::npos )
			var->SetValue( arguments.c_str(), var->GetType() );
		else
		{
			std::string		conVar;
			if ( var->GetType() == CVT_STRING )
				conVar = var->GetValueString();
			else
				switch ( var->GetType() )
				{
				case CVT_INT:		conVar = std::to_string( var->GetValueInt() ); break;
				case CVT_FLOAT:		conVar = std::to_string( var->GetValueFloat() ); break;
				case CVT_BOOL:		conVar = var->GetValueBool() ? "true" : "fasle"; break;
				default:			conVar = ""; break;
				}

			PrintInfo( "%s: %s", name.c_str(), itVar->second->GetHelpText() );
			PrintInfo( "Value: %s", conVar.c_str() );
			PrintInfo( "Default value: %s", itVar->second->GetValueDefault() );
		}

		return true;
	}

	// Ищем по названию комманду, если нашли то выполняем ее

	auto		itCommand = commands.find( name );
	if ( itCommand != commands.end() )
	{
		std::stringstream				strStream( arguments );
		std::string						tempString;
		std::vector<const char*>		arrayArguments;

		while ( !strStream.eof() )
		{
			strStream >> tempString;
			arrayArguments.push_back( new char[ tempString.size() ] );
			strcpy( ( char* ) arrayArguments[ arrayArguments.size() - 1 ], tempString.data() );
		}

		itCommand->second->Exec( arrayArguments.size(), arrayArguments.data() );

		for ( UInt32_t index = 0, count = arrayArguments.size(); index < count; index++ )	
			delete[] arrayArguments[ index ];

		return true;
	}

	PrintError( "Not correct command: \"%s\"", Command );
	return false;
}

// ------------------------------------------------------------------------------------ //
// Вывести сообщение-информацию
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::PrintInfo( const char* Message, ... )
{
	if ( !fileLog ) return;
	va_list			argList = nullptr;	

	va_start( argList, Message );	
	vfprintf( fileLog, ( "[Info] " + std::string( Message ) + "\n" ).c_str(), argList );
	va_end( argList );

	fflush( fileLog );
}

// ------------------------------------------------------------------------------------ //
// Вывести сообщение-предупреждение
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::PrintWarning( const char* Message, ... )
{
	if ( !fileLog ) return;
	va_list			argList = nullptr;

	va_start( argList, Message );
	vfprintf( fileLog, ( "[Warning] " + std::string( Message ) + "\n" ).c_str(), argList );
	va_end( argList );

	fflush( fileLog );
}

// ------------------------------------------------------------------------------------ //
// Вывести сообщение об ошибке
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::PrintError( const char* Message, ... )
{
	if ( !fileLog ) return;
	va_list			argList = nullptr;

	va_start( argList, Message );
	vfprintf( fileLog, ( "[Error] " + std::string( Message ) + "\n" ).c_str(), argList );
	va_end( argList );

	fflush( fileLog );
}

// ------------------------------------------------------------------------------------ //
// Получить консольную переменную
// ------------------------------------------------------------------------------------ //
le::IConVar* le::ConsoleSystem::GetVar( const char* Name ) const
{
	auto			it = vars.find( Name );

	if ( it == vars.end() ) return nullptr;
	return it->second;
}

// ------------------------------------------------------------------------------------ //
// Получить консольную команду
// ------------------------------------------------------------------------------------ //
le::IConCmd* le::ConsoleSystem::GetCommand( const char* Name ) const
{
	auto			it = commands.find( Name );

	if ( it == commands.end() ) return nullptr;
	return it->second;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ConsoleSystem::ConsoleSystem() :
	fileLog( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ConsoleSystem::~ConsoleSystem()
{
	if ( fileLog )		fclose( fileLog );
}
