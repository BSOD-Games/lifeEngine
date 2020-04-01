//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_INPUTSYSTEM_H
#define API_INPUTSYSTEM_H

//---------------------------------------------------------------------//

/* Buttons keyboard and mouse */
typedef enum
{
	BC_NONE = 0,

	BC_KEY_FIRST = 0,
	BC_KEY_NONE = BC_KEY_FIRST,

	BC_KEY_0,
	BC_KEY_1,
	BC_KEY_2,
	BC_KEY_3,
	BC_KEY_4,
	BC_KEY_5,
	BC_KEY_6,
	BC_KEY_7,
	BC_KEY_8,
	BC_KEY_9,
	BC_KEY_A,
	BC_KEY_B,
	BC_KEY_C,
	BC_KEY_D,
	BC_KEY_E,
	BC_KEY_F,
	BC_KEY_G,
	BC_KEY_H,
	BC_KEY_I,
	BC_KEY_J,
	BC_KEY_K,
	BC_KEY_L,
	BC_KEY_M,
	BC_KEY_N,
	BC_KEY_O,
	BC_KEY_P,
	BC_KEY_Q,
	BC_KEY_R,
	BC_KEY_S,
	BC_KEY_T,
	BC_KEY_U,
	BC_KEY_V,
	BC_KEY_W,
	BC_KEY_X,
	BC_KEY_Y,
	BC_KEY_Z,
	BC_KEY_PAD_0,
	BC_KEY_PAD_1,
	BC_KEY_PAD_2,
	BC_KEY_PAD_3,
	BC_KEY_PAD_4,
	BC_KEY_PAD_5,
	BC_KEY_PAD_6,
	BC_KEY_PAD_7,
	BC_KEY_PAD_8,
	BC_KEY_PAD_9,
	BC_KEY_PAD_DIVIDE,
	BC_KEY_PAD_MULTIPLY,
	BC_KEY_PAD_MINUS,
	BC_KEY_PAD_PLUS,
	BC_KEY_PAD_ENTER,
	BC_KEY_PAD_DECIMAL,
	BC_KEY_LBRACKET,
	BC_KEY_RBRACKET,
	BC_KEY_SEMICOLON,
	BC_KEY_APOSTROPHE,
	BC_KEY_BACKQUOTE,
	BC_KEY_COMMA,
	BC_KEY_PERIOD,
	BC_KEY_SLASH,
	BC_KEY_BACKSLASH,
	BC_KEY_MINUS,
	BC_KEY_EQUAL,
	BC_KEY_ENTER,
	BC_KEY_SPACE,
	BC_KEY_BACKSPACE,
	BC_KEY_TAB,
	BC_KEY_CAPSLOCK,
	BC_KEY_NUMLOCK,
	BC_KEY_ESCAPE,
	BC_KEY_SCROLLLOCK,
	BC_KEY_INSERT,
	BC_KEY_DELETE,
	BC_KEY_HOME,
	BC_KEY_END,
	BC_KEY_PAGEUP,
	BC_KEY_PAGEDOWN,
	BC_KEY_BREAK,
	BC_KEY_LSHIFT,
	BC_KEY_RSHIFT,
	BC_KEY_LALT,
	BC_KEY_RALT,
	BC_KEY_LCONTROL,
	BC_KEY_RCONTROL,
	BC_KEY_LWIN,
	BC_KEY_RWIN,
	BC_KEY_APP,
	BC_KEY_UP,
	BC_KEY_LEFT,
	BC_KEY_DOWN,
	BC_KEY_RIGHT,
	BC_KEY_F1,
	BC_KEY_F2,
	BC_KEY_F3,
	BC_KEY_F4,
	BC_KEY_F5,
	BC_KEY_F6,
	BC_KEY_F7,
	BC_KEY_F8,
	BC_KEY_F9,
	BC_KEY_F10,
	BC_KEY_F11,
	BC_KEY_F12,

	BC_KEY_LAST = BC_KEY_F12,
	BC_KEY_COUNT = BC_KEY_LAST - BC_KEY_FIRST + 1,

	BC_MOUSE_FIRST = BC_KEY_LAST + 1,

	BC_MOUSE_1 = BC_MOUSE_FIRST,
	BC_MOUSE_2,
	BC_MOUSE_3,
	BC_MOUSE_4,
	BC_MOUSE_5,
	BC_MOUSE_6,
	BC_MOUSE_7,
	BC_MOUSE_8,
	BC_MOUSE_WHEEL_UP,
	BC_MOUSE_WHEEL_DOWN,

	BC_MOUSE_LEFT = BC_MOUSE_1,
	BC_MOUSE_RIGHT = BC_MOUSE_2,
	BC_MOUSE_MIDDLE = BC_MOUSE_3,
	BC_MOUSE_LAST = BC_MOUSE_WHEEL_DOWN,
	BC_MOUSE_COUNT = BC_MOUSE_LAST - BC_MOUSE_FIRST + 1,

	BC_LAST = BC_MOUSE_LAST,
	BC_COUNT = BC_LAST - BC_KEY_FIRST + 1
} buttonCode_t;

/* Is key down */
extern int			InputSystem_IsKeyDown( buttonCode_t Key );

/* Is key up */
extern int			InputSystem_IsKeyUp( buttonCode_t Key );

/* Is mouse key down */
extern int			InputSystem_IsMouseKeyDown( buttonCode_t Key );

/* Is mouse key up */
extern int			InputSystem_IsMouseKeyUp( buttonCode_t Key );

/* Is mouse wheel */
extern int			InputSystem_IsMouseWheel( buttonCode_t Key );

//---------------------------------------------------------------------//

#endif // !API_INPUTSYSTEM_H
