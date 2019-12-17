//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "common/event.h"
#include "common/keyboardkey.h"
#include "common/mousekey.h"
#include "engine/iinputsysteminternal.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class InputSystem : public IInputSystemInternal
	{
	public:
		// IInputSystemInternal
		virtual void				ApplyEvent( const Event& Event );
		virtual void				Clear();

		// IInputSystem
		virtual bool				IsKeyDown( KEYBOARD_KEY Key );
		virtual bool				IsKeyUp( KEYBOARD_KEY Key );
		virtual	bool				IsMouseKeyDown( MOUSE_KEY Key );
		virtual	bool				IsMouseKeyUp( MOUSE_KEY Key );
		virtual bool				IsMouseWheel( MOUSE_WHEEL Wheel );

		virtual const Vector2D_t&	GetMousePosition() const;
		virtual const Vector2D_t&	GetMouseOffset() const;
		virtual float				GetMouseSensitivity() const;

		// InputSystem
		InputSystem();
		~InputSystem();

	private:
		Event::EVENT_TYPE			keyboardKeyEvents[ KK_KEY_COUNT ];
		Event::EVENT_TYPE			mouseButtonEvents[ MK_KEY_COUNT ];

		MOUSE_WHEEL					mouseWheel;
		Vector2D_t					mousePosition;
		Vector2D_t					mouseOffset;
	};	

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !INPUTSYSTEM_H