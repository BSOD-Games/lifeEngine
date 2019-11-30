//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef EVENT_H
#define EVENT_H

#include "keyboardkey.h"
#include "mousekey.h"

//----------------------------------------------------------------------//

namespace le
{
	//----------------------------------------------------------------------//
	
	struct Event
	{
		//----------------------------------------------------------------------//

		Event() :
			type( ET_NONE ),
			windowResize(),
			key(), 
			mouseButton(), 
			mouseMove(), 
			mouseWheel(), 
			textInputEvent()
		{}

		~Event()
		{}

		//----------------------------------------------------------------------//

		enum EVENT_TYPE
		{
			ET_NONE,				
			ET_WINDOW_CLOSE,		
			ET_WINDOW_RESIZE,		
			ET_WINDOW_FOCUS_GAINED,	
			ET_WINDOW_FOCUS_LOST,	
			ET_KEY_PRESSED,			
			ET_KEY_RELEASED,		
			ET_MOUSE_PRESSED,		
			ET_MOUSE_RELEASED,		
			ET_MOUSE_MOVE,			
			ET_MOUSE_WHEEL,			
			ET_TEXT_INPUT			
		};

		//----------------------------------------------------------------------//

		struct KeyEvent
		{
			KeyEvent() : 
				code( KK_UNKNOWN ), 
				isAlt( false ), 
				isControl( false ), 
				isShift( false ), 
				isSuper( false ), 
				isCapsLock( false ), 
				isNumLock( false )
			{}

			~KeyEvent()
			{}

			KEYBOARD_KEY		code;			
			bool				isAlt;			
			bool				isControl;		
			bool				isShift;		
			bool				isSuper;		
			bool				isCapsLock;		
			bool				isNumLock;		
		};

		//----------------------------------------------------------------------//

		struct MouseButtonEvent
		{
			MouseButtonEvent() : 
				code( MK_UNKNOWN ), 
				x( 0 ), 
				y( 0 )
			{}

			~MouseButtonEvent()
			{}

			MOUSE_KEY			code;		
			int					x;			
			int					y;			
		};

		//----------------------------------------------------------------------//

		struct MouseMoveEvent
		{	
			MouseMoveEvent() : 
				x( 0 ), 
				y( 0 ), 
				xDirection( 0 ), 
				yDirection( 0 )
			{}

			~MouseMoveEvent()
			{}

			int			x;			
			int			y;			
			int			xDirection;	
			int			yDirection;	
		};

		//----------------------------------------------------------------------//
		
		struct MouseWheelEvent
		{
			MouseWheelEvent() : 
				x( 0 ), 
				y( 0 )
			{}

			~MouseWheelEvent()
			{}

			int			x;			
			int			y;			
		};

		//----------------------------------------------------------------------//

		struct WindowResizeEvent
		{
			WindowResizeEvent() : 
				width( 0 ), 
				height( 0 )
			{}

			~WindowResizeEvent()
			{}

			float		width;			
			float		height;			
		};

		//----------------------------------------------------------------------//

		struct TextInputEvent
		{
			TextInputEvent() : 
				text( "" )
			{}

			~TextInputEvent()
			{}

			char*			text;
		};

		//----------------------------------------------------------------------//

		EVENT_TYPE			type;

		union
		{		
			WindowResizeEvent	windowResize;	
			KeyEvent			key;			
			MouseButtonEvent	mouseButton;	
			MouseMoveEvent		mouseMove;		
			MouseWheelEvent		mouseWheel;		
			TextInputEvent		textInputEvent;	
		}; 
	};

	//----------------------------------------------------------------------//
}

//----------------------------------------------------------------------//

#endif // !EVENT_H
