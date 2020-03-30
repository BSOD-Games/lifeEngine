//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef BUTTON_TRANSLATION_H
#define BUTTON_TRANSLATION_H

#include <SDL2/SDL.h>
#include "common/types.h"
#include "common/buttoncode.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	BUTTON_CODE				ButtonCode_ScanCodeToButtonCode( SDL_Scancode Scancode );
	SDL_Scancode			ButtonCode_ButtonCodeToScanCode( BUTTON_CODE ButtonCode );
	BUTTON_CODE				ButtonCode_MouseButtonToButtonCode( UInt8_t ButtonIndex );
	UInt8_t					ButtonCode_ButtonCodeToMouseButton( BUTTON_CODE ButtonCode );
	BUTTON_CODE				ButtonCode_StringToButtonCode( const char* String );

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !BUTTON_TRANSLATION_H
