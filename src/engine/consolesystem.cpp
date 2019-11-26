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

#include "consolesystem.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать систему консоли
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::Initialize()
{
	fileLog = fopen( "engine.log", "w" );
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
