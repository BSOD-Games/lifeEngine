// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <qapplication.h>
#include <qmessagebox.h>

#include <Engine.h>
#include <Logging/LogMacros.h>
#include <Rendering/RenderSystem.h>
#include <System/Window.h>
#include <System/FileSystem.h>

#include <fstream>

// ------------------------------------------------------------------------------------ //
// Main function
// ------------------------------------------------------------------------------------ //
int main( int argc, char** argv )
{
	le::FileSystem::GetInstance()->SetRootPath( "../../" );

	le::Engine::GetInstance()->Initialize( "../../Config.json", "../../lifeEditor.log" );
	le::Window::GetInstance()->Open( "lifeEditor", 800, 600 );
	le::FRHIContext rhiContext = le::RenderSystem::GetInstance()->CreateContext( le::Window::GetInstance()->GetHandle() );
	le::RenderSystem::GetInstance()->MakeCurrentContext( rhiContext );
	le::RenderSystem::GetInstance()->SetViewport( 0, 0, 800, 600 );

	while ( le::Window::GetInstance()->IsOpen() ) 
	{
		le::Event		event;
		while ( le::Window::GetInstance()->PollEvent( event ) ) 
		{
			switch ( event.type )
			{
			case le::ET_WindowClose:
				le::Window::GetInstance()->Close();
				break;
			}
		}

		le::RenderSystem::GetInstance()->Begin();
		le::RenderSystem::GetInstance()->End();
		le::RenderSystem::GetInstance()->Present();
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