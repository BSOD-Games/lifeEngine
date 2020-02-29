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

#include "common/buttoncode.h"

//----------------------------------------------------------------------//

namespace le
{
	//----------------------------------------------------------------------//
	
	struct Event
	{
		//----------------------------------------------------------------------//

		Event() :
            type( ET_NONE )
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
			BUTTON_CODE			code;
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
			BUTTON_CODE			code;
			int					x;			
			int					y;			
		};

		//----------------------------------------------------------------------//

		struct MouseMoveEvent
		{	
			int			x;			
			int			y;			
			int			xDirection;	
			int			yDirection;	
		};

		//----------------------------------------------------------------------//
		
		struct MouseWheelEvent
		{
			int			x;			
			int			y;			
		};

		//----------------------------------------------------------------------//

		struct WindowResizeEvent
		{
			float		width;			
			float		height;			
		};

		//----------------------------------------------------------------------//

		struct TextInputEvent
		{
			char*			text;
		};

		//----------------------------------------------------------------------//

        union Events
		{		
			WindowResizeEvent	windowResize;	
			KeyEvent			key;			
			MouseButtonEvent	mouseButton;	
			MouseMoveEvent		mouseMove;		
			MouseWheelEvent		mouseWheel;		
			TextInputEvent		textInputEvent;	
		}; 

        //----------------------------------------------------------------------//

        EVENT_TYPE			type;
        Events              events;
	};

	//----------------------------------------------------------------------//
}

//----------------------------------------------------------------------//

#endif // !EVENT_H
