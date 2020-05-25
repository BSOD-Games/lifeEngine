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
#include "engine/iwindowinternal.h"
#include "engine/iconsolesystem.h"
#include "engine/iconvar.h"
#include "studiorender/istudiorenderinternal.h"

#define DEFAULT_GAME		"episodic"

// ------------------------------------------------------------------------------------ //
// Критическая ошибка
// ------------------------------------------------------------------------------------ //
void Engine_CriticalError( const char* Message )
{
    std::ofstream			fileLog( "console.log", std::ios::app );

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
        if ( !engineDLL )               throw std::runtime_error( "Faile loaded engine/" LIFEENGINE_ENGINE_DLL );

        LE_CreateEngine = ( le::LE_CreateEngineFn_t ) GetProcAddress( engineDLL, "LE_CreateEngine" );
        if ( !LE_CreateEngine )		throw std::runtime_error( "Faile get adress on function LE_CreateEngine" );

        LE_DeleteEngine = ( le::LE_DeleteEngineFn_t ) GetProcAddress( engineDLL, "LE_DeleteEngine" );
        LE_SetCriticalError = ( le::LE_SetCriticalErrorFn_t ) GetProcAddress( engineDLL, "LE_SetCriticalError" );
        // Если нет функции LE_DeleteEngine или LE_SetCriticalErrorCallback, то это не критично

        LE_SetCriticalError( Engine_CriticalError );
        le::IEngineInternal*		engine = ( le::IEngineInternal* ) LE_CreateEngine();

        {
            std::string                 gameDir = DEFAULT_GAME;

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

            // Инициализируем движок для запуска игры
            if ( !engine->Initialize( "engine", "console.log" ) )
                throw std::runtime_error( "The engine is not initialized. See the logs for details" );

			le::Configurations			configurations = engine->GetConfigurations();

			// Parsing arguments start
			for ( int index = 0; index < argc; ++index )
				if ( ( strstr( argv[ index ], "-game" ) || strstr( argv[ index ], "-g" ) ) && index + 1 < argc )
				{
					gameDir = argv[ index + 1 ];
					break;
				}

            // Create window and context render
            le::IWindowInternal*			window = ( le::IWindowInternal* ) engine->GetWindow();
            if ( !window->Create( "lifeEngine " LIFEENGINE_VERSION, configurations.windowWidth->GetValueInt(), configurations.windowHeight->GetValueInt(), configurations.windowFullscreen->GetValueBool() ? le::SW_FULLSCREEN : le::SW_DEFAULT ) )
                throw std::runtime_error( "Fail creating window" );

            if ( !static_cast<le::IStudioRenderInternal*>( engine->GetStudioRender() )->CreateContext( window->GetHandle(), configurations.windowWidth->GetValueInt(), configurations.windowHeight->GetValueInt() ) )
                throw std::runtime_error( "Fail creating context render" );

            // Загружаем игру
            if ( !engine->LoadGame( gameDir.c_str(), argc, ( const char** ) argv ) )
                throw std::runtime_error( ( std::string( "Failed to load game [" ) + gameDir + "]" ).c_str() );

            // Удаляем выделенную память под аргументы и запускаем игру
            LocalFree( argList );
            delete[] argv;
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
