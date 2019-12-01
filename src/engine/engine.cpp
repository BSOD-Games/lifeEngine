//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
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
#include "engine/istudiorenderinternal.h"
#include "engine/global.h"
#include "engine/convar.h"
#include "engine/concmd.h"
#include "engine/buildnum.h"

LIFEENGINE_ENGINE_API( le::Engine );

// ------------------------------------------------------------------------------------ //
// ���������� ������� ������
// ------------------------------------------------------------------------------------ //
void CMD_Exit( le::UInt32_t CountArguments, const char** Arguments )
{
	if ( !le::g_engine ) return;
	le::g_engine->StopSimulation();
	exit( 0 );
}

// ------------------------------------------------------------------------------------ //
// ���������� ������� ������ ������ ������
// ------------------------------------------------------------------------------------ //
void CMD_Version( le::UInt32_t CountArguments, const char** Arguments )
{
	if ( !le::g_consoleSystem ) return;
	le::g_consoleSystem->PrintInfo( "lifeEngine version %i.%i.%i (build %i)", LIFEENGINE_VERSION_MAJOR, LIFEENGINE_VERSION_MINOR, LIFEENGINE_VERSION_PATCH, le::Engine_BuildNumber() );
}

// ------------------------------------------------------------------------------------ //
// �����������
// ------------------------------------------------------------------------------------ //
le::Engine::Engine() :
	isRunSimulation( false ),
	isInit( false ),
	studioRender( nullptr ),
	studioRenderDescriptor( { nullptr, nullptr, nullptr, nullptr } ),
	criticalError( nullptr ),
	cmd_Exit( new ConCmd() ),
	cmd_Version( new ConCmd() )
{
	LIFEENGINE_ASSERT( !g_engine );

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

	g_consoleSystem = &consoleSystem;
	g_engine = this;
}

// ------------------------------------------------------------------------------------ //
// ����������
// ------------------------------------------------------------------------------------ //
le::Engine::~Engine()
{
	if ( studioRender ) UnloadStudioRender();
	if ( window.IsOpen() ) window.Close();

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
// ��������� ������ �������
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadStudioRender( const char* PathDLL )
{
	// ���� ������ ����� ��� ��������, �� �������
	if ( studioRenderDescriptor.handle ) UnloadStudioRender();

	// ��������� ������
	studioRenderDescriptor.handle = SDL_LoadObject( PathDLL );
	if ( !studioRenderDescriptor.handle )	return false;

	// ����� �� ������ ������ ��� �������� �������
	studioRenderDescriptor.LE_CreateStudioRender = ( LE_CreateStudioRenderFn_t ) SDL_LoadFunction( studioRenderDescriptor.handle, "LE_CreateStudioRender" );
	studioRenderDescriptor.LE_DeleteStudioRender = ( LE_DeleteStudioRenderFn_t ) SDL_LoadFunction( studioRenderDescriptor.handle, "LE_DeleteStudioRender" );
	studioRenderDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( studioRenderDescriptor.handle, "LE_SetCriticalError" );
	if ( !studioRenderDescriptor.LE_CreateStudioRender )	return false;

	// ������� ������
	if ( studioRenderDescriptor.LE_SetCriticalError ) 
		studioRenderDescriptor.LE_SetCriticalError( g_criticalError );

	studioRender = ( IStudioRenderInternal* ) studioRenderDescriptor.LE_CreateStudioRender();
	if ( !studioRender->Initialize( this ) )	return false;

	return true;
}

// ------------------------------------------------------------------------------------ //
// ��������� ������ �������
// ------------------------------------------------------------------------------------ //
void le::Engine::UnloadStudioRender()
{
	if ( !studioRender ) return;

	if ( studioRenderDescriptor.LE_DeleteStudioRender )
		studioRenderDescriptor.LE_DeleteStudioRender( studioRender );

	SDL_UnloadObject( studioRenderDescriptor.handle );
	studioRender = nullptr;
	studioRenderDescriptor = { nullptr, nullptr, nullptr };
}

// ------------------------------------------------------------------------------------ //
// ��������� ������
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
			// ����� ���������
			if ( strcmp( it->name.GetString(), "general" ) == 0 )
			{
				// ������ ����
				if ( strcmp( itObject->name.GetString(), "width" ) == 0 && itObject->value.IsNumber() )
					configurations.windowWidth = itObject->value.GetInt();

				// ������ ����
				else if ( strcmp( itObject->name.GetString(), "height" ) == 0 && itObject->value.IsNumber() )
					configurations.windowHeight = itObject->value.GetInt();

				// ������������ �� �����
				else if ( strcmp( itObject->name.GetString(), "fullscreen" ) == 0 && itObject->value.IsBool() )
					configurations.isFullscreen = itObject->value.GetBool();
			}

			// ��������� �������
			else if ( strcmp( it->name.GetString(), "studiorender" ) == 0 )
			{
				// �������� �� ������������ �������������
				if ( strcmp( itObject->name.GetString(), "vsinc" ) == 0 && itObject->value.IsBool() )
					configurations.isVerticalSinc = itObject->value.GetBool();

				// �������� �� ������������ �������������
				else if ( strcmp( itObject->name.GetString(), "fov" ) == 0 && itObject->value.IsNumber() )
					configurations.fov = itObject->value.GetFloat();
			}
		}
	}

	consoleSystem.PrintInfo( "Loaded file configurations [%s]", FilePath );
	return true;
}

// ------------------------------------------------------------------------------------ //
// ��������� ������
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
// ��������� ���������
// ------------------------------------------------------------------------------------ //
void le::Engine::RunSimulation()
{
	LIFEENGINE_ASSERT( window.IsOpen() || studioRender );

	consoleSystem.PrintInfo( "*** Game logic start ***" );
	isRunSimulation = true;
	Event		event;

	while ( isRunSimulation )
	{
		// TODO: �������� ��������� ������� ����

		while ( window.PollEvent( event ) )
			switch ( event.type )
			{
			case Event::ET_WINDOW_CLOSE:
				StopSimulation();
				break;

			case Event::ET_WINDOW_RESIZE:
				studioRender->ResizeViewport( 0, 0, event.windowResize.width, event.windowResize.height );
				break;
			}			

		studioRender->RenderFrame();
	}

	isRunSimulation = false;
	consoleSystem.PrintInfo( "*** Game logic end ***" );
}

// ------------------------------------------------------------------------------------ //
// ���������� ���������
// ------------------------------------------------------------------------------------ //
void le::Engine::StopSimulation()
{
	isRunSimulation = false;
}

// ------------------------------------------------------------------------------------ //
// ������ ������������ ������
// ------------------------------------------------------------------------------------ //
void le::Engine::SetConfig( const Configurations& Configurations )
{
	configurations = Configurations;
}

// ------------------------------------------------------------------------------------ //
// �������� �� ���������
// ------------------------------------------------------------------------------------ //
bool le::Engine::IsRunSimulation() const
{
	return isRunSimulation;
}

// ------------------------------------------------------------------------------------ //
// �������� ������� �������
// ------------------------------------------------------------------------------------ //
le::IConsoleSystem* le::Engine::GetConsoleSystem() const
{
	return ( IConsoleSystem* ) &consoleSystem;
}

// ------------------------------------------------------------------------------------ //
// �������� ����
// ------------------------------------------------------------------------------------ //
le::IStudioRender* le::Engine::GetStudioRender() const
{
	return ( IStudioRender* ) &studioRender;
}

// ------------------------------------------------------------------------------------ //
// �������� ����
// ------------------------------------------------------------------------------------ //
le::IWindow* le::Engine::GetWindow() const
{
	return ( IWindow* ) &window;
}

// ------------------------------------------------------------------------------------ //
// �������� ������������ ������
// ------------------------------------------------------------------------------------ //
const le::Configurations& le::Engine::GetConfigurations() const
{
	return configurations;
}

// ------------------------------------------------------------------------------------ //
// �������� ������ ������
// ------------------------------------------------------------------------------------ //
const le::Version& le::Engine::GetVersion() const
{
	return Version( LIFEENGINE_VERSION_MAJOR, LIFEENGINE_VERSION_MINOR, LIFEENGINE_VERSION_PATCH, Engine_BuildNumber() );
}

// ------------------------------------------------------------------------------------ //
// ���������������� ������
// ------------------------------------------------------------------------------------ //
bool le::Engine::Initialize( WindowHandle_t WindowHandle )
{
	if ( isInit ) return true;
	consoleSystem.PrintInfo( "Initialization lifeEngine" );

	// ������� ��������� ����������
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
		// �������������� ���� ����������. ���� ��������� �� ���� � ���������� ����, 
		// �� ������� ���� ����, ����� ���������� ���

		if ( !WindowHandle )
		{
			if ( !window.Create( "lifeEngine", configurations.windowWidth, configurations.windowHeight, configurations.isFullscreen ? SW_FULLSCREEN : SW_DEFAULT ) )
				throw std::exception( SDL_GetError() );
		}
		else 
			window.SetHandle( WindowHandle );

		// ��������� � �������������� ������

		if ( !LoadStudioRender( LIFEENGINE_STUDIORENDER_DLL ) )		throw std::exception( "Failed loading studiorender" );
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
// ��������� ����
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadGame( const char* DirGame )
{
	return false;
}

// ------------------------------------------------------------------------------------ //
// ��������� ����
// ------------------------------------------------------------------------------------ //
void le::Engine::UnloadGame()
{}