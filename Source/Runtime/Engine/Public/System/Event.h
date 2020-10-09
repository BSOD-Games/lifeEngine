// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef EVENT_H
#define EVENT_H

#include "ButtonCode.h"

namespace le
{
	struct SEvent
	{
		/* Constructor */
		SEvent() :
			type( ET_None )
		{}

		enum EEventType
		{
			ET_None,
			ET_WindowClose,
			ET_WindowResize,
			ET_WindowFocusGained,
			ET_WindowFocusLost,
			ET_KeyPressed,
			ET_KeyReleased,
			ET_MousePressed,
			ET_MouseReleased,
			ET_MouseMove,
			ET_MouseWheel,
			ET_TextInput
		};

		struct SKeyEvent
		{
			EButtonCode			code;
			bool				isAlt;
			bool				isControl;
			bool				isShift;
			bool				isSuper;
			bool				isCapsLock;
			bool				isNumLock;
		};

		struct SMouseButtonEvent
		{
			EButtonCode			code;
			int					x;
			int					y;
		};

		struct SMouseMoveEvent
		{
			int			x;
			int			y;
			int			xDirection;
			int			yDirection;
		};

		struct SMouseWheelEvent
		{
			int			x;
			int			y;
		};

		struct SWindowResizeEvent
		{
			float		width;
			float		height;
		};

		struct STextInputEvent
		{
			char*		text;
		};

		union UEvents
		{
			SWindowResizeEvent	windowResize;
			SKeyEvent			key;
			SMouseButtonEvent	mouseButton;
			SMouseMoveEvent		mouseMove;
			SMouseWheelEvent	mouseWheel;
			STextInputEvent		textInputEvent;
		};

		EEventType			type;
		UEvents				event;
	};
}

#endif // !EVENT_H