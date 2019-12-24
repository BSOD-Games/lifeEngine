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
#include "common/buttoncode.h"
#include "engine/iinputsysteminternal.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	enum BUTTON_EVENTS
	{
		BE_NONE,
		BE_PRESSED,
		BE_RELEASED,
		BE_SCROLLED
	};

	//---------------------------------------------------------------------//

	struct BindDescriptor
	{
		BindDescriptor()
		{}

		BindDescriptor( BUTTON_CODE ButtonTrigger, const std::string& Command ) :
			buttonTrigger( ButtonTrigger ),
			command( Command )
		{}

		BUTTON_CODE				buttonTrigger;
		std::string				command;
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
		virtual bool				IsKeyDown( BUTTON_CODE Key );
		virtual bool				IsKeyUp( BUTTON_CODE Key );
		virtual	bool				IsMouseKeyDown( BUTTON_CODE Key );
		virtual	bool				IsMouseKeyUp( BUTTON_CODE Key );
		virtual bool				IsMouseWheel( BUTTON_CODE Wheel );

		virtual const Vector2D_t&	GetMousePosition() const;
		virtual const Vector2D_t&	GetMouseOffset() const;
		virtual float				GetMouseSensitivity() const;

		// InputSystem
		InputSystem();
		~InputSystem();

		friend void					CMD_Bind( le::UInt32_t CountArguments, const char** Arguments );

	private:
		BUTTON_EVENTS						buttonEvents[ BC_COUNT ];
		Vector2D_t							mousePosition;
		Vector2D_t							mouseOffset;

		std::vector< BindDescriptor >		binds;
	};	

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !INPUTSYSTEM_H