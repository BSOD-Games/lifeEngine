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
#include <fstream>

#include "common/configurations.h"
#include "engine/paths.h"
#include "engine/lifeengine.h"
#include "engine/iengineinternal.h"

#define DEFAULT_GAME		"episodic"

// ------------------------------------------------------------------------------------ //
// Критическая ошибка
// ------------------------------------------------------------------------------------ //
void Engine_CriticalError( const char* Message )
{
	std::ofstream			fileLog( "engine.log", std::ios::app );

	MessageBoxA( nullptr, Message, "Error lifeEngine", MB_OK | MB_ICONERROR );	
	fileLog << "\nCritical error: " << Message;

	exit( 1 );
}

// ------------------------------------------------------------------------------------ //
// Точка входа
// ------------------------------------------------------------------------------------ //
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpCmdLine, int nCmdShow )
{	
	HINSTANCE								engineDLL = nullptr;
	le::LE_CreateEngineFn_t					LE_CreateEngine = nullptr;
	le::LE_DeleteEngineFn_t					LE_DeleteEngine = nullptr;
	le::LE_SetCriticalErrorFn_t				LE_SetCriticalError = nullptr;

	SetDllDirectoryA( "engine" );

	try
	{
		// Загружаем ядро движка
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

		{
			// Загружаем конфигурации движка, если нет - сохраняем
			if ( !engine->LoadConfig( "config.cfg" ) )
				engine->SaveConfig( "config.cfg" );

			std::string				gameDir = DEFAULT_GAME;
			le::Configurations		configurations = engine->GetConfigurations();

			// Cчитываем аргументы запуска лаунчера
			int				argc;
			LPWSTR*			argList = CommandLineToArgvW( GetCommandLineW(), &argc );
			char**			argv = new char* [ argc ];

			for ( int index = 0; index < argc; ++index )
			{
				int			size = wcslen( argList[ index ] ) + 1;
				argv[ index ] = new char[ size ];

				wcstombs( argv[ index ], argList[ index ], size );
			}

			// Парсим аргументы запуска и меняем конфигурации
			for ( int index = 0; index < argc; ++index )
			{
				if ( ( strstr( argv[ index ], "-game" ) || strstr( argv[ index ], "-g" ) ) && index + 1 < argc )
				{
					gameDir = argv[ index + 1 ];
					++index;
				}
				else if ( ( strstr( argv[ index ], "-width" ) || strstr( argv[ index ], "-w" ) ) && index + 1 < argc )
				{
					configurations.windowWidth = atoi( argv[ index + 1 ] );
					++index;
				}
				else if ( ( strstr( argv[ index ], "-height" ) || strstr( argv[ index ], "-h" ) ) && index + 1 < argc )
				{
					configurations.windowHeight = atoi( argv[ index + 1 ] );
					++index;
				}
			}

			engine->SetConfig( configurations );

			// Удаляем выделенную память под аргументы и запускаем игру
			LocalFree( argList );
			delete[] argv;

			// Инициализируем движок для запуска игры
			if ( !engine->Initialize() )
				throw std::exception( "The engine is not initialized. See the logs for details" );

			// Загружаем игру
			if ( !engine->LoadGame( gameDir.c_str() ) )
				throw std::exception( ( std::string( "Failed to load game [" ) + gameDir + "]" ).c_str() );
		}

		// Если все прошло успешно - запускаем симуляцию игры
		engine->RunSimulation();

		if ( LE_DeleteEngine ) LE_DeleteEngine( engine );
		FreeLibrary( engineDLL );
	}
	catch ( const std::exception& Exception )
	{
		Engine_CriticalError( Exception.what() );
		return 1;
	}

	return 0;
}
