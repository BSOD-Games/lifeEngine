//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <sstream>
#include <string>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <SDL2/SDL.h>

#include "engine/lifeengine.h"
#include "engine/paths.h"
#include "engine/engine.h"
#include "engine/window.h"
#include "engine/igame.h"
#include "engine/global.h"
#include "engine/convar.h"
#include "engine/concmd.h"
#include "engine/buildnum.h"
#include "engine/resourcesystem.h"
#include "studiorender/istudiorenderinternal.h"
#include "materialsystem/imaterialsysteminternal.h"

LIFEENGINE_ENGINE_API( le::Engine );

// ------------------------------------------------------------------------------------ //
// Консольная команда выхода
// ------------------------------------------------------------------------------------ //
void CMD_Exit( le::UInt32_t CountArguments, const char** Arguments )
{
	if ( !le::g_engine ) return;
	le::g_engine->StopSimulation();
	exit( 0 );
}

// ------------------------------------------------------------------------------------ //
// Консольная команда вывода версии движка
// ------------------------------------------------------------------------------------ //
void CMD_Version( le::UInt32_t CountArguments, const char** Arguments )
{
	if ( !le::g_consoleSystem ) return;
	le::g_consoleSystem->PrintInfo( "lifeEngine version %i.%i.%i (build %i)", LIFEENGINE_VERSION_MAJOR, LIFEENGINE_VERSION_MINOR, LIFEENGINE_VERSION_PATCH, le::Engine_BuildNumber() );
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Engine::Engine() :
	isRunSimulation( false ),
	isInit( false ),
	studioRender( nullptr ),
	studioRenderDescriptor( { nullptr, nullptr, nullptr, nullptr } ),
	materialSystem( nullptr ),
	materialSystemDescriptor( { nullptr, nullptr, nullptr, nullptr } ),
	game( nullptr ),
	gameDescriptor( { nullptr, nullptr, nullptr, nullptr } ),
	criticalError( nullptr ),
	cmd_Exit( new ConCmd() ),
	cmd_Version( new ConCmd() )
{
	LIFEENGINE_ASSERT( !g_engine );

	g_consoleSystem = &consoleSystem;
	g_engine = this;

	configurations.fov = 75.f;
	configurations.isFullscreen = false;
	configurations.isVerticalSinc = false;
	configurations.sensitivityMouse = 1.f;
	configurations.windowWidth = 800;
	configurations.windowHeight = 600;

	consoleSystem.Initialize();
	cmd_Exit->Initialize( "exit", CMD_Exit );
	cmd_Version->Initialize( "version", CMD_Version );

	consoleSystem.RegisterCommand( cmd_Exit );
	consoleSystem.RegisterCommand( cmd_Version );
}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Engine::~Engine()
{
	if ( game )				UnloadModule_Game();

	resourceSystem.UnloadAll();

	if ( materialSystem )	UnloadModule_MaterialSystem();
	if ( studioRender )		UnloadModule_StudioRender();	
	if ( window.IsOpen() )	window.Close();

	if ( cmd_Exit )
	{
		consoleSystem.UnregisterCommand( cmd_Exit->GetName() );
		delete cmd_Exit;
	}

	if ( cmd_Version )
	{
		consoleSystem.UnregisterCommand( cmd_Version->GetName() );
		delete cmd_Version;
	}
}

// ------------------------------------------------------------------------------------ //
// Загрузить модуль рендера
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadModule_StudioRender( const char* PathDLL )
{
	// Если модуль ранее был загружен, то удаляем
	if ( studioRenderDescriptor.handle ) UnloadModule_StudioRender();

	consoleSystem.PrintInfo( "Loading studiorender [%s]", PathDLL );

	try
	{
		// Загружаем модуль
		studioRenderDescriptor.handle = SDL_LoadObject( PathDLL );
		if ( !studioRenderDescriptor.handle )	throw std::exception( SDL_GetError() );

		// Берем из модуля API для работы с ним
		studioRenderDescriptor.LE_CreateStudioRender = ( LE_CreateStudioRenderFn_t ) SDL_LoadFunction( studioRenderDescriptor.handle, "LE_CreateStudioRender" );
		studioRenderDescriptor.LE_DeleteStudioRender = ( LE_DeleteStudioRenderFn_t ) SDL_LoadFunction( studioRenderDescriptor.handle, "LE_DeleteStudioRender" );
		studioRenderDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( studioRenderDescriptor.handle, "LE_SetCriticalError" );
		if ( !studioRenderDescriptor.LE_CreateStudioRender )	throw std::exception( "Function LE_CreateStudioRender not found" );

		// Создаем рендер
		if ( studioRenderDescriptor.LE_SetCriticalError )
			studioRenderDescriptor.LE_SetCriticalError( g_criticalError );

		studioRender = ( IStudioRenderInternal* ) studioRenderDescriptor.LE_CreateStudioRender();
		if ( !studioRender->Initialize( this ) )				throw std::exception( "Fail initialize studiorender" );
	}
	catch ( std::exception& Exception )
	{
		consoleSystem.PrintError( Exception.what() );
		return false;
	}

	consoleSystem.PrintInfo( "Loaded studiorender [%s]", PathDLL );
	return true;
}

// ------------------------------------------------------------------------------------ //
// Выгрузить модуль рендера
// ------------------------------------------------------------------------------------ //
void le::Engine::UnloadModule_StudioRender()
{
	if ( !studioRender ) return;
	
	if ( studioRenderDescriptor.LE_DeleteStudioRender )
		studioRenderDescriptor.LE_DeleteStudioRender( studioRender );

	SDL_UnloadObject( studioRenderDescriptor.handle );
	studioRender = nullptr;
	studioRenderDescriptor = { nullptr, nullptr, nullptr, nullptr };

	consoleSystem.PrintInfo( "Unloaded studiorender" );
}

// ------------------------------------------------------------------------------------ //
// Загрузить модуль системы материалов
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadModule_MaterialSystem( const char* PathDLL )
{
	// Если модуль ранее был загружен, то удаляем
	if ( materialSystemDescriptor.handle ) UnloadModule_MaterialSystem();

	consoleSystem.PrintInfo( "Loading material system [%s]", PathDLL );

	try
	{
		// Загружаем модуль
		materialSystemDescriptor.handle = SDL_LoadObject( PathDLL );
		if ( !materialSystemDescriptor.handle )	throw std::exception( SDL_GetError() );

		// Берем из модуля API для работы с ним
		materialSystemDescriptor.LE_CreateMaterialSystem = ( LE_CreateMaterialSystemFn_t ) SDL_LoadFunction( materialSystemDescriptor.handle, "LE_CreateMaterialSystem" );
		materialSystemDescriptor.LE_DeleteMaterialSystem = ( LE_DeleteMaterialSystemFn_t ) SDL_LoadFunction( materialSystemDescriptor.handle, "LE_DeleteMaterialSystem" );
		materialSystemDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( materialSystemDescriptor.handle, "LE_SetCriticalError" );
		if ( !materialSystemDescriptor.LE_CreateMaterialSystem )	throw std::exception( "Function LE_CreateMaterialSystem not found" );

		// Создаем рендер
		if ( materialSystemDescriptor.LE_SetCriticalError )
			materialSystemDescriptor.LE_SetCriticalError( g_criticalError );

		materialSystem = ( IMaterialSystemInternal* ) materialSystemDescriptor.LE_CreateMaterialSystem();
		if ( !materialSystem->Initialize( this ) )				throw std::exception( "Fail initialize material system" );
	}
	catch ( std::exception& Exception )
	{
		consoleSystem.PrintError( Exception.what() );
		return false;
	}

	consoleSystem.PrintInfo( "Loaded material system [%s]", PathDLL );
	return true;
}

// ------------------------------------------------------------------------------------ //
// Выгрузить модуль системы материалов
// ------------------------------------------------------------------------------------ //
void le::Engine::UnloadModule_MaterialSystem()
{
	if ( !materialSystem ) return;

	if ( materialSystemDescriptor.LE_DeleteMaterialSystem )
		materialSystemDescriptor.LE_DeleteMaterialSystem( materialSystem );

	SDL_UnloadObject( materialSystemDescriptor.handle );
	materialSystem = nullptr;
	materialSystemDescriptor = { nullptr, nullptr, nullptr, nullptr };

	consoleSystem.PrintInfo( "Unloaded material system" );
}

// ------------------------------------------------------------------------------------ //
// Загрузить информацию об игре
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadGameInfo( const char* DirGame )
{
	gameInfo.Clear();
	consoleSystem.PrintInfo( "Loading gameinfo.txt from dir [%s]", DirGame );
	
	std::ifstream		file( std::string( DirGame ) + "/gameinfo.txt" );
	if ( !file.is_open() )
	{
		consoleSystem.PrintError( "File gameinfo.txt not found in dir [%s]", DirGame );
		return false;
	}

	std::string					stringBuffer;
	UInt32_t					stringLength = 0;
	std::getline( file, stringBuffer, '\0' );

	rapidjson::Document			document;
	document.Parse( stringBuffer.c_str() );
	if ( document.HasParseError() )
	{
		consoleSystem.PrintError( "Fail parse gameinfo.txt in dir [%s]", DirGame );
		return false;
	}

	for ( auto it = document.MemberBegin(), itEnd = document.MemberEnd(); it != itEnd; ++it )
	{
		// Путь к модулю с игровой логикой
		if ( strcmp( it->name.GetString(), "gameDLL" ) == 0 && it->value.IsString() )
		{
			stringLength = it->value.GetStringLength();

			gameInfo.gameDLL = new char[ stringLength + 1 ];
			memcpy( gameInfo.gameDLL, it->value.GetString(), stringLength );
			gameInfo.gameDLL[ stringLength ] = '\0';
		}

		// Путь к иконке
		if ( strcmp( it->name.GetString(), "icon" ) == 0 && it->value.IsString() )
		{
			stringLength = it->value.GetStringLength();

			gameInfo.icon = new char[ stringLength + 1 ];
			memcpy( gameInfo.icon, it->value.GetString(), stringLength );
			gameInfo.icon[ stringLength ] = '\0';
		}

		// Заголовок игры	
		if ( strcmp( it->name.GetString(), "title" ) == 0 && it->value.IsString() )
		{
			stringLength = it->value.GetStringLength();

			gameInfo.title = new char[ stringLength + 1 ];
			memcpy( gameInfo.title, it->value.GetString(), stringLength );
			gameInfo.title[ stringLength ] = '\0';
		}
	}

	stringLength = strlen( DirGame );
	gameInfo.gameDir = new char[ stringLength + 1 ];
	memcpy( gameInfo.gameDir, DirGame, stringLength );
	gameInfo.gameDir[ stringLength ] = '\0';

	consoleSystem.PrintInfo( "Loaded gameinfo.txt from dir [%s]", DirGame );
	return true;
}

// ------------------------------------------------------------------------------------ //
// Загрузить модуль игры
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadModule_Game( const char* PathDLL )
{
	// Если модуль ранее был загружен, то удаляем
	if ( gameDescriptor.handle ) UnloadModule_Game();

	consoleSystem.PrintInfo( "Loading game [%s]", PathDLL );

	try
	{
		// Загружаем модуль
		gameDescriptor.handle = SDL_LoadObject( PathDLL );
		if ( !gameDescriptor.handle )	throw std::exception( SDL_GetError() );

		// Берем из модуля API для работы с ним
		gameDescriptor.LE_CreateGame = ( LE_CreateGameFn_t ) SDL_LoadFunction( gameDescriptor.handle, "LE_CreateGame" );
		gameDescriptor.LE_DeleteGame = ( LE_DeleteGameFn_t ) SDL_LoadFunction( gameDescriptor.handle, "LE_DeleteGame" );
		gameDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( gameDescriptor.handle, "LE_SetCriticalError" );
		if ( !gameDescriptor.LE_CreateGame )	throw std::exception( "Function LE_CreateGame not found" );

		// Создаем игровую логику
		if ( gameDescriptor.LE_SetCriticalError )
			gameDescriptor.LE_SetCriticalError( g_criticalError );

		game = gameDescriptor.LE_CreateGame();
		if ( !game->Initialize( this ) )						throw std::exception( "Fail initialize game" );
	}
	catch ( std::exception& Exception )
	{
		consoleSystem.PrintError( Exception.what() );
		return false;
	}

	consoleSystem.PrintInfo( "Loaded game [%s]", PathDLL );
	return true;
}

// ------------------------------------------------------------------------------------ //
// Выгрузить модуль игры
// ------------------------------------------------------------------------------------ //
void le::Engine::UnloadModule_Game()
{
	if ( !game ) return;

	if ( gameDescriptor.LE_DeleteGame )
		gameDescriptor.LE_DeleteGame( game );

	SDL_UnloadObject( gameDescriptor.handle );
	game = nullptr;
	gameDescriptor = { nullptr, nullptr, nullptr, nullptr };
	
	consoleSystem.PrintInfo( "Unloaded game" );
}

// ------------------------------------------------------------------------------------ //
// Загрузить конфиг
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadConfig( const char* FilePath )
{
	consoleSystem.PrintInfo( "Loading file configurations [%s]", FilePath );

	std::ifstream		file( FilePath );
	if ( !file.is_open() )
	{
		consoleSystem.PrintError( "File configurations [%s] not found", FilePath );
		return false;
	}

	std::string					stringBuffer;
	std::getline( file, stringBuffer, '\0' );

	rapidjson::Document			document;
	document.Parse( stringBuffer.c_str() );
	if ( document.HasParseError() )
	{
		consoleSystem.PrintError( "Fail parse file configurations [%s]", FilePath );
		return false;
	}

	for ( auto it = document.MemberBegin(), itEnd = document.MemberEnd(); it != itEnd; ++it )
	{
		if ( !it->value.IsObject() ) continue;

		for ( auto itObject = it->value.MemberBegin(), itObjectEnd = it->value.MemberEnd(); itObject != itObjectEnd; ++itObject )
		{
			// Общии параметры
			if ( strcmp( it->name.GetString(), "general" ) == 0 )
			{
				// Ширина окна
				if ( strcmp( itObject->name.GetString(), "width" ) == 0 && itObject->value.IsNumber() )
					configurations.windowWidth = itObject->value.GetInt();

				// Высота окна
				else if ( strcmp( itObject->name.GetString(), "height" ) == 0 && itObject->value.IsNumber() )
					configurations.windowHeight = itObject->value.GetInt();

				// Полноэкраный ли режим
				else if ( strcmp( itObject->name.GetString(), "fullscreen" ) == 0 && itObject->value.IsBool() )
					configurations.isFullscreen = itObject->value.GetBool();
			}

			// Параметры рендера
			else if ( strcmp( it->name.GetString(), "studiorender" ) == 0 )
			{
				// Включена ли вертикальная синхронизация
				if ( strcmp( itObject->name.GetString(), "vsinc" ) == 0 && itObject->value.IsBool() )
					configurations.isVerticalSinc = itObject->value.GetBool();

				// Включена ли вертикальная синхронизация
				else if ( strcmp( itObject->name.GetString(), "fov" ) == 0 && itObject->value.IsNumber() )
					configurations.fov = itObject->value.GetFloat();
			}
		}
	}

	consoleSystem.PrintInfo( "Loaded file configurations [%s]", FilePath );
	return true;
}

// ------------------------------------------------------------------------------------ //
// Сохранить конфиг
// ------------------------------------------------------------------------------------ //
bool le::Engine::SaveConfig( const char* FilePath )
{
	consoleSystem.PrintInfo( "Saving file configurations [%s]", FilePath );
	std::ofstream		file( FilePath );
	if ( !file.is_open() )
	{
		consoleSystem.PrintError( "Fail saving file configurations [%s]. File not open" );
		return false;
	}

	file <<
		"{\n\
	\"general\": {\n\
		\"width\": " << configurations.windowWidth << ",\n\
		\"height\" : " << configurations.windowHeight << ",\n\
		\"fullscreen\" : " << ( configurations.isFullscreen ? "true" : "false" ) << "\n\
	},\n\
\n\
	\"studiorender\": {\n\
		\"vsinc\" : " << ( configurations.isVerticalSinc ? "true" : "false" ) << ",\n\
		\"fov\" : " << configurations.fov << "\n\
	}\n\
}";

	consoleSystem.PrintInfo( "Saved file configurations [%s]", FilePath );
	return true;
}

// ------------------------------------------------------------------------------------ //
// Запустить симуляцию
// ------------------------------------------------------------------------------------ //
void le::Engine::RunSimulation()
{
	LIFEENGINE_ASSERT( window.IsOpen() && studioRender );
	
	if ( !game )
	{
		consoleSystem.PrintError( "The simulation is not running because the game is not loaded" );
		return;
	}

	consoleSystem.PrintInfo( "*** Game logic start ***" );
	
	UInt32_t			startTime = 0;
	UInt32_t			deltaTime = 0;
	Event				event;
	bool				isFocus = true;
	isRunSimulation = true;	

	while ( isRunSimulation )
	{
		// TODO: добавить обработку событий окна

		while ( window.PollEvent( event ) )
		{
			switch ( event.type )
			{
			case Event::ET_WINDOW_CLOSE:
				StopSimulation();
				break;

			case Event::ET_WINDOW_FOCUS_GAINED:
				isFocus = true;
				break;

			case Event::ET_WINDOW_FOCUS_LOST:
				isFocus = false;
				break;

			case Event::ET_WINDOW_RESIZE:
				studioRender->ResizeViewport( 0, 0, event.windowResize.width, event.windowResize.height );
				break;

			default: break;
			}

			game->OnEvent( event );
		}

		if ( isFocus )
		{	
			startTime = SDL_GetTicks();

			game->Update( deltaTime );
			studioRender->RenderFrame();

			deltaTime = ( SDL_GetTicks() - startTime );
		}
	}

	isRunSimulation = false;
	consoleSystem.PrintInfo( "*** Game logic end ***" );
}

// ------------------------------------------------------------------------------------ //
// Остановить симуляцию
// ------------------------------------------------------------------------------------ //
void le::Engine::StopSimulation()
{
	isRunSimulation = false;
}

// ------------------------------------------------------------------------------------ //
// Задать конфигурации движка
// ------------------------------------------------------------------------------------ //
void le::Engine::SetConfig( const Configurations& Configurations )
{
	configurations = Configurations;
}

// ------------------------------------------------------------------------------------ //
// Запущена ли симуляция
// ------------------------------------------------------------------------------------ //
bool le::Engine::IsRunSimulation() const
{
	return isRunSimulation;
}

// ------------------------------------------------------------------------------------ //
// Получить систему консоли
// ------------------------------------------------------------------------------------ //
le::IConsoleSystem* le::Engine::GetConsoleSystem() const
{
	return ( IConsoleSystem* ) &consoleSystem;
}

// ------------------------------------------------------------------------------------ //
// Получить окно
// ------------------------------------------------------------------------------------ //
le::IStudioRender* le::Engine::GetStudioRender() const
{
	return studioRender;
}

// ------------------------------------------------------------------------------------ //
// Получить систему материалов
// ------------------------------------------------------------------------------------ //
le::IMaterialSystem* le::Engine::GetMaterialSystem() const
{
	return materialSystem;
}

// ------------------------------------------------------------------------------------ //
// Получить окно
// ------------------------------------------------------------------------------------ //
le::IResourceSystem* le::Engine::GetResourceSystem() const
{
	return ( IResourceSystem* ) &resourceSystem;
}

// ------------------------------------------------------------------------------------ //
// Получить окно
// ------------------------------------------------------------------------------------ //
le::IWindow* le::Engine::GetWindow() const
{
	return ( IWindow* ) &window;
}

// ------------------------------------------------------------------------------------ //
// Получить фабрику движка
// ------------------------------------------------------------------------------------ //
le::IFactory* le::Engine::GetFactory() const
{
	return ( IFactory* ) &engineFactory;
}

// ------------------------------------------------------------------------------------ //
// Получить конфигурации движка
// ------------------------------------------------------------------------------------ //
const le::Configurations& le::Engine::GetConfigurations() const
{
	return configurations;
}

// ------------------------------------------------------------------------------------ //
// Получить версию движка
// ------------------------------------------------------------------------------------ //
const le::Version& le::Engine::GetVersion() const
{
	return Version( LIFEENGINE_VERSION_MAJOR, LIFEENGINE_VERSION_MINOR, LIFEENGINE_VERSION_PATCH, Engine_BuildNumber() );
}

// ------------------------------------------------------------------------------------ //
// Инициализировать движок
// ------------------------------------------------------------------------------------ //
bool le::Engine::Initialize( WindowHandle_t WindowHandle )
{
	if ( isInit ) return true;
	consoleSystem.PrintInfo( "Initialization lifeEngine" );

	// Выводим системную информацию
	SDL_version		sdlVersion;
	SDL_GetVersion( &sdlVersion );

	consoleSystem.PrintInfo( "*** System info start ****" );
	consoleSystem.PrintInfo( "  Base path: %s", SDL_GetBasePath() );
	consoleSystem.PrintInfo( "" );
	consoleSystem.PrintInfo( "  lifeEngine %i.%i.%i (build %i)", LIFEENGINE_VERSION_MAJOR, LIFEENGINE_VERSION_MINOR, LIFEENGINE_VERSION_PATCH, Engine_BuildNumber() );
	consoleSystem.PrintInfo( "  SDL version: %i.%i.%i", sdlVersion.major, sdlVersion.minor, sdlVersion.patch );
	consoleSystem.PrintInfo( "  Platform: %s", SDL_GetPlatform() );
	consoleSystem.PrintInfo( "  CPU cache L1 size: %i bytes", SDL_GetCPUCacheLineSize() );
	consoleSystem.PrintInfo( "  CPU cores: %i", SDL_GetCPUCount() );
	consoleSystem.PrintInfo( "  RAM: %i MB", SDL_GetSystemRAM() );
	consoleSystem.PrintInfo( "  Has 3DNow: %s", ( SDL_Has3DNow() == SDL_TRUE ? "true" : "false" ) );
	consoleSystem.PrintInfo( "  Has AVX: %s", ( SDL_HasAVX() == SDL_TRUE ? "true" : "false" ) );
	consoleSystem.PrintInfo( "  Has AVX2: %s", ( SDL_HasAVX2() == SDL_TRUE ? "true" : "false" ) );
	consoleSystem.PrintInfo( "  Has AltiVec: %s", ( SDL_HasAltiVec() == SDL_TRUE ? "true" : "false" ) );
	consoleSystem.PrintInfo( "  Has MMX: %s", ( SDL_HasMMX() == SDL_TRUE ? "true" : "false" ) );
	consoleSystem.PrintInfo( "  Has RDTSC: %s", ( SDL_HasRDTSC() == SDL_TRUE ? "true" : "false" ) );
	consoleSystem.PrintInfo( "  Has SSE: %s", ( SDL_HasSSE() == SDL_TRUE ? "true" : "false" ) );
	consoleSystem.PrintInfo( "  Has SSE2: %s", ( SDL_HasSSE2() == SDL_TRUE ? "true" : "false" ) );
	consoleSystem.PrintInfo( "  Has SSE3: %s", ( SDL_HasSSE3() == SDL_TRUE ? "true" : "false" ) );
	consoleSystem.PrintInfo( "  Has SSE41: %s", ( SDL_HasSSE41() == SDL_TRUE ? "true" : "false" ) );
	consoleSystem.PrintInfo( "  Has SSE42: %s", ( SDL_HasSSE42() == SDL_TRUE ? "true" : "false" ) );
	consoleSystem.PrintInfo( "*** System info end ****" );

	try
	{
		// Инициализируем окно приложения. Если заголовок на окно в аргументах пуст, 
		// то создаем свое окно, иначе запоминаем его

		if ( !WindowHandle )
		{
			if ( !window.Create( "lifeEngine", configurations.windowWidth, configurations.windowHeight, configurations.isFullscreen ? SW_FULLSCREEN : SW_DEFAULT ) )
				throw std::exception( SDL_GetError() );
		}
		else 
			window.SetHandle( WindowHandle );

		// Загружаем и инициализируем подсистемы

		if ( !LoadModule_StudioRender( LIFEENGINE_STUDIORENDER_DLL ) )		throw std::exception( "Failed loading studiorender" );
		if ( !LoadModule_MaterialSystem( LIFEENGINE_MATERIALSYSTEM_DLL ) )	throw std::exception( "Failed loading materialsystem" );

		if ( !materialSystem->LoadShaderDLL( LIFEENGINE_STDSHADERS_DLL ) )	throw std::exception( "Failed loading stdshaders" );

		resourceSystem.Initialize( this );
	}
	catch ( const std::exception& Exception )
	{
		if ( g_criticalError )		g_criticalError( Exception.what() );
		return false;
	}

	consoleSystem.PrintInfo( "Initialized lifeEngine" );
	isInit = true;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Загрузить игру
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadGame( const char* DirGame )
{
	// Загружаем информацию об игре
	if ( !LoadGameInfo( DirGame ) )	
		return false;

	// Задаем каталог игры для загрузки ресурсов
	resourceSystem.SetGameDir( gameInfo.gameDir );

	// Загружаем игровую логику
	if ( !LoadModule_Game( ( std::string( DirGame ) + "/" + gameInfo.gameDLL ).c_str() ) )
		return false;

	// Если есть иконка у игры - грузим ее
	if ( gameInfo.icon )
	{
		bool		isError = false;
		Image		image = resourceSystem.LoadImage( gameInfo.icon, isError, true, false );
		
		if ( !isError )
		{
			window.SetIcon( image );
			resourceSystem.UnloadImage( image );
		}
	}

	window.SetTitle( gameInfo.title );
	return true;
}

// ------------------------------------------------------------------------------------ //
// Выгрузить игру
// ------------------------------------------------------------------------------------ //
void le::Engine::UnloadGame()
{
	StopSimulation();

	gameInfo.Clear();
	resourceSystem.SetGameDir( "" );

	if ( game ) UnloadModule_Game();
}