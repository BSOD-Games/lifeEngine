// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <qapplication.h>
#include <qmessagebox.h>

#include <Engine.h>
#include <Logging/LogMacros.h>
#include <System/Window.h>

// ------------------------------------------------------------------------------------ //
// Main function
// ------------------------------------------------------------------------------------ //
int main( int argc, char** argv )
{
	le::Engine::GetInstance()->Initialize( "../../Config.json", "../../lifeEditor.log" );
	le::Window::GetInstance()->Open( "lifeEditor", 800, 600 );

	LIFEENGINE_LOG_DEBUG( "Editor", "Messa" );
	le::Window::ShowMessageBox( "Hello", le::MBT_Info );
	
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
	}
	
	LIFEENGINE_LOG_FAIL( "Editor", "FAAAAIL i = %i", 23 );
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