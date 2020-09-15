// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef BUTTONTRANSLATION_H
#define BUTTONTRANSLATION_H

#include <string>
#include <SDL2/SDL.h>

#include "Misc/Types.h"
#include "System/ButtonCode.h"

namespace le
{
	EButtonCode				ScanCodeToButtonCode( SDL_Scancode InScancode );
	SDL_Scancode			ButtonCodeToScanCode( EButtonCode InButtonCode );
	EButtonCode				MouseButtonToButtonCode( uint8 InButtonIndex );
	uint8					ButtonCodeToMouseButton( EButtonCode InButtonCode );
	EButtonCode				StringToButtonCode( const std::string& InString );
}

#endif // !BUTTONTRANSLATION_H
