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

//---------------------------------------------------------------------//

struct EngineDescriptor
{
    EngineDescriptor() :
        engineDLL( nullptr ),
        engine( nullptr ),
        LE_CreateEngine( nullptr ),
        LE_DeleteEngine( nullptr ),
        LE_SetCriticalError( nullptr )
    {}

    void        Clear()
    {
        engineDLL = nullptr;
        engine = nullptr;
        LE_CreateEngine = nullptr;
        LE_DeleteEngine = nullptr;
        LE_SetCriticalError = nullptr;
    }

    bool        IsLoaded() const
    {
        return engineDLL;
    }

    HINSTANCE						engineDLL;
    le::IEngineInternal*            engine;
    le::LE_CreateEngineFn_t			LE_CreateEngine;
    le::LE_DeleteEngineFn_t			LE_DeleteEngine;
    le::LE_SetCriticalErrorFn_t		LE_SetCriticalError;
} g_engineDescriptor;

void            Engine_CriticalError( const char* Message );

//---------------------------------------------------------------------//

// ------------------------------------------------------------------------------------ //
// Load engine
// ------------------------------------------------------------------------------------ //
void Application_LoadEngine()
{
    // Loading engine
    g_engineDescriptor.engineDLL = LoadLibraryA( "engine/" LIFEENGINE_ENGINE_DLL );
    if ( !g_engineDescriptor.engineDLL )               
        throw std::runtime_error( "Faile loaded engine/" LIFEENGINE_ENGINE_DLL );

    g_engineDescriptor.LE_CreateEngine = ( le::LE_CreateEngineFn_t ) GetProcAddress( g_engineDescriptor.engineDLL, "LE_CreateEngine" );
    if ( !g_engineDescriptor.LE_CreateEngine )		
        throw std::runtime_error( "Faile get adress on function LE_CreateEngine" );

    g_engineDescriptor.LE_DeleteEngine = ( le::LE_DeleteEngineFn_t ) GetProcAddress( g_engineDescriptor.engineDLL, "LE_DeleteEngine" );
    g_engineDescriptor.LE_SetCriticalError = ( le::LE_SetCriticalErrorFn_t ) GetProcAddress( g_engineDescriptor.engineDLL, "LE_SetCriticalError" );
   
    // Если нет функции LE_DeleteEngine или LE_SetCriticalErrorCallback, то это не критично
    g_engineDescriptor.LE_SetCriticalError( Engine_CriticalError );
    g_engineDescriptor.engine = ( le::IEngineInternal* ) g_engineDescriptor.LE_CreateEngine();
    if ( !g_engineDescriptor.engine )           throw std::runtime_error( "Failed creating engine" );
}

// ------------------------------------------------------------------------------------ //
// Unload engine
// ------------------------------------------------------------------------------------ //
void Application_UnloadEngine()
{
    if ( !g_engineDescriptor.IsLoaded() )       return;

    if ( g_engineDescriptor.engine && g_engineDescriptor.LE_DeleteEngine )
        g_engineDescriptor.LE_DeleteEngine( g_engineDescriptor.engine );
    
    FreeLibrary( g_engineDescriptor.engineDLL );
    g_engineDescriptor.Clear();
}

// ------------------------------------------------------------------------------------ //
// Critical error 
// ------------------------------------------------------------------------------------ //
void Engine_CriticalError( const char* Message )
{
    if ( g_engineDescriptor.engine )
    {
        le::IWindow*            window = g_engineDescriptor.engine->GetWindow();
        window->SetShowCursor( true );
    }

    std::ofstream			fileLog( "console.log", std::ios::app );
    MessageBoxA( nullptr, Message, "Error lifeEngine", MB_OK | MB_ICONERROR );
    fileLog << "\nCritical error: " << Message;

    Application_UnloadEngine();
    exit( 1 );
}

// ------------------------------------------------------------------------------------ //
// Start point
// ------------------------------------------------------------------------------------ //
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpCmdLine, int nCmdShow )
{	
    SetDllDirectoryA( "engine" );

    try
    {
        // Loading engine
        Application_LoadEngine();

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
            if ( !g_engineDescriptor.engine->Initialize( "engine", "console.log" ) )
                throw std::runtime_error( "The engine is not initialized. See the logs for details" );

			le::Configurations			configurations = g_engineDescriptor.engine->GetConfigurations();

			// Parsing arguments start
			for ( int index = 0; index < argc; ++index )
				if ( ( strstr( argv[ index ], "-game" ) || strstr( argv[ index ], "-g" ) ) && index + 1 < argc )
				{
					gameDir = argv[ index + 1 ];
					break;
				}

            // Create window and context render
            le::IWindowInternal*			window = ( le::IWindowInternal* ) g_engineDescriptor.engine->GetWindow();
            if ( !window->Create( "lifeEngine " LIFEENGINE_VERSION, configurations.windowWidth->GetValueInt(), configurations.windowHeight->GetValueInt(), configurations.windowFullscreen->GetValueBool() ? le::SW_FULLSCREEN : le::SW_DEFAULT ) )
                throw std::runtime_error( "Fail creating window" );

            if ( !static_cast<le::IStudioRenderInternal*>( g_engineDescriptor.engine->GetStudioRender() )->CreateContext( window->GetHandle(), configurations.windowWidth->GetValueInt(), configurations.windowHeight->GetValueInt() ) )
                throw std::runtime_error( "Fail creating context render" );

            // Загружаем игру
            if ( !g_engineDescriptor.engine->LoadGame( gameDir.c_str(), argc, ( const char** ) argv ) )
                throw std::runtime_error( ( std::string( "Failed to load game [" ) + gameDir + "]" ).c_str() );

            // Удаляем выделенную память под аргументы и запускаем игру
            LocalFree( argList );
            delete[] argv;
        }

        // Если все прошло успешно - запускаем симуляцию игры
        g_engineDescriptor.engine->RunSimulation();

        // Unload engine and DLL
        Application_UnloadEngine();
    }
    catch ( const std::exception& Exception )
    {
        Engine_CriticalError( Exception.what() );
        return 1;
    }

    return 0;
}
