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

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	enum BUTTON_CODE;

	//---------------------------------------------------------------------//

	BUTTON_CODE				ButtonCode_ScanCodeToButtonCode( SDL_Scancode Scancode );
	BUTTON_CODE				ButtonCode_MouseButtonToButtonCode( UInt8_t ButtonIndex );
	BUTTON_CODE				ButtonCode_StringToButtonCode( const char* String );

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !BUTTON_TRANSLATION_H