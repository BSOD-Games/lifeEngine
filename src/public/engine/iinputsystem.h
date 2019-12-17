//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IINPUTSYSTEM_H
#define IINPUTSYSTEM_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	enum KEYBOARD_KEY;
	enum MOUSE_KEY;
	enum MOUSE_WHEEL;

	//---------------------------------------------------------------------//

	class IInputSystem
	{
	public:
		virtual bool				IsKeyDown( KEYBOARD_KEY Key ) = 0;
		virtual bool				IsKeyUp( KEYBOARD_KEY Key ) = 0;
		virtual	bool				IsMouseKeyDown( MOUSE_KEY Key ) = 0;
		virtual	bool				IsMouseKeyUp( MOUSE_KEY Key ) = 0;
		virtual bool				IsMouseWheel( MOUSE_WHEEL Wheel ) = 0;

		virtual const Vector2D_t&	GetMousePosition() const = 0;
		virtual const Vector2D_t&	GetMouseOffset() const = 0;
		virtual float				GetMouseSensitivity() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IINPUTSYSTEM_H

