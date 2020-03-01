//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://gitlab.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>
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

    SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "Error lifeEngine", Message, nullptr );
    fileLog << "\nCritical error: " << Message;

    exit( 1 );
}

// ------------------------------------------------------------------------------------ //
// Точка входа
// ------------------------------------------------------------------------------------ //
int main( int argc, char** argv )
{
    void*                                   engineSO = nullptr;
    le::LE_CreateEngineFn_t					LE_CreateEngine = nullptr;
    le::LE_DeleteEngineFn_t					LE_DeleteEngine = nullptr;
    le::LE_SetCriticalErrorFn_t				LE_SetCriticalError = nullptr;

    try
    {
        // Загружаем ядро движка
        engineSO = SDL_LoadObject( "engine/" LIFEENGINE_ENGINE_DLL );
        if ( !engineSO )            throw std::runtime_error( "Faile loaded engine/" LIFEENGINE_ENGINE_DLL );

        LE_CreateEngine = ( le::LE_CreateEngineFn_t ) SDL_LoadFunction( engineSO, "LE_CreateEngine" );
        if ( !LE_CreateEngine )		throw std::runtime_error( "Faile get adress on function LE_CreateEngine" );

        LE_DeleteEngine = ( le::LE_DeleteEngineFn_t ) SDL_LoadFunction( engineSO, "LE_DeleteEngine" );
        LE_SetCriticalError = ( le::LE_SetCriticalErrorFn_t ) SDL_LoadFunction( engineSO, "LE_SetCriticalError" );
        // Если нет функции LE_DeleteEngine или LE_SetCriticalErrorCallback, то это не критично

        LE_SetCriticalError( Engine_CriticalError );
        le::IEngineInternal*		engine = ( le::IEngineInternal* ) LE_CreateEngine();

        {
            // Загружаем конфигурации движка, если нет - сохраняем
            if ( !engine->LoadConfig( "config.cfg" ) )
                engine->SaveConfig( "config.cfg" );

            std::string                 gameDir = DEFAULT_GAME;
            le::Configurations          configurations = engine->GetConfigurations();

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

            // Инициализируем движок для запуска игры
            if ( !engine->Initialize( "engine" ) )
                throw std::runtime_error( "The engine is not initialized. See the logs for details" );

            // Загружаем игру
            if ( !engine->LoadGame( gameDir.c_str() ) )
                throw std::runtime_error( ( std::string( "Failed to load game [" ) + gameDir + "]" ).c_str() );
        }

        // Если все прошло успешно - запускаем симуляцию игры
        engine->RunSimulation();

        if ( LE_DeleteEngine ) LE_DeleteEngine( engine );
        SDL_UnloadObject( engineSO );
    }
    catch ( std::exception& Exception )
    {
        Engine_CriticalError( Exception.what() );
        return 1;
    }

    return 0;
}
