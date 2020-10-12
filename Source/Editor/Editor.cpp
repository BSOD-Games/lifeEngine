// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include <qapplication.h>
#include <qmessagebox.h>

#include <Engine.h>
#include <Misc/EngineGlobals.h>
#include <Logging/LogMacros.h>
#include <Rendering/RHI/IRHI.h>
#include <Rendering/RenderSystem.h>
#include <System/Window.h>
#include <System/FileSystem.h>
#include <System/InputSystem.h>
#include <Resources/ResourceSystem.h>
#include <World/Actor.h>
#include <World/Components/SpriteComponent.h>
#include <World/Components/CameraComponent.h>

#include <Resources/Resource.h>
#include <Resources/Texture2D.h>
#include <Resources/Material.h>
#include <Resources/World.h>

class Player : public le::Actor
{
public:
	/* Contructor */
	Player() :
		size( 64.f, 64.f ),
		move( 0.f, 0.f )
	{}

	/* Initialize */
	void Initialize() override
	{
		cameraComponent.SetOrthoProjection( 0, 800, 0, 600, 0, 10 );
		cameraComponent.SetPosition( le::FVector3D( -300, 0, 0 ) ); /*5800*/
		//cameraComponent.SetActor( this );
		le::GRenderSystem->SetCamera( &cameraComponent );

		spriteComponent.SetActor( this );
		spriteComponent.SetSize( size );
		spriteComponent.SetType( le::ST_Static );
		spriteComponent.SetMaterial( Cast< le::Material >( le::GResourceSystem->FindResource( "Content/Materials/Player.lmt", le::RT_Material ) ) );
		spriteComponent.SetTextureRect( le::FSRectFloat( 0.5410f, 0.2935f, 0.1343f, 0.3134f ) );

		isInitialized = true;
	}

	/* Tick */
	void Tick() override
	{
		if ( le::GInputSystem->IsKeyDown( le::BC_KeyA ) )
			move.x -= 1.f;

		if ( le::GInputSystem->IsKeyDown( le::BC_KeyD ) )
			move.x += 1.f;

		if ( le::GInputSystem->IsKeyDown( le::BC_KeyW ) )
			move.y += 1.f;

		if ( le::GInputSystem->IsKeyDown( le::BC_KeyS ) )
			move.y -= 1.f;

		position = le::FVector3D( move.x - move.y, ( move.x + move.y ) / 2.f, 1.f );
	}

	/* Render */
	void Render() override
	{
		spriteComponent.Render();
	}

private:
	le::FVector2D				size;
	le::FVector2D move;
	le::SpriteComponent			spriteComponent;
	le::CameraComponent			cameraComponent;
};

class Game : public le::IGame
{
public:
	/* Constructor */
	Game() :
		world( nullptr )
	{}

	/* Destructor */
	~Game()
	{
		if ( world )		world->ReleaseRef();
	}

	/* Initialize game */
	bool Initialize()
	{
		world = Cast< le::World >( le::GResourceSystem->FindResource( "Content/untitled.tmx", le::RT_World ) );
		if ( !world )
		{
			world = new le::World();
			LIFEENGINE_LOG_ERROR( "Parterya", "World [Content/untitled.tmx] not founded or not readed" );
		}

		world->Spawn< Player >( le::FVector3D( 0, 0, 1.f ) );	
		return true;
	}

	/* Tick game */
	void Tick()
	{
		if ( world )	world->Tick();
	}

	/* Render frame */
	void RenderFrame()
	{
		if ( world )	world->Render();
	}

private:
	le::World*		world;
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