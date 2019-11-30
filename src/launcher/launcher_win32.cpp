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

#define DEFAULT_GAME		"eliot"

// ------------------------------------------------------------------------------------ //
// Точка входа
// ------------------------------------------------------------------------------------ //
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpCmdLine, int nCmdShow )
{	
	// Загружаем ядро движка

	HINSTANCE					engineDLL = nullptr;
	le::LE_CreateEngineFn_t		LE_CreateEngine = nullptr;
	le::LE_DeleteEngineFn_t		LE_DeleteEngine = nullptr;

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
		// Если нет функции LE_DeleteEngine, то это не критично

		le::IEngineInternal*		engine = ( le::IEngineInternal* ) LE_CreateEngine();
		if ( !engine->LoadConfig( "config.cfg" ) )
			engine->SaveConfig( "config.cfg" );

		if ( !engine->Initialize() )
			throw std::exception( "Engine not initialized" );

		engine->RunSimulation();
	}
	catch ( const std::exception& Exception )
	{
		MessageBoxA( nullptr, Exception.what(), "Error lifeEngine", MB_OK | MB_ICONERROR );
		return 1;
	}

	return 0;
}
