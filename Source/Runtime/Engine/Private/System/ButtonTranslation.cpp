// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "ButtonTranslation.h"

static SDL_Scancode		GScanCodeToButtonCode[] =
{
	SDL_SCANCODE_UNKNOWN,				// BC_KeyNone
	SDL_SCANCODE_0,						// BC_Key0
	SDL_SCANCODE_1,						// BC_Key1
	SDL_SCANCODE_2,						// BC_Key2
	SDL_SCANCODE_3,						// BC_Key3
	SDL_SCANCODE_4,						// BC_Key4
	SDL_SCANCODE_5,						// BC_Key5
	SDL_SCANCODE_6,						// BC_Key6
	SDL_SCANCODE_7,						// BC_Key7
	SDL_SCANCODE_8,						// BC_Key8
	SDL_SCANCODE_9,						// BC_Key9

	SDL_SCANCODE_A,						// BC_KeyA
	SDL_SCANCODE_B,						// BC_KeyB
	SDL_SCANCODE_C,						// BC_KeyC
	SDL_SCANCODE_D,						// BC_KeyD
	SDL_SCANCODE_E,						// BC_KeyE
	SDL_SCANCODE_F,						// BC_KeyF
	SDL_SCANCODE_G,						// BC_KeyG
	SDL_SCANCODE_H,						// BC_KeyH
	SDL_SCANCODE_I,						// BC_KeyI
	SDL_SCANCODE_J,						// BC_KeyJ
	SDL_SCANCODE_K,						// BC_KeyK
	SDL_SCANCODE_L,						// BC_KeyL
	SDL_SCANCODE_M,						// BC_KeyM
	SDL_SCANCODE_N,						// BC_KeyN
	SDL_SCANCODE_O,						// BC_KeyO
	SDL_SCANCODE_P,						// BC_KeyP
	SDL_SCANCODE_Q,						// BC_KeyQ
	SDL_SCANCODE_R,						// BC_KeyR
	SDL_SCANCODE_S,						// BC_KeyS
	SDL_SCANCODE_T,						// BC_KeyT
	SDL_SCANCODE_U,						// BC_KeyU
	SDL_SCANCODE_V,						// BC_KeyV
	SDL_SCANCODE_W,						// BC_KeyW
	SDL_SCANCODE_X,						// BC_KeyX
	SDL_SCANCODE_Y,						// BC_KeyY
	SDL_SCANCODE_Z,						// BC_KeyZ

	SDL_SCANCODE_KP_0,					// BC_KeyPad0
	SDL_SCANCODE_KP_1,					// BC_KeyPad1
	SDL_SCANCODE_KP_2,					// BC_KeyPad2
	SDL_SCANCODE_KP_3,					// BC_KeyPad3
	SDL_SCANCODE_KP_4,					// BC_KeyPad4
	SDL_SCANCODE_KP_5,					// BC_KeyPad5
	SDL_SCANCODE_KP_6,					// BC_KeyPad6
	SDL_SCANCODE_KP_7,					// BC_KeyPad7
	SDL_SCANCODE_KP_8,					// BC_KeyPad8
	SDL_SCANCODE_KP_9,					// BC_KeyPad9
	SDL_SCANCODE_KP_DIVIDE,				// BC_KeyPadDivide
	SDL_SCANCODE_KP_MULTIPLY,			// BC_KeyPadMultip
	SDL_SCANCODE_KP_MINUS,				// BC_KeyPadMinus
	SDL_SCANCODE_KP_PLUS,				// BC_KeyPadPlus
	SDL_SCANCODE_KP_ENTER,				// BC_KeyPadEnter
	SDL_SCANCODE_KP_DECIMAL,			// BC_KeyPadDecimal

	SDL_SCANCODE_LEFTBRACKET,			// BC_KeyLBracket
	SDL_SCANCODE_RIGHTBRACKET,			// BC_KeyRBracket
	SDL_SCANCODE_SEMICOLON,				// BC_KeySenicolon
	SDL_SCANCODE_APOSTROPHE,			// BC_KeyApostrophe
	SDL_SCANCODE_GRAVE,					// BC_KeyBackQuote
	SDL_SCANCODE_COMMA,					// BC_KeyComma
	SDL_SCANCODE_PERIOD,				// BC_KeyPeriod
	SDL_SCANCODE_SLASH,					// BC_KeySlash
	SDL_SCANCODE_BACKSLASH,				// BC_KeyBackslash
	SDL_SCANCODE_MINUS,					// BC_KeyMinus
	SDL_SCANCODE_EQUALS,				// BC_KeyEqual
	SDL_SCANCODE_RETURN,				// BC_KeyEnter
	SDL_SCANCODE_SPACE,					// BC_KeySpace
	SDL_SCANCODE_BACKSPACE,				// BC_KeyBackspace
	SDL_SCANCODE_TAB,					// BC_KeyTab
	SDL_SCANCODE_CAPSLOCK,				// BC_KeyCapsLock
	SDL_SCANCODE_NUMLOCKCLEAR,			// BC_KeyNumLock
	SDL_SCANCODE_ESCAPE,				// BC_KeyEscape
	SDL_SCANCODE_SCROLLLOCK,			// BC_KeyScrollLock
	SDL_SCANCODE_INSERT,				// BC_KeyInsert
	SDL_SCANCODE_DELETE,				// BC_KeyDelete
	SDL_SCANCODE_HOME,					// BC_KeyHome
	SDL_SCANCODE_END,					// BC_KeyEnd
	SDL_SCANCODE_PAGEUP,				// BC_KeyPageUp
	SDL_SCANCODE_PAGEDOWN,				// BC_KeyPageDown
	SDL_SCANCODE_PAUSE,					// BC_KeyBreak
	SDL_SCANCODE_LSHIFT,				// BC_KeyLShift
	SDL_SCANCODE_RSHIFT,				// BC_KeyRShift
	SDL_SCANCODE_LALT,					// BC_KeyLAlt
	SDL_SCANCODE_RALT,					// BC_KeyRAlt
	SDL_SCANCODE_LCTRL,					// BC_KeyLControl
	SDL_SCANCODE_RCTRL,					// BC_KeyRControl
	SDL_SCANCODE_LGUI,					// BC_KeyLWin
	SDL_SCANCODE_RGUI,					// BC_KeyRWin
	SDL_SCANCODE_APPLICATION,			// BC_KeyApp
	SDL_SCANCODE_UP,					// BC_KeyUp
	SDL_SCANCODE_LEFT,					// BC_KeyLeft
	SDL_SCANCODE_DOWN,					// BC_KeyDown
	SDL_SCANCODE_RIGHT,					// BC_KeyRight

	SDL_SCANCODE_F1,					// BC_KeyF1
	SDL_SCANCODE_F2,					// BC_KeyF2
	SDL_SCANCODE_F3,					// BC_KeyF3
	SDL_SCANCODE_F4,					// BC_KeyF4
	SDL_SCANCODE_F5,					// BC_KeyF5
	SDL_SCANCODE_F6,					// BC_KeyF6
	SDL_SCANCODE_F7,					// BC_KeyF7
	SDL_SCANCODE_F8,					// BC_KeyF8
	SDL_SCANCODE_F9,					// BC_KeyF9
	SDL_SCANCODE_F10,					// BC_KeyF10
	SDL_SCANCODE_F11,					// BC_KeyF11
	SDL_SCANCODE_F12					// BC_KeyF12
};

static const char* GButtonName[] =
{
	"",						// BC_KeyNone
	"0",					// BC_Key0
	"1",					// BC_Key1
	"2",					// BC_Key2
	"3",					// BC_Key3
	"4",					// BC_Key4
	"5",					// BC_Key5
	"6",					// BC_Key6
	"7",					// BC_Key7
	"8",					// BC_Key8
	"9",					// BC_Key9
	"a",					// BC_KeyA
	"b",					// BC_KeyB
	"c",					// BC_KeyC
	"d",					// BC_KeyD
	"e",					// BC_KeyE
	"f",					// BC_KeyF
	"g",					// BC_KeyG
	"h",					// BC_KeyH
	"i",					// BC_KeyI
	"j",					// BC_KeyJ
	"k",					// BC_KeyK
	"l",					// BC_KeyL
	"m",					// BC_KeyM
	"n",					// BC_KeyN
	"o",					// BC_KeyO
	"p",					// BC_KeyP
	"q",					// BC_KeyQ
	"r",					// BC_KeyR
	"s",					// BC_KeyS
	"t",					// BC_KeyT
	"u",					// BC_KeyU
	"v",					// BC_KeyV
	"w",					// BC_KeyW
	"x",					// BC_KeyX
	"y",					// BC_KeyY
	"z",					// BC_KeyZ
	"kp_0",					// BC_KeyPad0
	"kp_1",					// BC_KeyPad1
	"kp_2",					// BC_KeyPad2
	"kp_3",					// BC_KeyPad3
	"kp_4",					// BC_KeyPad4
	"kp_5",					// BC_KeyPad5
	"kp_6",					// BC_KeyPad6
	"kp_7",					// BC_KeyPad7
	"kp_8",					// BC_KeyPad8
	"kp_9",					// BC_KeyPad9
	"kp_slash",				// BC_KeyPadDivide
	"kp_multiply",			// BC_KeyPadMultiply
	"kp_minus",				// BC_KeyPadMinus
	"kp_plus",				// BC_KeyPadPlus
	"kp_enter",				// BC_KeyPadEnter
	"kp_del",				// BC_KeyPadDecimal
	"[",					// BC_KeyLBracket
	"]",					// BC_KeyRBracket
	"semicolon",			// BC_KeySemicolon
	"'",					// BC_KeyApostrophe
	"`",					// BC_KeyBackQuote
	",",					// BC_KeyComma
	".",					// BC_KeyPeriod
	"/",					// BC_KeySlash
	"\\",					// BC_KeyBackslash
	"-",					// BC_KeyMinus
	"=",					// BC_KeyEqual
	"enter",				// BC_KeyEnter
	"space",				// BC_KeySpace
	"backspace",			// BC_KeyBackspace
	"tab",					// BC_KeyTab
	"capslock",				// BC_KeyCapsLock
	"numlock",				// BC_KeyNumLock
	"esc",					// BC_KeyEscape
	"scrolllock",			// BC_KeyScrollLock
	"ins",					// BC_KeyInsert
	"del",					// BC_KeyDelete
	"home",					// BC_KeyHome
	"end",					// BC_KeyEnd
	"pgup",					// BC_KeyPageUp
	"pgdn",					// BC_KeyPageDown
	"pause",				// BC_KeyBreak
	"shift",				// BC_KeyLShift
	"rshift",				// BC_KeyRShift
	"alt",					// BC_KeyLAlt
	"ralt",					// BC_KeyRAlt
	"ctrl",					// BC_KeyLControl
	"rctrl",				// BC_KeyRControl
	"win",					// BC_KeyLWin
	"rwin",					// BC_KeyRWin
	"app",					// BC_KeyApp
	"up",					// BC_KeyUp
	"left",					// BC_KeyLeft
	"down",					// BC_KeyDown
	"right",				// BC_KeyRight
	"f1",					// BC_KeyF1
	"f2",					// BC_KeyF2
	"f3",					// BC_KeyF3
	"f4",					// BC_KeyF4
	"f5",					// BC_KeyF5
	"f6",					// BC_KeyF6
	"f7",					// BC_KeyF7
	"f8",					// BC_KeyF8
	"f9",					// BC_KeyF9
	"f10",					// BC_KeyF10
	"f11",					// BC_KeyF11
	"f12",					// BC_KeyF12

	"mouse1",				// BC_Mouse1
	"mouse2",				// BC_Mouse2
	"mouse3",				// BC_Mouse3
	"mouse4",				// BC_Mouse4
	"mouse5",				// BC_Mouse5
	"mouse6",				// BC_Mouse6
	"mouse7",				// BC_Mouse7
	"mouse8",				// BC_Mouse8
	"mwheelup",				// BC_MouseWheelUp
	"mwheeldown"			// BC_MouseWheelDown
};

/**
 * Scancode to button code
 */
le::EButtonCode le::ScanCodeToButtonCode( SDL_Scancode InScancode )
{
	for ( uint32 index = 0; index < BC_KeyCount; ++index )
		if ( GScanCodeToButtonCode[ index ] == InScancode )
			return ( EButtonCode ) index;

	return BC_None;
}

/**
 * Button code to Scancode
 */
SDL_Scancode le::ButtonCodeToScanCode( EButtonCode InButtonCode )
{
	if ( InButtonCode < BC_KeyFirst || InButtonCode > BC_KeyLast )
		return SDL_SCANCODE_UNKNOWN;

	return GScanCodeToButtonCode[ InButtonCode ];
}

/**
 * Mouse button to button code
 */
le::EButtonCode le::MouseButtonToButtonCode( uint8 InButtonIndex )
{
	switch ( InButtonIndex )
	{
	case 1:			return BC_Mouse1;
	case 2:			return BC_Mouse2;
	case 3:			return BC_Mouse3;
	case 4:			return BC_Mouse4;
	case 5:			return BC_Mouse5;
	case 6:			return BC_Mouse6;
	case 7:			return BC_Mouse7;
	case 8:			return BC_Mouse8;
	default:		return BC_None;
	}
}

/**
 * Button code to mouse button
 */
le::uint8 le::ButtonCodeToMouseButton( EButtonCode InButtonCode )
{
	switch ( InButtonCode )
	{
	case BC_Mouse1:			return 1;
	case BC_Mouse2:			return 2;
	case BC_Mouse3:			return 3;
	case BC_Mouse4:			return 4;
	case BC_Mouse5:			return 5;
	case BC_Mouse6:			return 6;
	case BC_Mouse7:			return 7;
	case BC_Mouse8:			return 8;
	default:				return 0;
	}
}

/**
 * String to button code
 */
le::EButtonCode le::StringToButtonCode( const std::string& InString )
{
	if ( !InString.empty() )
		for ( uint32 index = 0; index < BC_Count; ++index )
			if ( !strcmp( GButtonName[ index ], InString.data() ) )
				return ( EButtonCode ) index;

	return BC_None;
}
