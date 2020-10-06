// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <SDL2/SDL.h>

#include "Misc/EngineGlobals.h"
#include "Logging/Logger.h"
#include "System/FileSystem.h"
#include "System/InputSystem.h"
#include "Rendering/RenderSystem.h"
#include "Resources/ResourceSystem.h"
#include "World/World.h"
#include "Engine.h"

// RHI depending on the platform
#ifdef PLATFORM_WINDOWS
	#include "OpenGL4RHI.h"
#else
	#error Unsopported platform
#endif // PLATFORM_WINDOWS

#define	FIXED_TIME_TICK		( 1 / 60.f )

/**
 * Constructor
 */
le::Engine::Engine() :
	currentTick( 0.f ),
	lastTick( 0.f ),
	delayFrame( 0.f ),
	deltaTime( 0.f ),
	mainRHIContext( nullptr ),
	game( nullptr )
{}

/**
 * Destructor
 */
le::Engine::~Engine()
{
	StopGame();
	
	if ( GRHI )
	{
		if ( mainRHIContext )		GRHI->DeleteContext( mainRHIContext );
		
		delete GRHI;
		GRHI = nullptr;
	}

	GLogger->CloseFile();
}

/**
 * Initialize engine with creating window
 */
bool le::Engine::Initialize( const std::string& InWindowTitle, uint32 InWindowWidth, uint32 InWindowHeight, const Path& InLogPath, const Path& InRootPath )
{
	GFileSystem->SetRootPath( InRootPath );
	GLogger->SetFile( InLogPath );

	GWindow->Open( InWindowTitle, InWindowWidth, InWindowHeight );
	
	// Create RHI and initialize render system
	GRHI = new OpenGL4RHI();
	
	mainRHIContext = GRHI->CreateContext( GWindow->GetHandle() );
	if ( !mainRHIContext )		return false;
	GRHI->MakeCurrentContext( mainRHIContext );
	
	if ( !GRenderSystem->Initialize() )		return false;

	// Initialize resource system
	if ( !GResourceSystem->Initialize() )	return false;
	return true;
}

/**
 * Initialize engine with native window
 */
bool le::Engine::Initialize( FWindowHandle InWindowHandle, const Path& InLogPath, const Path& InRootPath )
{
	// Create RHI and initialize render system
	GRHI = new OpenGL4RHI();

	mainRHIContext = GRHI->CreateContext( GWindow->GetHandle() );
	if ( !mainRHIContext )		return false;
	GRHI->MakeCurrentContext( mainRHIContext );

	if ( !GRenderSystem->Initialize() )		return false;

	// Initialize resource system
	if ( !GResourceSystem->Initialize() )	return false;
	return true;
}

/**
 * Stop game
 */
void le::Engine::StopGame()
{
	if ( !game )	return;

	GWorld->KillAllActors();
	GResourceSystem->UnloadResources();
	game = nullptr;
}

/**
 * Tick game
 */
void le::Engine::Tick()
{
	if ( !game )	return;

	currentTick = SDL_GetTicks();
	deltaTime = ( currentTick - lastTick ) / 1000.f;
	lastTick = currentTick;

	delayFrame += deltaTime;
	while ( delayFrame >= FIXED_TIME_TICK )
	{
		delayFrame -= FIXED_TIME_TICK;

		game->Tick();
		GInputSystem->Reset();
	}
}

/**
 * Render frame
 */
void le::Engine::RenderFrame()
{
	if ( !game )	return;

	GRenderSystem->Begin();
	game->RenderFrame();
	GRenderSystem->End();
	GRenderSystem->Present( mainRHIContext );
}
