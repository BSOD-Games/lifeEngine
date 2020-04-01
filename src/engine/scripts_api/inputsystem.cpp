//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "../global.h"
#include "engine/inputsystem.h"
#include "inputsystem.h"

// ------------------------------------------------------------------------------------ //
// Is key down
// ------------------------------------------------------------------------------------ //
int InputSystem_IsKeyDown( buttonCode_t Key )
{
	return le::g_inputSystem->IsKeyDown( ( le::BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is key up
// ------------------------------------------------------------------------------------ //
int InputSystem_IsKeyUp( buttonCode_t Key )
{
	return le::g_inputSystem->IsKeyUp( ( le::BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is mouse key down
// ------------------------------------------------------------------------------------ //
int InputSystem_IsMouseKeyDown( buttonCode_t Key )
{
	return le::g_inputSystem->IsMouseKeyDown( ( le::BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is mouse key up
// ------------------------------------------------------------------------------------ //
int InputSystem_IsMouseKeyUp( buttonCode_t Key )
{
	return le::g_inputSystem->IsMouseKeyUp( ( le::BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is mouse wheel scrolled
// ------------------------------------------------------------------------------------ //
int InputSystem_IsMouseWheel( buttonCode_t Key )
{
	return le::g_inputSystem->IsMouseWheel( ( le::BUTTON_CODE ) Key );
}
