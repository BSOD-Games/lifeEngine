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
#include "engine/resourcesystem.h"
#include "studiorender/istudiorenderinternal.h"
#include "studiorender/studiorenderviewport.h"
#include "physics/iphysicssysteminternal.h"
#include "audio/iaudiosysteminternal.h"

#define	FIXED_TIME_UPDATE		( 1 / 60.f )

LIFEENGINE_ENGINE_API( le::Engine );

namespace le
{
	// ------------------------------------------------------------------------------------ //
	// Callback: window resize
	// ------------------------------------------------------------------------------------ //
	void ChangeCallback_WindowResize( IConVar* ConVar )
	{
		if ( !g_engine->studioRender ) return;

		g_engine->window.SetSize( g_engine->cvar_windowWidth->GetValueInt(), g_engine->cvar_windowHeight->GetValueInt() );
		
		StudioRenderViewport		viewport = g_engine->studioRender->GetViewport();
		viewport.width = ( UInt32_t ) g_engine->cvar_windowWidth->GetValueInt();
		viewport.height = ( UInt32_t ) g_engine->cvar_windowHeight->GetValueInt();
		g_engine->studioRender->SetViewport( viewport );
	}

	// ------------------------------------------------------------------------------------ //
	// Callback: window fullscreen
	// ------------------------------------------------------------------------------------ //
	void ChangeCallback_WindowFullscreen( IConVar* ConVar )
	{
		g_engine->window.SetFullscreen( ConVar->GetValueBool() );
	}

	// ------------------------------------------------------------------------------------ //
	// Callback: enable vsinc
	// ------------------------------------------------------------------------------------ //
	void ChangeCallback_EnableVSinc( IConVar* ConVar )
	{
		if ( !g_engine->studioRender ) return;
		g_engine->studioRender->SetVerticalSyncEnabled( ConVar->GetValueBool() );
	}
}

// ------------------------------------------------------------------------------------ //
// РљРѕРЅСЃРѕР»СЊРЅР°СЏ РєРѕРјР°РЅРґР° РІС‹С…РѕРґР°
// ------------------------------------------------------------------------------------ //
void CMD_Exit( le::UInt32_t CountArguments, const char** Arguments )
{
	if ( !le::g_engine ) return;
	le::g_engine->StopSimulation();
}

// ------------------------------------------------------------------------------------ //
// РљРѕРЅСЃРѕР»СЊРЅР°СЏ РєРѕРјР°РЅРґР° РІС‹РІРѕРґР° РІРµСЂСЃРёРё РґРІРёР¶РєР°
// ------------------------------------------------------------------------------------ //
void CMD_Version( le::UInt32_t CountArguments, const char** Arguments )
{
	if ( !le::g_consoleSystem ) return;
	le::g_consoleSystem->PrintInfo( "lifeEngine %s", LIFEENGINE_VERSION );
}

// ------------------------------------------------------------------------------------ //
// РљРѕРЅСЃС‚СЂСѓРєС‚РѕСЂ
// ------------------------------------------------------------------------------------ //
le::Engine::Engine() :
	isRunSimulation( false ),
	isInitialized( false ),
	isEditor( false ),
	studioRender( nullptr ),
	studioRenderDescriptor( { nullptr, nullptr, nullptr, nullptr } ),
	physicSystem( nullptr ),
	physicSystemDescriptor( { nullptr, nullptr, nullptr, nullptr } ),
	audioSystem( nullptr ),
	audioSystemDescriptor( { nullptr, nullptr, nullptr, nullptr } ),
	criticalError( nullptr ),
	cmd_Exit( new ConCmd() ),
	cmd_Version( new ConCmd() ),
	cvar_phyDebug( new ConVar() ),
	cvar_windowWidth( new ConVar() ),
	cvar_windowHeight( new ConVar() ),
	cvar_windowFullscreen( new ConVar() ),
	cvar_mouseSensitivity( new ConVar() ),
	cvar_rvsinc( new ConVar() )
{
	LIFEENGINE_ASSERT( !g_engine );

	g_consoleSystem = &consoleSystem;
	g_resourceSystem = &resourceSystem;
	g_inputSystem = &inputSystem;
	g_scriptSystem = &scriptSystem;
	g_window = &window;
	g_engine = this;
	g_engineFactory = &engineFactory;
	g_materialSystem = &materialSystem;
}

// ------------------------------------------------------------------------------------ //
// Р”РµСЃС‚СЂСѓРєС‚РѕСЂ
// ------------------------------------------------------------------------------------ //
le::Engine::~Engine()
{
	resourceSystem.UnloadAll();

	if ( physicSystem )     UnloadModule_PhysicsSystem();
	if ( studioRender )		UnloadModule_StudioRender();
	if ( audioSystem )		UnloadModule_AudioSystem();

	if ( window.IsOpen() )	window.Close();

	if ( cmd_Exit )			consoleSystem.UnregisterCommand( cmd_Exit->GetName() );
	if ( cmd_Version )		consoleSystem.UnregisterCommand( cmd_Version->GetName() );
	if ( cvar_phyDebug )	consoleSystem.UnregisterVar( cvar_phyDebug->GetName() );
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
		g_studioRender = studioRender;
		g_studioRenderFactory = studioRender->GetFactory();
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
	g_studioRender = nullptr;
	g_studioRenderFactory = nullptr;

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
		physicSystemDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( physicSystemDescriptor.handle, "LE_SetCriticalError" );
		if ( !physicSystemDescriptor.LE_CreatePhysicsSystem )	throw std::runtime_error( "Function LE_CreatePhysicsSystem not found" );

		// РЎРѕР·РґР°РµРј СЂРµРЅРґРµСЂ
		if ( physicSystemDescriptor.LE_SetCriticalError )
			physicSystemDescriptor.LE_SetCriticalError( g_criticalError );

		physicSystem = ( IPhysicsSystemInternal* ) physicSystemDescriptor.LE_CreatePhysicsSystem();
		g_physicsSystem = physicSystem;
		g_physicsSystemFactory = physicSystem->GetFactory();
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
// Load module audio system
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadModule_AudioSystem( const char* PathDLL )
{
	// Р•СЃР»Рё РјРѕРґСѓР»СЊ СЂР°РЅРµРµ Р±С‹Р» Р·Р°РіСЂСѓР¶РµРЅ, С‚Рѕ СѓРґР°Р»СЏРµРј
	if ( audioSystemDescriptor.handle ) UnloadModule_AudioSystem();

	consoleSystem.PrintInfo( "Loading audio system [%s]", PathDLL );

	try
	{
		// Р—Р°РіСЂСѓР¶Р°РµРј РјРѕРґСѓР»СЊ
		audioSystemDescriptor.handle = SDL_LoadObject( PathDLL );
		if ( !audioSystemDescriptor.handle )	throw std::runtime_error( SDL_GetError() );

		// Р‘РµСЂРµРј РёР· РјРѕРґСѓР»СЏ API РґР»СЏ СЂР°Р±РѕС‚С‹ СЃ РЅРёРј
		audioSystemDescriptor.LE_CreateAudioSystem = ( LE_CreateAudioSystemFn_t ) SDL_LoadFunction( audioSystemDescriptor.handle, "LE_CreateAudioSystem" );
		audioSystemDescriptor.LE_DeleteAudioSystem = ( LE_DeleteAudioSystemFn_t ) SDL_LoadFunction( audioSystemDescriptor.handle, "LE_DeleteAudioSystem" );
		audioSystemDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( audioSystemDescriptor.handle, "LE_SetCriticalError" );
		if ( !audioSystemDescriptor.LE_CreateAudioSystem )	throw std::runtime_error( "Function LE_CreateAudioSystem not found" );

		// РЎРѕР·РґР°РµРј СЂРµРЅРґРµСЂ
		if ( audioSystemDescriptor.LE_SetCriticalError )
			audioSystemDescriptor.LE_SetCriticalError( g_criticalError );

		audioSystem = ( IAudioSystemInternal* ) audioSystemDescriptor.LE_CreateAudioSystem();
	}
	catch ( std::exception& Exception )
	{
		consoleSystem.PrintError( Exception.what() );
		return false;
	}

	consoleSystem.PrintInfo( "Loaded audio system [%s]", PathDLL );
	return true;
}

// ------------------------------------------------------------------------------------ //
// Unload module audio system
// ------------------------------------------------------------------------------------ //
void le::Engine::UnloadModule_AudioSystem()
{
	if ( !audioSystem ) return;

	if ( audioSystemDescriptor.LE_DeleteAudioSystem )
		audioSystemDescriptor.LE_DeleteAudioSystem( audioSystem );

	SDL_UnloadObject( audioSystemDescriptor.handle );
	audioSystem = nullptr;
	audioSystemDescriptor = { nullptr, nullptr, nullptr, nullptr };

	consoleSystem.PrintInfo( "Unloaded audio system" );
}

// ------------------------------------------------------------------------------------ //
// Р—Р°РіСЂСѓР·РёС‚СЊ РёРЅС„РѕСЂРјР°С†РёСЋ РѕР± РёРіСЂРµ
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadGameInfo()
{
	consoleSystem.PrintInfo( "Loading gameinfo.json" );
	
	// Open file for read
	std::ifstream		file( "gameinfo.json" );
	if ( !file.is_open() )
	{
		consoleSystem.PrintError( "File gameinfo.json not found" );
		return false;
	}

	// Getting all content from file
	std::string					stringBuffer;
	UInt32_t					stringLength = 0;
	std::getline( file, stringBuffer, '\0' );

	// Parse JSON document
	rapidjson::Document			document;
	document.Parse( stringBuffer.c_str() );
	if ( document.HasParseError() )
	{
		consoleSystem.PrintError( "Fail parse gameinfo.json" );
		return false;
	}

	// Getting values from file
	for ( auto it = document.MemberBegin(), itEnd = document.MemberEnd(); it != itEnd; ++it )
	{
		// Getting name game
		if ( !strcmp( it->name.GetString(), "name" ) && it->value.IsString() )
		{
			gameInfo.name = it->value.GetString();
			window.SetTitle( gameInfo.name.c_str() );
		}

		// Getting icon
		else if ( !strcmp( it->name.GetString(), "icon" ) && it->value.IsString() )
		{
			bool		isError = false;
			gameInfo.icon = it->value.GetString();
			Image		image = resourceSystem.LoadImage( gameInfo.icon.c_str(), isError, true, false );

			if ( !isError )
			{
				window.SetIcon( image );
				resourceSystem.UnloadImage( image );
			}
		}
	}

	consoleSystem.PrintInfo( "Loaded gameinfo.json" );
	return true;
}

#include "engine/iscript.h"

// ------------------------------------------------------------------------------------ //
// Р—Р°РїСѓСЃС‚РёС‚СЊ СЃРёРјСѓР»СЏС†РёСЋ
// ------------------------------------------------------------------------------------ //
void le::Engine::RunSimulation()
{
	LIFEENGINE_ASSERT( window.IsOpen() && studioRender );

	if ( !isInitialized )
	{
		consoleSystem.PrintError( "The simulation is not running because engine not initialized" );
		return;
	}
	
	if ( !LoadGameInfo() )
	{
		consoleSystem.PrintError( "Failed loading game info" );
		return;
	}

	IScript*			script = scriptSystem.CreateScript( "main.lua" );
	if ( script ) script->Start();

	consoleSystem.PrintInfo( "*** Run simulation ***" );
	
	float                   currentTick = 0.f;
	float                   lastTick = 0.f;
	float                   delayFrame = 0.f;
	float					deltaTime = 0.f;
	Event                   event;
	bool                    isFocus = true;

	isRunSimulation = true;

	while ( isRunSimulation )
	{
		while ( window.PollEvent( event ) )
		{
			switch ( event.type )
			{
			case Event::ET_WINDOW_CLOSE:
				isRunSimulation = false;
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
			{
				StudioRenderViewport		viewport = studioRender->GetViewport();
				viewport.width = ( UInt32_t ) event.events.windowResize.width;
				viewport.height = ( UInt32_t ) event.events.windowResize.height;

				studioRender->SetViewport( viewport );
				break;
			}

			default: break;
			}
		}

		if ( isFocus )
		{
			currentTick = SDL_GetTicks();
			deltaTime = ( currentTick - lastTick ) / 1000.f;
			lastTick = currentTick;

			delayFrame += deltaTime;
			while ( delayFrame >= FIXED_TIME_UPDATE )
			{
				delayFrame -= FIXED_TIME_UPDATE;

				inputSystem.Update();
				physicSystem->Update();
				materialSystem.Update();
				if ( script ) script->Update();

				inputSystem.Clear();
			}

			studioRender->Begin();
			
			if ( script ) script->Render();
			if ( cvar_phyDebug->GetValueBool() )
				physicSystem->DebugRender();

			studioRender->End();
			studioRender->Present();
		}
	}

	consoleSystem.PrintInfo( "*** Stop simulation ***" );
}

// ------------------------------------------------------------------------------------ //
// РћСЃС‚Р°РЅРѕРІРёС‚СЊ СЃРёРјСѓР»СЏС†РёСЋ
// ------------------------------------------------------------------------------------ //
void le::Engine::StopSimulation()
{
	isRunSimulation = false;
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
le::IMaterialSystem* le::Engine::GetMaterialSystem() const
{
	return ( IMaterialSystem* ) &materialSystem;
}

// ------------------------------------------------------------------------------------ //
// Return physics system
// ------------------------------------------------------------------------------------ //
le::IPhysicsSystem *le::Engine::GetPhysicsSystem() const
{
	return physicSystem;
}

// ------------------------------------------------------------------------------------ //
// Get script system
// ------------------------------------------------------------------------------------ //
le::IScriptSystem* le::Engine::GetScriptSystem() const
{
	return ( IScriptSystem* ) &scriptSystem;
}

// ------------------------------------------------------------------------------------ //
// Get audio system
// ------------------------------------------------------------------------------------ //
le::IAudioSystem* le::Engine::GetAudioSystem() const
{
	return ( IAudioSystem* ) audioSystem;
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
// Get game info
// ------------------------------------------------------------------------------------ //
le::GameInfo le::Engine::GetGameInfo() const
{
	return le::GameInfo{ gameInfo.name.c_str(), gameInfo.icon.c_str() };
}

// ------------------------------------------------------------------------------------ //
// РџРѕР»СѓС‡РёС‚СЊ РІРµСЂСЃРёСЋ РґРІРёР¶РєР°
// ------------------------------------------------------------------------------------ //
const char* le::Engine::GetVersion() const
{
	return LIFEENGINE_VERSION;
}

// ------------------------------------------------------------------------------------ //
// Initialize engine
// ------------------------------------------------------------------------------------ //
bool le::Engine::Initialize( const char* EngineDirectory, const char* LogFile, bool IsEditor )
{
	if ( isInitialized )	return true;
	consoleSystem.PrintInfo( "Initialization lifeEngine" );
	
	engineDirectory = EngineDirectory;
	isEditor = IsEditor;
	
	try
	{
		// Initialize console system
		consoleSystem.Initialize( LogFile );
		
		// Print info by sysem
		SDL_version		sdlVersion;
		SDL_GetVersion( &sdlVersion );

		consoleSystem.PrintInfo( "*** System info start ****" );
		consoleSystem.PrintInfo( "  Base path: %s", SDL_GetBasePath() );
		consoleSystem.PrintInfo( "" );
		consoleSystem.PrintInfo( "  lifeEngine %s", LIFEENGINE_VERSION );
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

		// Initialize path to resources
		resourceSystem.AddPath( "content" );
		resourceSystem.AddPath( ( std::string( EngineDirectory ) + "/content" ).c_str() );

		// Р—Р°РіСЂСѓР¶Р°РµРј Рё РёРЅРёС†РёР°Р»РёР·РёСЂСѓРµРј РїРѕРґСЃРёСЃС‚РµРјС‹
		if ( !LoadModule_StudioRender( ( engineDirectory + "/" + LIFEENGINE_STUDIORENDER_DLL ).c_str() ) )			throw std::runtime_error( "Failed loading studiorender" );
		if ( !LoadModule_PhysicsSystem( ( engineDirectory + "/" + LIFEENGINE_PHYSICSSYSTEM_DLL ).c_str() ) )		throw std::runtime_error( "Failed loading physics system" );
		if ( !LoadModule_AudioSystem( ( engineDirectory + "/" + LIFEENGINE_AUDIOSYSTEM_DLL ).c_str() ) )			throw std::runtime_error( "Failed loading audio system" );

		// Initialize studiorender
		if ( !studioRender )							throw std::runtime_error( "Studiorender not loaded" );
		else if ( !studioRender->Initialize( this ) )	throw std::runtime_error( "Fail initialize studiorender" );

		// Initialize physics
		if ( !physicSystem )							throw std::runtime_error( "Physics system not loaded" );
		else if ( !physicSystem->Initialize( this ) )	throw std::runtime_error( "Fail initialize physics system" );
		
		// Initialize audio
		if ( !audioSystem )								throw std::runtime_error( "Audio system not loaded" );
		else if ( !audioSystem->Initialize( this ) )	throw std::runtime_error( "Fail initialize audio system" );

		// Initialize script system
		if ( !scriptSystem.Initialize( this ) )			throw std::runtime_error( "Fail initialize script system" );

		// Initialize input system
		if ( !inputSystem.Initialize( this ) )			throw std::runtime_error( "Fail initialize input system" );

		// Initialize material system
		if ( !materialSystem.Initialize( this ) )		throw std::runtime_error( "Fail initialize material system" );

		// Initialize resource system
		if ( !resourceSystem.Initialize( this ) )		throw std::runtime_error( "Fail initialize resource system" );

		// Register console commands and vars
		cmd_Exit->Initialize( "exit", "close game", CMD_Exit );
		cmd_Version->Initialize( "version", "show version engine", CMD_Version );
		cvar_phyDebug->Initialize( "phy_debug", "0", CVT_BOOL, "bool value for showing physics debug info", true, 0.f, true, 1.f, nullptr );
		cvar_windowWidth->Initialize( "window_width", "800", CVT_INT, "window width", ChangeCallback_WindowResize );
		cvar_windowHeight->Initialize( "window_height", "600", CVT_INT, "window height", ChangeCallback_WindowResize );
		cvar_windowFullscreen->Initialize( "window_fullscreen", "0", CVT_BOOL, "is fullscreen window", true, 0.f, true, 1.f, ChangeCallback_WindowFullscreen );
		cvar_mouseSensitivity->Initialize( "mouse_sensitivity", "0.15", CVT_FLOAT, "mouse sensitivity", nullptr );
		cvar_rvsinc->Initialize( "r_vsinc", "0", CVT_BOOL, "virtical sinchronize", true, 0.f, true, 1.f, ChangeCallback_EnableVSinc );

		consoleSystem.RegisterCommand( cmd_Exit );
		consoleSystem.RegisterCommand( cmd_Version );

		consoleSystem.RegisterVar( cvar_phyDebug );
		consoleSystem.RegisterVar( cvar_windowWidth );
		consoleSystem.RegisterVar( cvar_windowHeight );
		consoleSystem.RegisterVar( cvar_windowFullscreen );
		consoleSystem.RegisterVar( cvar_mouseSensitivity );
		consoleSystem.RegisterVar( cvar_rvsinc );
	}
	catch ( const std::exception& Exception )
	{
		consoleSystem.PrintError( Exception.what() );
		return false;
	}

	consoleSystem.PrintInfo( "Initialized lifeEngine" );
	isInitialized = true;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Get engine directory
// ------------------------------------------------------------------------------------ //
const char* le::Engine::GetEngineDirectory() const
{
	return engineDirectory.c_str();
}

// ------------------------------------------------------------------------------------ //
// Get fixed time step
// ------------------------------------------------------------------------------------ //
float le::Engine::GetFixedTimeStep() const
{
	return FIXED_TIME_UPDATE;
}

// ------------------------------------------------------------------------------------ //
// Get configurations
// ------------------------------------------------------------------------------------ //
le::Configurations le::Engine::GetConfigurations() const
{
	return Configurations{ cvar_windowWidth, cvar_windowHeight, cvar_windowFullscreen, cvar_mouseSensitivity, cvar_rvsinc };
}

// ------------------------------------------------------------------------------------ //
// Is editor
// ------------------------------------------------------------------------------------ //
bool le::Engine::IsEditor() const
{
	return isEditor;
}