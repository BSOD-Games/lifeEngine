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
#include <FreeImage/FreeImage.h>

#include "engine/lifeengine.h"
#include "engine/paths.h"
#include "engine/engine.h"
#include "engine/window.h"
#include "engine/istudiorenderinternal.h" // TODO: ����������� � studiorender ������ �����
#include "engine/igame.h"
#include "engine/global.h"
#include "engine/convar.h"
#include "engine/concmd.h"
#include "engine/buildnum.h"
#include "engine/resourcesystem.h"
#include "materialsystem/imaterialsysteminternal.h"

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
// ��������� �����������
// ------------------------------------------------------------------------------------ //
void LE_LoadImage( const char* Path, le::Image& Image, bool& IsError, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels )
{
	IsError = false;
	FREE_IMAGE_FORMAT		imageFormat = FIF_UNKNOWN;
	imageFormat = FreeImage_GetFileType( Path, 0 );

	if ( imageFormat == FIF_UNKNOWN )
		imageFormat = FreeImage_GetFIFFromFilename( Path );

	FIBITMAP* bitmap = FreeImage_Load( imageFormat, Path, 0 );
	if ( !bitmap )
	{
		IsError = true;
		return;
	}

	if ( IsFlipVertical )				FreeImage_FlipVertical( bitmap );
	if ( IsSwitchRedAndBlueChannels )
	{
		auto		red = FreeImage_GetChannel( bitmap, FREE_IMAGE_COLOR_CHANNEL::FICC_RED );
		auto		blue = FreeImage_GetChannel( bitmap, FREE_IMAGE_COLOR_CHANNEL::FICC_BLUE );

		FreeImage_SetChannel( bitmap, blue, FREE_IMAGE_COLOR_CHANNEL::FICC_RED );
		FreeImage_SetChannel( bitmap, red, FREE_IMAGE_COLOR_CHANNEL::FICC_BLUE );

		FreeImage_Unload( red );
		FreeImage_Unload( blue );
	}

	le::UInt8_t*		tempData = FreeImage_GetBits( bitmap );
	Image.width = FreeImage_GetWidth( bitmap );
	Image.height = FreeImage_GetHeight( bitmap );
	Image.depth = FreeImage_GetBPP( bitmap );
	Image.pitch = FreeImage_GetPitch( bitmap );

	Image.data = ( le::UInt8_t* ) malloc( Image.pitch * Image.height );
	memcpy( Image.data, tempData, Image.pitch * Image.height );

	Image.rMask = 0x00ff0000;
	Image.gMask = 0x0000ff00;
	Image.bMask = 0x000000ff;
	Image.aMask = ( Image.depth == 24 ) ? 0 : 0xff000000;

	FreeImage_Unload( bitmap );
	return;
}

// ------------------------------------------------------------------------------------ //
// �����������
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

	resourceSystem.RegisterParser( "png", LE_LoadImage );
	resourceSystem.RegisterParser( "jpg", LE_LoadImage );
}

// ------------------------------------------------------------------------------------ //
// ����������
// ------------------------------------------------------------------------------------ //
le::Engine::~Engine()
{
	if ( game )				UnloadModule_Game();

	resourceSystem.DeleteAll();

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
// ��������� ������ �������
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadModule_StudioRender( const char* PathDLL )
{
	// ���� ������ ����� ��� ��������, �� �������
	if ( studioRenderDescriptor.handle ) UnloadModule_StudioRender();

	consoleSystem.PrintInfo( "Loading studiorender [%s]", PathDLL );

	try
	{
		// ��������� ������
		studioRenderDescriptor.handle = SDL_LoadObject( PathDLL );
		if ( !studioRenderDescriptor.handle )	throw std::exception( SDL_GetError() );

		// ����� �� ������ API ��� ������ � ���
		studioRenderDescriptor.LE_CreateStudioRender = ( LE_CreateStudioRenderFn_t ) SDL_LoadFunction( studioRenderDescriptor.handle, "LE_CreateStudioRender" );
		studioRenderDescriptor.LE_DeleteStudioRender = ( LE_DeleteStudioRenderFn_t ) SDL_LoadFunction( studioRenderDescriptor.handle, "LE_DeleteStudioRender" );
		studioRenderDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( studioRenderDescriptor.handle, "LE_SetCriticalError" );
		if ( !studioRenderDescriptor.LE_CreateStudioRender )	throw std::exception( "Function LE_CreateStudioRender not found" );

		// ������� ������
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
// ��������� ������ �������
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
// ��������� ������ ������� ����������
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadModule_MaterialSystem( const char* PathDLL )
{
	// ���� ������ ����� ��� ��������, �� �������
	if ( materialSystemDescriptor.handle ) UnloadModule_MaterialSystem();

	consoleSystem.PrintInfo( "Loading material system [%s]", PathDLL );

	try
	{
		// ��������� ������
		materialSystemDescriptor.handle = SDL_LoadObject( PathDLL );
		if ( !materialSystemDescriptor.handle )	throw std::exception( SDL_GetError() );

		// ����� �� ������ API ��� ������ � ���
		materialSystemDescriptor.LE_CreateMaterialSystem = ( LE_CreateMaterialSystemFn_t ) SDL_LoadFunction( materialSystemDescriptor.handle, "LE_CreateMaterialSystem" );
		materialSystemDescriptor.LE_DeleteMaterialSystem = ( LE_DeleteMaterialSystemFn_t ) SDL_LoadFunction( materialSystemDescriptor.handle, "LE_DeleteMaterialSystem" );
		materialSystemDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( materialSystemDescriptor.handle, "LE_SetCriticalError" );
		if ( !materialSystemDescriptor.LE_CreateMaterialSystem )	throw std::exception( "Function LE_CreateMaterialSystem not found" );

		// ������� ������
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
// ��������� ������ ������� ����������
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
// ��������� ���������� �� ����
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
		// ���� � ������ � ������� �������
		if ( strcmp( it->name.GetString(), "gameDLL" ) == 0 && it->value.IsString() )
		{
			stringLength = it->value.GetStringLength();

			gameInfo.gameDLL = new char[ stringLength + 1 ];
			memcpy( gameInfo.gameDLL, it->value.GetString(), stringLength );
			gameInfo.gameDLL[ stringLength ] = '\0';
		}

		// ���� � ������
		if ( strcmp( it->name.GetString(), "icon" ) == 0 && it->value.IsString() )
		{
			stringLength = it->value.GetStringLength();

			gameInfo.icon = new char[ stringLength + 1 ];
			memcpy( gameInfo.icon, it->value.GetString(), stringLength );
			gameInfo.icon[ stringLength ] = '\0';
		}

		// ��������� ����	
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
// ��������� ������ ����
// ------------------------------------------------------------------------------------ //
bool le::Engine::LoadModule_Game( const char* PathDLL )
{
	// ���� ������ ����� ��� ��������, �� �������
	if ( gameDescriptor.handle ) UnloadModule_Game();

	consoleSystem.PrintInfo( "Loading game [%s]", PathDLL );

	try
	{
		// ��������� ������
		gameDescriptor.handle = SDL_LoadObject( PathDLL );
		if ( !gameDescriptor.handle )	throw std::exception( SDL_GetError() );

		// ����� �� ������ API ��� ������ � ���
		gameDescriptor.LE_CreateGame = ( LE_CreateGameFn_t ) SDL_LoadFunction( gameDescriptor.handle, "LE_CreateGame" );
		gameDescriptor.LE_DeleteGame = ( LE_DeleteGameFn_t ) SDL_LoadFunction( gameDescriptor.handle, "LE_DeleteGame" );
		gameDescriptor.LE_SetCriticalError = ( LE_SetCriticalErrorFn_t ) SDL_LoadFunction( gameDescriptor.handle, "LE_SetCriticalError" );
		if ( !gameDescriptor.LE_CreateGame )	throw std::exception( "Function LE_CreateGame not found" );

		// ������� ������� ������
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
// ��������� ������ ����
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
		// TODO: �������� ��������� ������� ����

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
// �������� ������� ����������
// ------------------------------------------------------------------------------------ //
le::IMaterialSystem* le::Engine::GetMaterialSystem() const
{
	return materialSystem;
}

// ------------------------------------------------------------------------------------ //
// �������� ����
// ------------------------------------------------------------------------------------ //
le::IResourceSystem* le::Engine::GetResourceSystem() const
{
	return ( IResourceSystem* ) &resourceSystem;
}

// ------------------------------------------------------------------------------------ //
// �������� ����
// ------------------------------------------------------------------------------------ //
le::IWindow* le::Engine::GetWindow() const
{
	return ( IWindow* ) &window;
}

// ------------------------------------------------------------------------------------ //
// �������� ������� ������
// ------------------------------------------------------------------------------------ //
le::IFactory* le::Engine::GetFactory() const
{
	return ( IFactory* ) &engineFactory;
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

		// ��������� � �������������� ����������

		if ( !LoadModule_StudioRender( LIFEENGINE_STUDIORENDER_DLL ) )		throw std::exception( "Failed loading studiorender" );
		if ( !LoadModule_MaterialSystem( LIFEENGINE_MATERIALSYSTEM_DLL ) )	throw std::exception( "Failed loading materialsystem" );

		if ( !materialSystem->LoadShaderDLL( LIFEENGINE_STDSHADERS_DLL ) )	throw std::exception( "Failed loading stdshaders" );
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
	if ( !LoadGameInfo( DirGame ) || !LoadModule_Game( ( std::string( DirGame ) + "/" + gameInfo.gameDLL ).c_str() ) )
		return false;

	if ( gameInfo.icon )
	{
		bool		isError = false;
		Image		image;

		LE_LoadImage( ( std::string( gameInfo.gameDir ) + "/" + gameInfo.icon ).c_str(), image, isError, true, false );
		if ( !isError )
		{
			window.SetIcon( image );
			delete[] image.data;
		}
	}

	window.SetTitle( gameInfo.title );
	return true;
}

// ------------------------------------------------------------------------------------ //
// ��������� ����
// ------------------------------------------------------------------------------------ //
void le::Engine::UnloadGame()
{
	StopSimulation();

	gameInfo.Clear();
	if ( game ) UnloadModule_Game();
}