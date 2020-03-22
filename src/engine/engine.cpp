//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Р”РІРёРіР°С‚РµР»СЊ Р¶РёР·РЅРё) ***
//				Copyright (C) 2018-2019
//
// Р РµРїРѕР·РёС‚РѕСЂРёР№ РґРІРёР¶РєР°:  https://github.com/zombihello/lifeEngine
// РђРІС‚РѕСЂС‹:				Р•РіРѕСЂ РџРѕРіСѓР»СЏРєР° (zombiHello)
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
#include "engine/fontfreetype.h"
#include "studiorender/istudiorenderinternal.h"
#include "studiorender/studiorenderviewport.h"
#include "studiorender/ishadermanager.h"
#include "physics/iphysicssysteminternal.h"

#define	FIXED_TIME_UPDATE		( 1 / 60.f )

LIFEENGINE_ENGINE_API( le::Engine );

// ------------------------------------------------------------------------------------ //
// РљРѕРЅСЃРѕР»СЊРЅР°СЏ РєРѕРјР°РЅРґР° РІС‹С…РѕРґР°
// ------------------------------------------------------------------------------------ //
void CMD_Exit( le::UInt32_t CountArguments, const char** Arguments )
{
	if ( !le::g_engine ) return;
	le::g_engine->StopSimulation();
	exit( 0 );
}

// ------------------------------------------------------------------------------------ //
// РљРѕРЅСЃРѕР»СЊРЅР°СЏ РєРѕРјР°РЅРґР° РІС‹РІРѕРґР° РІРµСЂСЃРёРё РґРІРёР¶РєР°
// ------------------------------------------------------------------------------------ //
void CMD_Version( le::UInt32_t CountArguments, const char** Arguments )
{
	if ( !le::g_consoleSystem ) return;
	le::g_consoleSystem->PrintInfo( "lifeEngine %s (build %i)", LIFEENGINE_VERSION, le::Engine_BuildNumber() );
}

// ------------------------------------------------------------------------------------ //
// РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ
// ------------------------------------------------------------------------------------ //
le::Engine::Engine() :
	isRunSimulation( false ),
	isInit( false ),
	studioRender( nullptr ),
	studioRenderDescriptor( { nullptr, nullptr, nullptr, nullptr } ),
    physicSystem( nullptr ),
    physicSystemDescriptor( { nullptr, nullptr, nullptr, nullptr } ),
	game( nullptr ),
	gameDescriptor( { nullptr, nullptr, nullptr, nullptr } ),
	criticalError( nullptr ),
	cmd_Exit( new ConCmd() ),
	cmd_Version( new ConCmd() ),
	deltaTime( 0.f )
{
	LIFEENGINE_ASSERT( !g_engine );

	g_consoleSystem = &consoleSystem;
	g_resourceSystem = &resourceSystem;
	g_inputSystem = &inputSystem;
	g_engine = this;

	configurations.fov = 75.f;
	configurations.isFullscreen = false;
	configurations.isVerticalSinc = false;
	configurations.sensitivityMouse = 0.15f;
	configurations.windowWidth = 800;
	configurations.windowHeight = 600;

	consoleSystem.Initialize();
	inputSystem.Initialize( this );
	cmd_Exit->Initialize( "exit", "close game", CMD_Exit );
	cmd_Version->Initialize( "version", "show version engine", CMD_Version );

	consoleSystem.RegisterCommand( cmd_Exit );
	consoleSystem.RegisterCommand( cmd_Version );
}

// ------------------------------------------------------------------------------------ //
// Р”РµСЃС‚СЂСѓРєС‚РѕСЂ
// ------------------------------------------------------------------------------------ //
le::Engine::~Engine()
{
	if ( game )				UnloadModule_Game();

	resourceSystem.UnloadAll();

    if ( physicSystem )     UnloadModule_PhysicsSystem();
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

	FontFreeType::UninitializeFreeType();
}

// ------------------------------------------------------------------------------------ //
// Р—Р°РіСЂСѓР·РёС‚СЊ РјРѕРґСѓР»СЊ СЂРµРЅРґРµСЂР°
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadModule_StudioRender( const char* PathDLL )
{
	// Р•СЃР»Рё РјРѕРґСѓР»СЊ СЂР°РЅРµРµ Р±С‹Р» Р·Р°РіСЂСѓР¶РµРЅ, С‚Рѕ СѓРґР°Р»СЏРµРј
	if ( studioRenderDescriptor.handle ) UnloadModule_StudioRender();

	consoleSystem.PrintInfo( "Loading studiorender [%s]", PathDLL );

	try
	{
		// Р—Р°РіСЂСѓР¶Р°РµРј РјРѕРґСѓР»СЊ
		studioRenderDescriptor.handle = SDL_LoadObject( PathDLL );
        if ( !studioRenderDescriptor.handle )	throw std::runtime_error( SDL_GetError() );

		// Р‘РµСЂРµРј РёР· РјРѕРґСѓР»СЏ API РґР»СЏ СЂР°Р±РѕС‚С‹ СЃ РЅРёРј
		studioRenderDescriptor.LE_CreateStudioRender = ( LE_CreateStudioRenderFn_t ) SDL_LoadFunction( studioRenderDescriptor.handle, "LE_CreateStudioRender" );
		studioRenderDescriptor.LE_DeleteStudioRender = ( LE_DeleteStudioRenderFn_t ) SDL_LoadFunction( studioRenderDescriptor.handle, "LE_DeleteStudioRender" );
		studioRenderDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( studioRenderDescriptor.handle, "LE_SetCriticalError" );
        if ( !studioRenderDescriptor.LE_CreateStudioRender )	throw std::runtime_error( "Function LE_CreateStudioRender not found" );

		// РЎРѕР·РґР°РµРј СЂРµРЅРґРµСЂ
		if ( studioRenderDescriptor.LE_SetCriticalError )
			studioRenderDescriptor.LE_SetCriticalError( g_criticalError );

		studioRender = ( IStudioRenderInternal* ) studioRenderDescriptor.LE_CreateStudioRender();
        if ( !studioRender->Initialize( this ) )				throw std::runtime_error( "Fail initialize studiorender" );
		g_studioRender = studioRender;
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
// Р’С‹РіСЂСѓР·РёС‚СЊ РјРѕРґСѓР»СЊ СЂРµРЅРґРµСЂР°
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
// Load module physics system
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadModule_PhysicsSystem( const char *PathDLL )
{
    // Р•СЃР»Рё РјРѕРґСѓР»СЊ СЂР°РЅРµРµ Р±С‹Р» Р·Р°РіСЂСѓР¶РµРЅ, С‚Рѕ СѓРґР°Р»СЏРµРј
    if ( physicSystemDescriptor.handle ) UnloadModule_PhysicsSystem();

    consoleSystem.PrintInfo( "Loading physics system [%s]", PathDLL );

    try
    {
        // Р—Р°РіСЂСѓР¶Р°РµРј РјРѕРґСѓР»СЊ
        physicSystemDescriptor.handle = SDL_LoadObject( PathDLL );
        if ( !physicSystemDescriptor.handle )	throw std::runtime_error( SDL_GetError() );

        // Р‘РµСЂРµРј РёР· РјРѕРґСѓР»СЏ API РґР»СЏ СЂР°Р±РѕС‚С‹ СЃ РЅРёРј
        physicSystemDescriptor.LE_CreatePhysicsSystem = ( LE_CreatePhysicsSystemFn_t ) SDL_LoadFunction( physicSystemDescriptor.handle, "LE_CreatePhysicsSystem" );
        physicSystemDescriptor.LE_DeletePhysicsSystem = ( LE_DeletePhysicsSystemFn_t ) SDL_LoadFunction( physicSystemDescriptor.handle, "LE_DeletePhysicsSystem" );
        physicSystemDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( studioRenderDescriptor.handle, "LE_SetCriticalError" );
        if ( !physicSystemDescriptor.LE_CreatePhysicsSystem )	throw std::runtime_error( "Function LE_CreatePhysicsSystem not found" );

        // РЎРѕР·РґР°РµРј СЂРµРЅРґРµСЂ
        if ( physicSystemDescriptor.LE_SetCriticalError )
            physicSystemDescriptor.LE_SetCriticalError( g_criticalError );

		physicSystem = ( IPhysicsSystemInternal* ) physicSystemDescriptor.LE_CreatePhysicsSystem();
        if ( !physicSystem->Initialize( this ) )	throw std::runtime_error( "Fail initialize physics system" );
		g_physicsSystem = physicSystem;
    }
    catch ( std::exception& Exception )
    {
        consoleSystem.PrintError( Exception.what() );
        return false;
    }

    consoleSystem.PrintInfo( "Loaded physics system [%s]", PathDLL );
    return true;
}

// ------------------------------------------------------------------------------------ //
// Unload module physics system
// ------------------------------------------------------------------------------------ //
void le::Engine::UnloadModule_PhysicsSystem()
{
    if ( !physicSystem ) return;

    if ( physicSystemDescriptor.LE_DeletePhysicsSystem )
        physicSystemDescriptor.LE_DeletePhysicsSystem( physicSystem );

    SDL_UnloadObject( physicSystemDescriptor.handle );
    physicSystem = nullptr;
	g_physicsSystem = nullptr;
    physicSystemDescriptor = { nullptr, nullptr, nullptr, nullptr };

    consoleSystem.PrintInfo( "Unloaded physics system" );
}

// ------------------------------------------------------------------------------------ //
// Р—Р°РіСЂСѓР·РёС‚СЊ РёРЅС„РѕСЂРјР°С†РёСЋ РѕР± РёРіСЂРµ
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
		// РџСѓС‚СЊ Рє РјРѕРґСѓР»СЋ СЃ РёРіСЂРѕРІРѕР№ Р»РѕРіРёРєРѕР№
		if ( strcmp( it->name.GetString(), "gameDLL" ) == 0 && it->value.IsString() )
		{
            UInt32_t            sizeExnesion;
            UInt32_t            sizeJsonString = it->value.GetStringLength();
            const char*         extension;

#       if defined( PLATFORM_WINDOWS )
            extension = ".dll";
            sizeExnesion = 4;
#       elif defined( PLATFORM_LINUX )
            extension = ".so";
            sizeExnesion = 3;
#       endif // PLATFORM_WINDOWS

            stringLength = sizeJsonString + sizeExnesion;

            gameInfo.gameDLL = new char[ stringLength + 1 ];
            memcpy( gameInfo.gameDLL, it->value.GetString(), sizeJsonString );
            memcpy( gameInfo.gameDLL + sizeJsonString, extension, sizeExnesion );
            gameInfo.gameDLL[ stringLength ] = '\0';
		}

		// РџСѓС‚СЊ Рє РёРєРѕРЅРєРµ
		if ( strcmp( it->name.GetString(), "icon" ) == 0 && it->value.IsString() )
		{
			stringLength = it->value.GetStringLength();

			gameInfo.icon = new char[ stringLength + 1 ];
			memcpy( gameInfo.icon, it->value.GetString(), stringLength );
			gameInfo.icon[ stringLength ] = '\0';
		}

		// Р—Р°РіРѕР»РѕРІРѕРє РёРіСЂС‹	
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
// Р—Р°РіСЂСѓР·РёС‚СЊ РјРѕРґСѓР»СЊ РёРіСЂС‹
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadModule_Game( const char* PathDLL )
{
	// Р•СЃР»Рё РјРѕРґСѓР»СЊ СЂР°РЅРµРµ Р±С‹Р» Р·Р°РіСЂСѓР¶РµРЅ, С‚Рѕ СѓРґР°Р»СЏРµРј
	if ( gameDescriptor.handle ) UnloadModule_Game();

	consoleSystem.PrintInfo( "Loading game [%s]", PathDLL );

	try
	{
		// Р—Р°РіСЂСѓР¶Р°РµРј РјРѕРґСѓР»СЊ
		gameDescriptor.handle = SDL_LoadObject( PathDLL );
        if ( !gameDescriptor.handle )	throw std::runtime_error( SDL_GetError() );

		// Р‘РµСЂРµРј РёР· РјРѕРґСѓР»СЏ API РґР»СЏ СЂР°Р±РѕС‚С‹ СЃ РЅРёРј
		gameDescriptor.LE_CreateGame = ( LE_CreateGameFn_t ) SDL_LoadFunction( gameDescriptor.handle, "LE_CreateGame" );
		gameDescriptor.LE_DeleteGame = ( LE_DeleteGameFn_t ) SDL_LoadFunction( gameDescriptor.handle, "LE_DeleteGame" );
		gameDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( gameDescriptor.handle, "LE_SetCriticalError" );
        if ( !gameDescriptor.LE_CreateGame )	throw std::runtime_error( "Function LE_CreateGame not found" );

		// РЎРѕР·РґР°РµРј РёРіСЂРѕРІСѓСЋ Р»РѕРіРёРєСѓ
		if ( gameDescriptor.LE_SetCriticalError )
			gameDescriptor.LE_SetCriticalError( g_criticalError );

		game = gameDescriptor.LE_CreateGame();
        if ( !game->Initialize( this ) )						throw std::runtime_error( "Fail initialize game" );
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
// Р’С‹РіСЂСѓР·РёС‚СЊ РјРѕРґСѓР»СЊ РёРіСЂС‹
// ------------------------------------------------------------------------------------ //
void le::Engine::UnloadModule_Game()
{
	if ( !game ) return;

    if ( game && gameDescriptor.LE_DeleteGame )
		gameDescriptor.LE_DeleteGame( game );

	SDL_UnloadObject( gameDescriptor.handle );
	game = nullptr;
	gameDescriptor = { nullptr, nullptr, nullptr, nullptr };
	
	consoleSystem.PrintInfo( "Unloaded game" );
}

// ------------------------------------------------------------------------------------ //
// Р—Р°РіСЂСѓР·РёС‚СЊ РєРѕРЅС„РёРі
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
			// РћР±С‰РёРё РїР°СЂР°РјРµС‚СЂС‹
			if ( strcmp( it->name.GetString(), "general" ) == 0 )
			{
				// РЁРёСЂРёРЅР° РѕРєРЅР°
				if ( strcmp( itObject->name.GetString(), "width" ) == 0 && itObject->value.IsNumber() )
					configurations.windowWidth = itObject->value.GetInt();

				// Р’С‹СЃРѕС‚Р° РѕРєРЅР°
				else if ( strcmp( itObject->name.GetString(), "height" ) == 0 && itObject->value.IsNumber() )
					configurations.windowHeight = itObject->value.GetInt();

				// РџРѕР»РЅРѕСЌРєСЂР°РЅС‹Р№ Р»Рё СЂРµР¶РёРј
				else if ( strcmp( itObject->name.GetString(), "fullscreen" ) == 0 && itObject->value.IsBool() )
					configurations.isFullscreen = itObject->value.GetBool();

				// Р§СѓРІСЃС‚РІРёС‚РµР»СЊРЅРѕСЃС‚СЊ РјС‹С€РєРё
				else if ( strcmp( itObject->name.GetString(), "sensitivityMouse" ) == 0 && itObject->value.IsNumber() )
					configurations.sensitivityMouse = itObject->value.GetFloat();
			}

			// РџР°СЂР°РјРµС‚СЂС‹ СЂРµРЅРґРµСЂР°
			else if ( strcmp( it->name.GetString(), "studiorender" ) == 0 )
			{
				// Р’РєР»СЋС‡РµРЅР° Р»Рё РІРµСЂС‚РёРєР°Р»СЊРЅР°СЏ СЃРёРЅС…СЂРѕРЅРёР·Р°С†РёСЏ
				if ( strcmp( itObject->name.GetString(), "vsinc" ) == 0 && itObject->value.IsBool() )
					configurations.isVerticalSinc = itObject->value.GetBool();

				// Р’РєР»СЋС‡РµРЅР° Р»Рё РІРµСЂС‚РёРєР°Р»СЊРЅР°СЏ СЃРёРЅС…СЂРѕРЅРёР·Р°С†РёСЏ
				else if ( strcmp( itObject->name.GetString(), "fov" ) == 0 && itObject->value.IsNumber() )
					configurations.fov = itObject->value.GetFloat();
			}
		}
	}

	consoleSystem.PrintInfo( "Loaded file configurations [%s]", FilePath );
	return true;
}

// ------------------------------------------------------------------------------------ //
// РЎРѕС…СЂР°РЅРёС‚СЊ РєРѕРЅС„РёРі
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
		\"fullscreen\" : " << ( configurations.isFullscreen ? "true" : "false" ) << ",\n\
        \"sensitivityMouse\" : " << configurations.sensitivityMouse << "\n\
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
// Р—Р°РїСѓСЃС‚РёС‚СЊ СЃРёРјСѓР»СЏС†РёСЋ
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
	
    float                   currentTick = 0.f;
    float                   lastTick = 0.f;
	float                   delayFrame = 0.f;
    Event                   event;
    bool                    isFocus = true;

    isRunSimulation = true;

	while ( isRunSimulation )
	{
        currentTick = SDL_GetTicks();
        deltaTime = ( currentTick - lastTick ) / 1000.f;
        lastTick = currentTick;

        inputSystem.Clear();

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

			case Event::ET_KEY_PRESSED:
			case Event::ET_KEY_RELEASED:
			case Event::ET_MOUSE_MOVE:
			case Event::ET_MOUSE_PRESSED:
			case Event::ET_MOUSE_RELEASED:
			case Event::ET_MOUSE_WHEEL:
			case Event::ET_TEXT_INPUT:
				inputSystem.ApplyEvent( event );
				break;

			case Event::ET_WINDOW_RESIZE:
                studioRender->SetViewport( { 0, 0, ( UInt32_t ) event.events.windowResize.width, ( UInt32_t ) event.events.windowResize.height } );
				break;

			default: break;
			}

			game->OnEvent( event );
		}

		if ( isFocus )
        {
            delayFrame += deltaTime;
			while ( delayFrame >= FIXED_TIME_UPDATE )
            {
				delayFrame -= FIXED_TIME_UPDATE;

				inputSystem.Update();
				game->Update();
                materialManager.UpdateProxes();
            }

			studioRender->Begin();
			physicSystem->Update();
            game->Render();
            studioRender->End();
            studioRender->Present();
		}     
	}

	StopSimulation();
}

// ------------------------------------------------------------------------------------ //
// РћСЃС‚Р°РЅРѕРІРёС‚СЊ СЃРёРјСѓР»СЏС†РёСЋ
// ------------------------------------------------------------------------------------ //
void le::Engine::StopSimulation()
{
    consoleSystem.PrintInfo( "*** Game logic end ***" );

    if ( game && gameDescriptor.LE_DeleteGame )     gameDescriptor.LE_DeleteGame( game );
    resourceSystem.UnloadAll();
    isRunSimulation = false;
}

// ------------------------------------------------------------------------------------ //
// Р—Р°РґР°С‚СЊ РєРѕРЅС„РёРіСѓСЂР°С†РёРё РґРІРёР¶РєР°
// ------------------------------------------------------------------------------------ //
void le::Engine::SetConfig( const Configurations& Configurations )
{
	configurations = Configurations;
}

// ------------------------------------------------------------------------------------ //
// Р—Р°РїСѓС‰РµРЅР° Р»Рё СЃРёРјСѓР»СЏС†РёСЏ
// ------------------------------------------------------------------------------------ //
bool le::Engine::IsRunSimulation() const
{
	return isRunSimulation;
}

// ------------------------------------------------------------------------------------ //
// РџРѕР»СѓС‡РёС‚СЊ СЃРёСЃС‚РµРјСѓ РєРѕРЅСЃРѕР»Рё
// ------------------------------------------------------------------------------------ //
le::IConsoleSystem* le::Engine::GetConsoleSystem() const
{
	return ( IConsoleSystem* ) &consoleSystem;
}

// ------------------------------------------------------------------------------------ //
// РџРѕР»СѓС‡РёС‚СЊ РѕРєРЅРѕ
// ------------------------------------------------------------------------------------ //
le::IStudioRender* le::Engine::GetStudioRender() const
{
	return studioRender;
}

// ------------------------------------------------------------------------------------ //
// РџРѕР»СѓС‡РёС‚СЊ РѕРєРЅРѕ
// ------------------------------------------------------------------------------------ //
le::IResourceSystem* le::Engine::GetResourceSystem() const
{
	return ( IResourceSystem* ) &resourceSystem;
}

// ------------------------------------------------------------------------------------ //
// РџРѕР»СѓС‡РёС‚СЊ СЃРёСЃС‚РµРјСѓ РІРІРѕРґР°
// ------------------------------------------------------------------------------------ //
le::IInputSystem* le::Engine::GetInputSystem() const
{
    return ( IInputSystem* ) &inputSystem;
}

// ------------------------------------------------------------------------------------ //
// Return material manager
// ------------------------------------------------------------------------------------ //
le::IMaterialManager* le::Engine::GetMaterialManager() const
{
    return ( IMaterialManager* ) &materialManager;
}

// ------------------------------------------------------------------------------------ //
// Return physics system
// ------------------------------------------------------------------------------------ //
le::IPhysicsSystem *le::Engine::GetPhysicsSystem() const
{
    return physicSystem;
}

// ------------------------------------------------------------------------------------ //
// РџРѕР»СѓС‡РёС‚СЊ РѕРєРЅРѕ
// ------------------------------------------------------------------------------------ //
le::IWindow* le::Engine::GetWindow() const
{
	return ( IWindow* ) &window;
}

// ------------------------------------------------------------------------------------ //
// РџРѕР»СѓС‡РёС‚СЊ С„Р°Р±СЂРёРєСѓ РґРІРёР¶РєР°
// ------------------------------------------------------------------------------------ //
le::IFactory* le::Engine::GetFactory() const
{
	return ( IFactory* ) &engineFactory;
}

// ------------------------------------------------------------------------------------ //
// РџРѕР»СѓС‡РёС‚СЊ РєРѕРЅС„РёРіСѓСЂР°С†РёРё РґРІРёР¶РєР°
// ------------------------------------------------------------------------------------ //
const le::Configurations& le::Engine::GetConfigurations() const
{
    return configurations;
}

// ------------------------------------------------------------------------------------ //
// Get game info
// ------------------------------------------------------------------------------------ //
const le::GameInfo& le::Engine::GetGameInfo() const
{
    return gameInfo;
}

// ------------------------------------------------------------------------------------ //
// РџРѕР»СѓС‡РёС‚СЊ РІРµСЂСЃРёСЋ РґРІРёР¶РєР°
// ------------------------------------------------------------------------------------ //
const le::Version& le::Engine::GetVersion() const
{
	return Version( LIFEENGINE_VERSION, Engine_BuildNumber() );
}

// ------------------------------------------------------------------------------------ //
// Р�РЅРёС†РёР°Р»РёР·РёСЂРѕРІР°С‚СЊ РґРІРёР¶РѕРє
// ------------------------------------------------------------------------------------ //
bool le::Engine::Initialize( const char* EngineDirectory, WindowHandle_t WindowHandle )
{
	if ( isInit ) return true;
	consoleSystem.PrintInfo( "Initialization lifeEngine" );

	// Р’С‹РІРѕРґРёРј СЃРёСЃС‚РµРјРЅСѓСЋ РёРЅС„РѕСЂРјР°С†РёСЋ
	SDL_version		sdlVersion;
	SDL_GetVersion( &sdlVersion );

	consoleSystem.PrintInfo( "*** System info start ****" );
	consoleSystem.PrintInfo( "  Base path: %s", SDL_GetBasePath() );
	consoleSystem.PrintInfo( "" );
	consoleSystem.PrintInfo( "  lifeEngine %s (build %i)", LIFEENGINE_VERSION, Engine_BuildNumber() );
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
		// Р�РЅРёС†РёР°Р»РёР·РёСЂСѓРµРј РѕРєРЅРѕ РїСЂРёР»РѕР¶РµРЅРёСЏ. Р•СЃР»Рё Р·Р°РіРѕР»РѕРІРѕРє РЅР° РѕРєРЅРѕ РІ Р°СЂРіСѓРјРµРЅС‚Р°С… РїСѓСЃС‚, 
		// С‚Рѕ СЃРѕР·РґР°РµРј СЃРІРѕРµ РѕРєРЅРѕ, РёРЅР°С‡Рµ Р·Р°РїРѕРјРёРЅР°РµРј РµРіРѕ

		if ( !WindowHandle )
		{
			if ( !window.Create( "lifeEngine", configurations.windowWidth, configurations.windowHeight, configurations.isFullscreen ? SW_FULLSCREEN : SW_DEFAULT ) )
                throw std::runtime_error( SDL_GetError() );
		}
		else 
			window.SetHandle( WindowHandle );

		// Р—Р°РіСЂСѓР¶Р°РµРј Рё РёРЅРёС†РёР°Р»РёР·РёСЂСѓРµРј РїРѕРґСЃРёСЃС‚РµРјС‹
        std::string         engineDir = EngineDirectory;
        if ( !LoadModule_StudioRender( ( engineDir + "/" + LIFEENGINE_STUDIORENDER_DLL ).c_str() ) )        throw std::runtime_error( "Failed loading studiorender" );
        if ( !LoadModule_PhysicsSystem( ( engineDir + "/" + LIFEENGINE_PHYSICSSYSTEM_DLL ).c_str() ) )      throw std::runtime_error( "Failed loading physics system" );

		auto*		shaderManager = studioRender->GetShaderManager();
        if ( !shaderManager )		throw std::runtime_error( "In studiorender not exist shader manager" );
        if ( !shaderManager->LoadShaderDLL( ( engineDir + "/" + LIFEENGINE_STDSHADERS_DLL ).c_str() ) )   throw std::runtime_error( "Failed loading stdshaders" );

		FontFreeType::InitializeFreeType();
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
// Р—Р°РіСЂСѓР·РёС‚СЊ РёРіСЂСѓ
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadGame( const char* DirGame )
{
	// Р—Р°РіСЂСѓР¶Р°РµРј РёРЅС„РѕСЂРјР°С†РёСЋ РѕР± РёРіСЂРµ
	if ( !LoadGameInfo( DirGame ) )	
		return false;

	// Р—Р°РґР°РµРј РєР°С‚Р°Р»РѕРі РёРіСЂС‹ РґР»СЏ Р·Р°РіСЂСѓР·РєРё СЂРµСЃСѓСЂСЃРѕРІ
	resourceSystem.SetGameDir( gameInfo.gameDir );

	// Р—Р°РіСЂСѓР¶Р°РµРј РёРіСЂРѕРІСѓСЋ Р»РѕРіРёРєСѓ
    if ( !LoadModule_Game( ( std::string( DirGame ) + "/" + gameInfo.gameDLL ).c_str() ) )
		return false;

	// Р•СЃР»Рё РµСЃС‚СЊ РёРєРѕРЅРєР° Сѓ РёРіСЂС‹ - РіСЂСѓР·РёРј РµРµ
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
// Р’С‹РіСЂСѓР·РёС‚СЊ РёРіСЂСѓ
// ------------------------------------------------------------------------------------ //
void le::Engine::UnloadGame()
{
	StopSimulation();

	gameInfo.Clear();
	resourceSystem.SetGameDir( "" );

	if ( game ) UnloadModule_Game();
}

// ------------------------------------------------------------------------------------ //
// Get delta time
// ------------------------------------------------------------------------------------ //
float le::Engine::GetDeltaTime() const
{
	return deltaTime;
}

// ------------------------------------------------------------------------------------ //
// Get rate game update
// ------------------------------------------------------------------------------------ //
float le::Engine::GetRateGameUpdate() const
{
	return FIXED_TIME_UPDATE;
}
