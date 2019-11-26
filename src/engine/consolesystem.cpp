//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <stdarg.h>
#include <string>

#include "consolesystem.h"

// ------------------------------------------------------------------------------------ //
// ���������������� ������� �������
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystem::Initialize()
{
	fileLog = fopen( "engine.log", "w" );
}

// ------------------------------------------------------------------------------------ //
// ������� ���������-����������
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
// ������� ���������-��������������
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
// ������� ��������� �� ������
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
// �����������
// ------------------------------------------------------------------------------------ //
le::ConsoleSystem::ConsoleSystem() :
	fileLog( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// ����������
// ------------------------------------------------------------------------------------ //
le::ConsoleSystem::~ConsoleSystem()
{
	if ( fileLog )		fclose( fileLog );
}
