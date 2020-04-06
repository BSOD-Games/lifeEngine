//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "../../global.h"
#include "engine/inputsystem.h"

namespace scripts_api
{
	#include "inputsystem.h"
}

// ------------------------------------------------------------------------------------ //
// Is key down
// ------------------------------------------------------------------------------------ //
int scripts_api::InputSystem_IsKeyDown( buttonCode_t Key )
{
	return le::g_inputSystem->IsKeyDown( ( le::BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is key up
// ------------------------------------------------------------------------------------ //
int scripts_api::InputSystem_IsKeyUp( buttonCode_t Key )
{
	return le::g_inputSystem->IsKeyUp( ( le::BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is mouse key down
// ------------------------------------------------------------------------------------ //
int scripts_api::InputSystem_IsMouseKeyDown( buttonCode_t Key )
{
	return le::g_inputSystem->IsMouseKeyDown( ( le::BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is mouse key up
// ------------------------------------------------------------------------------------ //
int scripts_api::InputSystem_IsMouseKeyUp( buttonCode_t Key )
{
	return le::g_inputSystem->IsMouseKeyUp( ( le::BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is mouse wheel scrolled
// ------------------------------------------------------------------------------------ //
int scripts_api::InputSystem_IsMouseWheel( buttonCode_t Key )
{
	return le::g_inputSystem->IsMouseWheel( ( le::BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Get mouse position
// ------------------------------------------------------------------------------------ //
scripts_api::vec2i_t scripts_api::InputSystem_GetMousePosition()
{
	const le::Vector2DInt_t&		vec = le::g_inputSystem->GetMousePosition();
	return { vec.x, vec.y };
}

// ------------------------------------------------------------------------------------ //
// Get mouse offset
// ------------------------------------------------------------------------------------ //
scripts_api::vec2i_t scripts_api::InputSystem_GetMouseOffset()
{
	const le::Vector2DInt_t&		vec = le::g_inputSystem->GetMouseOffset();
	return { vec.x, vec.y };
}

// ------------------------------------------------------------------------------------ //
// Get mouse offset
// ------------------------------------------------------------------------------------ //
float scripts_api::InputSystem_GetMouseSensitivity()
{
	return le::g_inputSystem->GetMouseSensitivity();
}
