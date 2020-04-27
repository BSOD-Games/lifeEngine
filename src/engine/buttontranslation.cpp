//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "common/buttoncode.h"
#include "buttontranslation.h"

static SDL_Scancode		g_scanCodeToButtonCode[] =
{
	SDL_SCANCODE_UNKNOWN,				// BC_KEY_NONE
	SDL_SCANCODE_0,						// BC_KEY_0
	SDL_SCANCODE_1,						// BC_KEY_1
	SDL_SCANCODE_2,						// BC_KEY_2
	SDL_SCANCODE_3,						// BC_KEY_3
	SDL_SCANCODE_4,						// BC_KEY_4
	SDL_SCANCODE_5,						// BC_KEY_5
	SDL_SCANCODE_6,						// BC_KEY_6
	SDL_SCANCODE_7,						// BC_KEY_7
	SDL_SCANCODE_8,						// BC_KEY_8
	SDL_SCANCODE_9,						// BC_KEY_9

	SDL_SCANCODE_A,						// BC_KEY_A
	SDL_SCANCODE_B,						// BC_KEY_B
	SDL_SCANCODE_C,						// BC_KEY_C
	SDL_SCANCODE_D,						// BC_KEY_D
	SDL_SCANCODE_E,						// BC_KEY_E
	SDL_SCANCODE_F,						// BC_KEY_F
	SDL_SCANCODE_G,						// BC_KEY_G
	SDL_SCANCODE_H,						// BC_KEY_H
	SDL_SCANCODE_I,						// BC_KEY_I
	SDL_SCANCODE_J,						// BC_KEY_J
	SDL_SCANCODE_K,						// BC_KEY_K
	SDL_SCANCODE_L,						// BC_KEY_L
	SDL_SCANCODE_M,						// BC_KEY_M
	SDL_SCANCODE_N,						// BC_KEY_N
	SDL_SCANCODE_O,						// BC_KEY_O
	SDL_SCANCODE_P,						// BC_KEY_P
	SDL_SCANCODE_Q,						// BC_KEY_Q
	SDL_SCANCODE_R,						// BC_KEY_R
	SDL_SCANCODE_S,						// BC_KEY_S
	SDL_SCANCODE_T,						// BC_KEY_T
	SDL_SCANCODE_U,						// BC_KEY_U
	SDL_SCANCODE_V,						// BC_KEY_V
	SDL_SCANCODE_W,						// BC_KEY_W
	SDL_SCANCODE_X,						// BC_KEY_X
	SDL_SCANCODE_Y,						// BC_KEY_Y
	SDL_SCANCODE_Z,						// BC_KEY_Z

	SDL_SCANCODE_KP_0,					// BC_KEY_PAD_0
	SDL_SCANCODE_KP_1,					// BC_KEY_PAD_1
	SDL_SCANCODE_KP_2,					// BC_KEY_PAD_2
	SDL_SCANCODE_KP_3,					// BC_KEY_PAD_3
	SDL_SCANCODE_KP_4,					// BC_KEY_PAD_4
	SDL_SCANCODE_KP_5,					// BC_KEY_PAD_5
	SDL_SCANCODE_KP_6,					// BC_KEY_PAD_6
	SDL_SCANCODE_KP_7,					// BC_KEY_PAD_7
	SDL_SCANCODE_KP_8,					// BC_KEY_PAD_8
	SDL_SCANCODE_KP_9,					// BC_KEY_PAD_9
	SDL_SCANCODE_KP_DIVIDE,				// BC_KEY_PAD_DIVIDE
	SDL_SCANCODE_KP_MULTIPLY,			// BC_KEY_PAD_MULTIPLY
	SDL_SCANCODE_KP_MINUS,				// BC_KEY_PAD_MINUS
	SDL_SCANCODE_KP_PLUS,				// BC_KEY_PAD_PLUS
	SDL_SCANCODE_KP_ENTER,				// BC_KEY_PAD_ENTER
	SDL_SCANCODE_KP_DECIMAL,			// BC_KEY_PAD_DECIMAL

	SDL_SCANCODE_LEFTBRACKET,			// BC_KEY_LBRACKET
	SDL_SCANCODE_RIGHTBRACKET,			// BC_KEY_RBRACKET
	SDL_SCANCODE_SEMICOLON,				// BC_KEY_SEMICOLON
	SDL_SCANCODE_APOSTROPHE,			// BC_KEY_APOSTROPHE
	SDL_SCANCODE_GRAVE,					// BC_KEY_BACKQUOTE
	SDL_SCANCODE_COMMA,					// BC_KEY_COMMA
	SDL_SCANCODE_PERIOD,				// BC_KEY_PERIOD
	SDL_SCANCODE_SLASH,					// BC_KEY_SLASH
	SDL_SCANCODE_BACKSLASH,				// BC_KEY_BACKSLASH
	SDL_SCANCODE_MINUS,					// BC_KEY_MINUS
	SDL_SCANCODE_EQUALS,				// BC_KEY_EQUAL
	SDL_SCANCODE_RETURN,				// BC_KEY_ENTER
	SDL_SCANCODE_SPACE,					// BC_KEY_SPACE
	SDL_SCANCODE_BACKSPACE,				// BC_KEY_BACKSPACE
	SDL_SCANCODE_TAB,					// BC_KEY_TAB
	SDL_SCANCODE_CAPSLOCK,				// BC_KEY_CAPSLOCK
	SDL_SCANCODE_NUMLOCKCLEAR,			// BC_KEY_NUMLOCK
	SDL_SCANCODE_ESCAPE,				// BC_KEY_ESCAPE
	SDL_SCANCODE_SCROLLLOCK,			// BC_KEY_SCROLLLOCK
	SDL_SCANCODE_INSERT,				// BC_KEY_INSERT
	SDL_SCANCODE_DELETE,				// BC_KEY_DELETE
	SDL_SCANCODE_HOME,					// BC_KEY_HOME
	SDL_SCANCODE_END,					// BC_KEY_END
	SDL_SCANCODE_PAGEUP,				// BC_KEY_PAGEUP
	SDL_SCANCODE_PAGEDOWN,				// BC_KEY_PAGEDOWN
	SDL_SCANCODE_PAUSE,					// BC_KEY_BREAK
	SDL_SCANCODE_LSHIFT,				// BC_KEY_LSHIFT
	SDL_SCANCODE_RSHIFT,				// BC_KEY_RSHIFT
	SDL_SCANCODE_LALT,					// BC_KEY_LALT
	SDL_SCANCODE_RALT,					// BC_KEY_RALT
	SDL_SCANCODE_LCTRL,					// BC_KEY_LCONTROL
	SDL_SCANCODE_RCTRL,					// BC_KEY_RCONTROL
	SDL_SCANCODE_LGUI,					// BC_KEY_LWIN
	SDL_SCANCODE_RGUI,					// BC_KEY_RWIN
	SDL_SCANCODE_APPLICATION,			// BC_KEY_APP
	SDL_SCANCODE_UP,					// BC_KEY_UP
	SDL_SCANCODE_LEFT,					// BC_KEY_LEFT
	SDL_SCANCODE_DOWN,					// BC_KEY_DOWN
	SDL_SCANCODE_RIGHT,					// BC_KEY_RIGHT

	SDL_SCANCODE_F1,					// BC_KEY_F1
	SDL_SCANCODE_F2,					// BC_KEY_F2
	SDL_SCANCODE_F3,					// BC_KEY_F3
	SDL_SCANCODE_F4,					// BC_KEY_F4
	SDL_SCANCODE_F5,					// BC_KEY_F5
	SDL_SCANCODE_F6,					// BC_KEY_F6
	SDL_SCANCODE_F7,					// BC_KEY_F7
	SDL_SCANCODE_F8,					// BC_KEY_F8
	SDL_SCANCODE_F9,					// BC_KEY_F9
	SDL_SCANCODE_F10,					// BC_KEY_F10
	SDL_SCANCODE_F11,					// BC_KEY_F11
	SDL_SCANCODE_F12					// BC_KEY_F12
};

static const char* g_buttonName[] =
{
	"",						// BC_KEY_NONE
	"0",					// BC_KEY_0
	"1",					// BC_KEY_1
	"2",					// BC_KEY_2
	"3",					// BC_KEY_3
	"4",					// BC_KEY_4
	"5",					// BC_KEY_5
	"6",					// BC_KEY_6
	"7",					// BC_KEY_7
	"8",					// BC_KEY_8
	"9",					// BC_KEY_9
	"a",					// BC_KEY_A
	"b",					// BC_KEY_B
	"c",					// BC_KEY_C
	"d",					// BC_KEY_D
	"e",					// BC_KEY_E
	"f",					// BC_KEY_F
	"g",					// BC_KEY_G
	"h",					// BC_KEY_H
	"i",					// BC_KEY_I
	"j",					// BC_KEY_J
	"k",					// BC_KEY_K
	"l",					// BC_KEY_L
	"m",					// BC_KEY_M
	"n",					// BC_KEY_N
	"o",					// BC_KEY_O
	"p",					// BC_KEY_P
	"q",					// BC_KEY_Q
	"r",					// BC_KEY_R
	"s",					// BC_KEY_S
	"t",					// BC_KEY_T
	"u",					// BC_KEY_U
	"v",					// BC_KEY_V
	"w",					// BC_KEY_W
	"x",					// BC_KEY_X
	"y",					// BC_KEY_Y
	"z",					// BC_KEY_Z
	"kp_0",					// BC_KEY_PAD_0
	"kp_1",					// BC_KEY_PAD_1
	"kp_2",					// BC_KEY_PAD_2
	"kp_3",					// BC_KEY_PAD_3
	"kp_4",					// BC_KEY_PAD_4
	"kp_5",					// BC_KEY_PAD_5
	"kp_6",					// BC_KEY_PAD_6
	"kp_7",					// BC_KEY_PAD_7
	"kp_8",					// BC_KEY_PAD_8
	"kp_9",					// BC_KEY_PAD_9
	"kp_slash",				// BC_KEY_PAD_DIVIDE
	"kp_multiply",			// BC_KEY_PAD_MULTIPLY
	"kp_minus",				// BC_KEY_PAD_MINUS
	"kp_plus",				// BC_KEY_PAD_PLUS
	"kp_enter",				// BC_KEY_PAD_ENTER
	"kp_del",				// BC_KEY_PAD_DECIMAL
	"[",					// BC_KEY_LBRACKET
	"]",					// BC_KEY_RBRACKET
	"semicolon",			// BC_KEY_SEMICOLON
	"'",					// BC_KEY_APOSTROPHE
	"`",					// BC_KEY_BACKQUOTE
	",",					// BC_KEY_COMMA
	".",					// BC_KEY_PERIOD
	"/",					// BC_KEY_SLASH
	"\\",					// BC_KEY_BACKSLASH
	"-",					// BC_KEY_MINUS
	"=",					// BC_KEY_EQUAL
	"enter",				// BC_KEY_ENTER
	"space",				// BC_KEY_SPACE
	"backspace",			// BC_KEY_BACKSPACE
	"tab",					// BC_KEY_TAB
	"capslock",				// BC_KEY_CAPSLOCK
	"numlock",				// BC_KEY_NUMLOCK
	"esc",					// BC_KEY_ESCAPE
	"scrolllock",			// BC_KEY_SCROLLLOCK
	"ins",					// BC_KEY_INSERT
	"del",					// BC_KEY_DELETE
	"home",					// BC_KEY_HOME
	"end",					// BC_KEY_END
	"pgup",					// BC_KEY_PAGEUP
	"pgdn",					// BC_KEY_PAGEDOWN
	"pause",				// BC_KEY_BREAK
	"shift",				// BC_KEY_LSHIFT
	"rshift",				// BC_KEY_RSHIFT
	"alt",					// BC_KEY_LALT
	"ralt",					// BC_KEY_RALT
	"ctrl",					// BC_KEY_LCONTROL
	"rctrl",				// BC_KEY_RCONTROL
	"win",					// BC_KEY_LWIN
	"rwin",					// BC_KEY_RWIN
	"app",					// BC_KEY_APP
	"up",					// BC_KEY_UP
	"left",					// BC_KEY_LEFT
	"down",					// BC_KEY_DOWN
	"right",				// BC_KEY_RIGHT
	"f1",					// BC_KEY_F1
	"f2",					// BC_KEY_F2
	"f3",					// BC_KEY_F3
	"f4",					// BC_KEY_F4
	"f5",					// BC_KEY_F5
	"f6",					// BC_KEY_F6
	"f7",					// BC_KEY_F7
	"f8",					// BC_KEY_F8
	"f9",					// BC_KEY_F9
	"f10",					// BC_KEY_F10
	"f11",					// BC_KEY_F11
	"f12",					// BC_KEY_F12

	"mouse1",				// BC_MOUSE_1
	"mouse2",				// BC_MOUSE_2
	"mouse3",				// BC_MOUSE_3
	"mouse4",				// BC_MOUSE_4
	"mouse5",				// BC_MOUSE_5
	"mouse6",				// BC_MOUSE_6
	"mouse7",				// BC_MOUSE_7
	"mouse8",				// BC_MOUSE_8
	"mwheelup",				// BC_MOUSE_WHEEL_UP
	"mwheeldown"			// BC_MOUSE_WHEEL_DOWN
};

// ------------------------------------------------------------------------------------ //
// Конвертировать сканкод в код клавиш движка
// ------------------------------------------------------------------------------------ //
le::BUTTON_CODE le::ButtonCode_ScanCodeToButtonCode( SDL_Scancode Scancode )
{
	for ( UInt32_t index = 0; index < BC_KEY_COUNT; ++index )
		if ( g_scanCodeToButtonCode[ index ] == Scancode )
			return ( BUTTON_CODE ) index;

	return BC_NONE;
}

// ------------------------------------------------------------------------------------ //
// Конвертировать код клавиш мыши в код клавиш движка
// ------------------------------------------------------------------------------------ //
le::BUTTON_CODE le::ButtonCode_MouseButtonToButtonCode( UInt8_t ButtonIndex )
{
	switch ( ButtonIndex )
	{
	case 1:			return BC_MOUSE_1;
	case 2:			return BC_MOUSE_2;
	case 3:			return BC_MOUSE_3;
	case 4:			return BC_MOUSE_4;
	case 5:			return BC_MOUSE_5;
	case 6:			return BC_MOUSE_6;
	case 7:			return BC_MOUSE_7;
	case 8:			return BC_MOUSE_8;
	default:		return BC_NONE;
	}
}

// ------------------------------------------------------------------------------------ //
// Конвертировать название клавиш в код клавиш движка
// ------------------------------------------------------------------------------------ //
le::BUTTON_CODE le::ButtonCode_StringToButtonCode( const char* String )
{
	if ( String )
		for ( UInt32_t index = 0; index < BC_COUNT; ++index )
			if ( strcmp( g_buttonName[ index ], String ) == 0 )
				return ( BUTTON_CODE ) index;

	return BC_NONE;
}

// ------------------------------------------------------------------------------------ //
// Convert BUTTON_CODE to SDL_ScanCode
// ------------------------------------------------------------------------------------ //
SDL_Scancode le::ButtonCode_ButtonCodeToScanCode( le::BUTTON_CODE ButtonCode )
{
	if ( ButtonCode < le::BC_KEY_FIRST || ButtonCode > le::BC_KEY_LAST )
		return SDL_SCANCODE_UNKNOWN;

	return g_scanCodeToButtonCode[ ButtonCode ];
}

// ------------------------------------------------------------------------------------ //
// Convert BUTTON_CODE to UInt8_t
// ------------------------------------------------------------------------------------ //
le::UInt8_t le::ButtonCode_ButtonCodeToMouseButton( le::BUTTON_CODE ButtonCode )
{
	switch ( ButtonCode )
	{
	case BC_MOUSE_1:			return 1;
	case BC_MOUSE_2:			return 2;
	case BC_MOUSE_3:			return 3;
	case BC_MOUSE_4:			return 4;
	case BC_MOUSE_5:			return 5;
	case BC_MOUSE_6:			return 6;
	case BC_MOUSE_7:			return 7;
	case BC_MOUSE_8:			return 8;
	default:					return 0;
	}
}
