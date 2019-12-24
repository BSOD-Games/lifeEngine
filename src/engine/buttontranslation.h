//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
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