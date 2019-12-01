//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://gitlab.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include <exception>

#include "engine/paths.h"
#include "engine/lifeengine.h"
#include "engine/iengineinternal.h"
#include <fstream>
#define DEFAULT_GAME		"eliot"

// ------------------------------------------------------------------------------------ //
// Критическая ошибка
// ------------------------------------------------------------------------------------ //
void Engine_CriticalError( const char* Message )
{
	std::ofstream			fileLog( "engine.log", std::ios::app );

	MessageBoxA( nullptr, Message, "Error lifeEngine", MB_OK | MB_ICONERROR );	
	fileLog << std::endl << Message;

	exit( 1 );
}

// ------------------------------------------------------------------------------------ //
// Точка входа
// ------------------------------------------------------------------------------------ //
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpCmdLine, int nCmdShow )
{	
	// Загружаем ядро движка

	HINSTANCE								engineDLL = nullptr;
	le::LE_CreateEngineFn_t					LE_CreateEngine = nullptr;
	le::LE_DeleteEngineFn_t					LE_DeleteEngine = nullptr;
	le::LE_SetCriticalErrorFn_t				LE_SetCriticalError = nullptr;

	SetDllDirectoryA( "engine" );

	try
	{
		engineDLL = LoadLibraryA( "engine/" LIFEENGINE_ENGINE_DLL );
		if ( !engineDLL )
			throw std::exception( "Faile loaded engine/" LIFEENGINE_ENGINE_DLL );

		LE_CreateEngine = ( le::LE_CreateEngineFn_t ) GetProcAddress( engineDLL, "LE_CreateEngine" );
		if ( !LE_CreateEngine )
			throw std::exception( "Faile get adress on function LE_CreateEngine" );

		LE_DeleteEngine = ( le::LE_DeleteEngineFn_t ) GetProcAddress( engineDLL, "LE_DeleteEngine" );
		LE_SetCriticalError = ( le::LE_SetCriticalErrorFn_t ) GetProcAddress( engineDLL, "LE_SetCriticalError" );
		// Если нет функции LE_DeleteEngine или LE_SetCriticalErrorCallback, то это не критично

		LE_SetCriticalError( Engine_CriticalError );
		le::IEngineInternal*		engine = ( le::IEngineInternal* ) LE_CreateEngine();

		if ( !engine->LoadConfig( "config.cfg" ) )
			engine->SaveConfig( "config.cfg" );

		if ( !engine->Initialize() )
			throw std::exception( "The engine is not initialized. See the logs for details" );

		engine->RunSimulation();
	}
	catch ( const std::exception& Exception )
	{
		Engine_CriticalError( Exception.what() );
		return 1;
	}

	return 0;
}
