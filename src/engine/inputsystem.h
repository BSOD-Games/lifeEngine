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

#include <string>
#include <vector>

#include "common/event.h"
#include "common/keyboardkey.h"
#include "common/mousekey.h"
#include "engine/iinputsysteminternal.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	template< typename T >
	struct BindDescriptor
	{
		BindDescriptor()
		{}

		BindDescriptor( T KeyTrigger, Event::EVENT_TYPE EventTrigger, const std::string& Command ) :
			keyTrigger( KeyTrigger ),
			eventTrigger( EventTrigger ),
			command( Command )
		{}

		Event::EVENT_TYPE			eventTrigger;
		T							keyTrigger;
		std::string					command;
	};

	//---------------------------------------------------------------------//

	class InputSystem : public IInputSystemInternal
	{
	public:
		// IInputSystemInternal
		virtual bool				Initialize( IEngine* Engine );
		virtual void				ApplyEvent( const Event& Event );
		virtual void				Update();
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

		friend void					CMD_Bind( le::UInt32_t CountArguments, const char** Arguments );

	private:
		
		Event::EVENT_TYPE									keyboardKeyEvents[ KK_KEY_COUNT ];
		Event::EVENT_TYPE									mouseButtonEvents[ MK_KEY_COUNT ];

		MOUSE_WHEEL											mouseWheel;
		Vector2D_t											mousePosition;
		Vector2D_t											mouseOffset;

		std::vector< BindDescriptor< KEYBOARD_KEY > >		keyboardBind;
		std::vector< BindDescriptor< MOUSE_KEY > >			mousedBind;
	};	

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !INPUTSYSTEM_H