// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <qapplication.h>
#include <qmessagebox.h>

#include <Engine.h>
#include <Misc/EngineGlobals.h>
#include <Logging/LogMacros.h>
#include <Rendering/RenderSystem.h>
#include <System/Window.h>
#include <System/FileSystem.h>
#include <System/InputSystem.h>
#include <Resources/ResourceSystem.h>
#include <World/IActor.h>
#include <World/World.h>
#include <World/Components/SpriteComponent.h>

#include <Resources/Resource.h>
#include <Resources/Texture2D.h>
#include <Resources/Material.h>

class Player : public le::IActor
{
public:
	/* Contructor */
	Player() :
		size( 0.5f, 0.5f )
	{
		spriteComponent.SetSize( size );
		spriteComponent.SetType( le::ST_Static );
		spriteComponent.SetMaterial( Cast< le::Material >( le::GResourceSystem->FindResource( "Content/M.lmt", le::RT_Material ) ) );
	}

	/* Tick */
	void Tick() override
	{
		size -= 0.001f;
		if ( size.x <= 0.1f || size.y <= 0.1f )		size.Set( 0.5f, 0.5f );

		spriteComponent.SetSize( size );
	}

	/* Render */
	void Render() override
	{
		spriteComponent.Render();
	}

private:
	le::SVector2D				size;
	le::SpriteComponent			spriteComponent;
};

class Game : public le::IGame
{
public:
	/* Initialize game */
	bool Initialize()
	{
		le::GWorld->Spawn<Player>();
		return true;
	}

	/* Tick game */
	void Tick()
	{
		le::GWorld->Tick();
	}

	/* Render frame */
	void RenderFrame()
	{
		le::GWorld->Render();
	}
};

// ------------------------------------------------------------------------------------ //
// Main function
// ------------------------------------------------------------------------------------ //
int main( int argc, char** argv )
{
	le::GEngine->Initialize( "Parterya", 800, 600, "../../Parterya.log", "../../" );
	
	Game		game;
	bool        isFocus = true;
	
	game.Initialize();
	le::GEngine->StartGame( &game );

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
				le::GRenderSystem->SetViewport( 0, 0, event.event.windowResize.width, event.event.windowResize.height );
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

	return 0;
}

//---------------------------------------------------------------------//

#ifdef PLATFORM_WINDOWS
#include <Windows.h>

// ------------------------------------------------------------------------------------ //
// Windows main function
// ------------------------------------------------------------------------------------ //
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpCmdLine, int nCmdShow )
{
	return main( 0, nullptr );
}

#endif // PLATFORM_WINDOWS

//---------------------------------------------------------------------//