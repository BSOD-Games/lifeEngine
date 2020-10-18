// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Misc/EngineDefines.h"
#include "Misc/EngineGlobals.h"
#include "System/InputSystem.h"
#include "Engine.h"
#include "LauncherPrivate.h"

/**
 * Start engine loop
 */
int AppEngineLoop( int Argc, char** Argv )
{
	{
		// Getting game manifest and initialize engine
		le::SGameManifest		gameManifest = AppGetGameManifest();
		if ( !le::GEngine->Initialize( gameManifest.windowName, gameManifest.windowWidth, gameManifest.windowHeight, gameManifest.logPath, gameManifest.rootPath ) )
			return 1;
	}

	// Creating game
	le::IGame*	game = AppGameCreate();
	LIFEENGINE_ASSERT( game );

	// Initializing game and if failed - exit from program
	if ( !game->Initialize() )
	{
		AppGameDelete( game );
		return 2;
	}

	// Start game in engine
	le::GEngine->StartGame( game );

	// Engine loop
	bool        isFocus = true;
	while ( le::GEngine->IsPlayedGame() )
	{
		le::SEvent		event;
		while ( le::GWindow->PollEvent( event ) )
		{
			switch ( event.type )
			{
			case le::SEvent::ET_WindowClose:
				le::GEngine->StopGame();
				le::GWindow->Close();
				break;

			case le::SEvent::ET_WindowFocusGained:
				isFocus = true;
				break;

			case le::SEvent::ET_WindowFocusLost:
				isFocus = false;
				break;

			case le::SEvent::ET_KeyPressed:
			case le::SEvent::ET_KeyReleased:
			case le::SEvent::ET_MouseMove:
			case le::SEvent::ET_MousePressed:
			case le::SEvent::ET_MouseReleased:
			case le::SEvent::ET_MouseWheel:
			case le::SEvent::ET_TextInput:
				le::GInputSystem->ApplyEvent( event );
				break;

			case le::SEvent::ET_WindowResize:
				LIFEENGINE_ASSERT( le::GRHI );
				le::GRHI->SetViewport( 0, 0, event.event.windowResize.width, event.event.windowResize.height );
				break;

			default: break;
			}
		}

		if ( isFocus )
		{
			le::GEngine->Tick();
			le::GEngine->RenderFrame();
		}
	}

	AppGameDelete( game );
	return 0;
}