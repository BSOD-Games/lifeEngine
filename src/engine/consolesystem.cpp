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
#include <string.h>
#include <sstream>
#include <vector>
#include <fstream>

#include "engine/lifeengine.h"
#include "engine.h"
#include "resourcesystem.h"
#include "convar.h"
#include "concmd.h"
#include "consolesystem.h"
#include "global.h"

le::ConCmd*     con_help = nullptr;
le::ConCmd*		con_exec = nullptr;

namespace le
{
	// ------------------------------------------------------------------------------------ //
	// Консольная комманда помощи
	// ------------------------------------------------------------------------------------ //
	void CMD_Help( UInt32_t CountArguments, const char** Arguments )
	{
		g_consoleSystem->PrintInfo( "** Console variables **" );

		for ( auto it = g_consoleSystem->vars.begin(), itEnd = g_consoleSystem->vars.end(); it != itEnd; ++it )
		{
			ConVar*             conVar = ( ConVar* ) it->second;
			g_consoleSystem->PrintInfo("%s : %s. Default value: %s", conVar->GetName(), conVar->GetHelpText(), conVar->GetValueDefault() );
		}

		g_consoleSystem->PrintInfo( "" );
		g_consoleSystem->PrintInfo( "** Console commands **" );

		for ( auto it = g_consoleSystem->commands.begin(), itEnd = g_consoleSystem->commands.end(); it != itEnd; ++it )
		{
			ConCmd*         conCmd = ( ConCmd* ) it->second;
			g_consoleSystem->PrintInfo( "%s : %s", conCmd->GetName(), conCmd->GetHelpText() );
		}
	}

	// ------------------------------------------------------------------------------------ //
	// Консольная комманда выполнения списка команд из файла
	// ------------------------------------------------------------------------------------ //
	void CMD_Exec( UInt32_t CountArguments, const char** Arguments  )
	{
		if ( CountArguments < 1 )
		{
			g_consoleSystem->PrintError( "Usage: exec <path_to_file>" );
			return;
		}

		UInt32_t		countPaths = g_resourceSystem->GetCountPaths();
		std::string		pathFile = Arguments[ 0 ];
		
		for ( UInt32_t index = 0; index < countPaths; ++index )
		{
			std::string			pathDir = g_resourceSystem->GetPath( index );
			std::ifstream		file( pathDir + "/" + pathFile );
			if ( !file.is_open() )		continue;

			std::string			lineFile;
			while ( std::getline( file, lineFile ) )
				g_consoleSystem->Exec( lineFile.c_str() );

			g_consoleSystem->PrintInfo( "File execuded [%s], founded in [%s]", pathFile.c_str(), pathDir.c_str() );
			return;
		}

		g_consoleSystem->PrintError( "File [%s] not founded", pathFile.c_str() );
	}
}

// ------------------------------------------------------------------------------------ //
// Инициализировать систему консоли
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::Initialize( const char* LogFile )
{
	fileLog = fopen( LogFile, "w" );

	con_help = new ConCmd();
	con_help->Initialize( "help", "Show help variables and comands", CMD_Help );
	
	con_exec = new ConCmd();
	con_exec->Initialize( "exec", "Execute file with commands", CMD_Exec );

	RegisterCommand( con_help );
	RegisterCommand( con_exec );
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать переменную в консоли
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::RegisterVar( IConVar* ConVar )
{
	LIFEENGINE_ASSERT( ConVar );

	if ( vars.find( ConVar->GetName() ) != vars.end() ) return;
	ConVar->IncrementReference();
	vars[ ConVar->GetName() ] = ConVar;
}

// ------------------------------------------------------------------------------------ //
// Зарегестрировать команду в консоли
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::RegisterCommand( IConCmd* ConCmd )
{
	LIFEENGINE_ASSERT( ConCmd );

	if ( commands.find( ConCmd->GetName() ) != commands.end() ) return;
	ConCmd->IncrementReference();
	commands[ ConCmd->GetName() ] = ConCmd;
}

// ------------------------------------------------------------------------------------ //
// Удалить переменную из консоли
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::UnregisterVar( const char* Name )
{
	auto			it = vars.find( Name );

	if ( it == vars.end() ) return;
	if ( it->second->GetCountReferences() <= 1 )
		it->second->Release();
	else
		it->second->DecrementReference();

	vars.erase( it );
}

// ------------------------------------------------------------------------------------ //
// Удалить команду из консоли
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::UnregisterCommand( const char* Name )
{
	auto			it = commands.find( Name );

	if ( it == commands.end() ) return;
	if ( it->second->GetCountReferences() <= 1 )
		it->second->Release();
	else
		it->second->DecrementReference();

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
		// Разбиваем строку на подстроки

		bool							isFindApostrof = false;
		bool							isEndSubstring = false;
		std::string						tempString;
		std::vector<const char*>		arrayArguments;

		for ( UInt32_t index = 0, count = arguments.size(); index < count; ++index )
		{
			if ( arguments[ index ] == '\"' )
			{
				isFindApostrof = !isFindApostrof;

				if ( isFindApostrof )	continue;
				else					isEndSubstring = true;
			}
			else if ( !isFindApostrof && arguments[ index ] == ' ' && !tempString.empty() )
				isEndSubstring = true;

			if ( !isEndSubstring )		tempString += arguments[ index ];
			if ( isEndSubstring || index + 1 >= count )
			{
				arrayArguments.push_back( new char[ tempString.size() ] );
				strcpy( ( char* ) arrayArguments[ arrayArguments.size() - 1 ], tempString.data() );

				tempString.clear();
				isEndSubstring = false;
			}
		}

		// Выполняем команду

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
    va_list			argList = {};
	std::string		message = "[Info] " + std::string( Message ) + "\n";

	va_start( argList, Message );
	vfprintf( stdout, message.c_str(), argList );
	fflush( stdout );
	va_end( argList );

	if ( !fileLog ) return;

	va_start( argList, Message );
	vfprintf( fileLog, message.c_str(), argList );
	fflush( fileLog );
    va_end( argList );
}

// ------------------------------------------------------------------------------------ //
// Вывести сообщение-предупреждение
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::PrintWarning( const char* Message, ... )
{
    va_list			argList = {};
	std::string		message = "[Warning] " + std::string( Message ) + "\n";

	va_start( argList, Message );
	vfprintf( stdout, message.c_str(), argList );
	fflush( stdout );
	va_end( argList );

	if ( !fileLog ) return;

	va_start( argList, Message );
	vfprintf( fileLog, message.c_str(), argList );
	fflush( fileLog );
    va_end( argList );
}

// ------------------------------------------------------------------------------------ //
// Вывести сообщение об ошибке
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::PrintError( const char* Message, ... )
{
    va_list			argList = {};
	std::string		message = "[Error] " + std::string( Message ) + "\n";

	va_start( argList, Message );
	vfprintf( stderr, message.c_str(), argList );
	fflush( stderr );
	va_end( argList );

	if ( !fileLog ) return;

	va_start( argList, Message );
	vfprintf( fileLog, message.c_str(), argList );
	fflush( fileLog );
    va_end( argList );
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
// Получить фабрику системы консоли
// ------------------------------------------------------------------------------------ //
le::IFactory* le::ConsoleSystem::GetFactory() const
{
	return ( IFactory* ) &consoleSystemFactory;
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
	if ( fileLog )
	{
		fclose( fileLog );
		fileLog = nullptr;
	}

	for ( auto it = vars.begin(), itEnd = vars.end(); it != itEnd; ++it )
	{
		if ( it->second->GetCountReferences() <= 1 )
			it->second->Release();
		else
			it->second->DecrementReference();
	}

	for ( auto it = commands.begin(), itEnd = commands.end(); it != itEnd; ++it )
	{
		if ( it->second->GetCountReferences() <= 1 )
			it->second->Release();
		else
			it->second->DecrementReference();
	}

	con_help = nullptr;
	con_exec = nullptr;
}
