// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <qapplication.h>
#include <qmessagebox.h>

#include <Engine.h>
#include <Logging/LogMacros.h>

// ------------------------------------------------------------------------------------ //
// Main function
// ------------------------------------------------------------------------------------ //
int main( int argc, char** argv )
{
	le::Engine::GetInstance()->Initialize( "../../Config.json", "../../lifeEditor.log" );
	
	LIFEENGINE_LOG_DEBUG( "Editor", "Messa" );
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