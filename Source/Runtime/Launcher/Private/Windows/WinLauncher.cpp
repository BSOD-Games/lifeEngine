// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Misc/EngineDefines.h"
#include "LauncherPrivate.h"

#ifdef PLATFORM_WINDOWS
#include <Windows.h>

/**
 * Main function for Windows
 */
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpCmdLine, int nCmdShow )
{
	// TODO: [yehor.pohuliaka] Add getting arguments of start program
	return AppEngineLoop( 0, nullptr );
}

#else
	#error This file only for Windows
#endif // PLATFORM_WINDOWS