//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef KEYBOARD_KEY_H
#define KEYBOARD_KEY_H

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	////////////////////////////////////////////////////////////
	/// Значения в этом перечислении основаны на стандарте страницы использования USB:
	/// https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
	////////////////////////////////////////////////////////////
	enum KEYBOARD_KEY
	{
		KK_UNKNOWN = 0,								///< Неизвестная клавиша

		//////////////////////////////////////////
		// Используемая страница 0x07
		//
		// Эти значения взяты из страницы использования 0x07 (страница USB клавиатуры)

		KK_A = 4,									///< A
		KK_B = 5,									///< B
		KK_C = 6,									///< C
		KK_D = 7,									///< D
		KK_E = 8,									///< E
		KK_F = 9,									///< F
		KK_G = 10,									///< G
		KK_H = 11,									///< H
		KK_I = 12,									///< I
		KK_J = 13,									///< J
		KK_K = 14,									///< K
		KK_L = 15,									///< L
		KK_M = 16,									///< M
		KK_N = 17,									///< N
		KK_O = 18,									///< O
		KK_P = 19,									///< P
		KK_Q = 20,									///< Q
		KK_R = 21,									///< R
		KK_S = 22,									///< S
		KK_T = 23,									///< T
		KK_U = 24,									///< U
		KK_V = 25,									///< V
		KK_W = 26,									///< W
		KK_X = 27,									///< X
		KK_Y = 28,									///< Y
		KK_Z = 29,									///< Z
		
		KK_1 = 30,									///< 1 и !
		KK_2 = 31,									///< 2 и @
		KK_3 = 32,									///< 3 и #
		KK_4 = 33,									///< 4 и $
		KK_5 = 34,									///< 5 и %
		KK_6 = 35,									///< 6 и ^
		KK_7 = 36,									///< 7 и &
		KK_8 = 37,									///< 8 и *
		KK_9 = 38,									///< 9 и (
		KK_0 = 39,									///< 0 и )

		KK_ENTER = 40,								///< Enter (Return)
		KK_ESCAPE = 41,								///< Escape
		KK_BACKSPACE = 42,							///< Backspace (Delete)
		KK_TAB = 43,								///< Tab
		KK_SPACE = 44,								///< Space

		KK_MINUS = 45,								///< - и _ (underscore)
		KK_EQUALS = 46,								///< = и +
		KK_LEFTBRACKET = 47,						///< [ и {
		KK_RIGHTBRACKET = 48,						///< ] и }
		KK_BACKSLASH = 49, 							///< \ и |
		KK_NONUSHASH = 50, 							///< Non-US # и ~
		KK_SEMICOLON = 51, 							///< ; и :
		KK_APOSTROPHE = 52,							///< ' и "
		KK_GRAVE = 53,								///< ` (Grave Accent) и ~ (Tilde)
		KK_COMMA = 54,								///< , и <
		KK_PERIOD = 55,								///< . и >
		KK_SLASH = 56,								///< / и ?

		KK_CAPSLOCK = 57,							///< Caps Lock

		KK_F1 = 58,									///< F1
		KK_F2 = 59,									///< F2
		KK_F3 = 60,									///< F3
		KK_F4 = 61,									///< F4
		KK_F5 = 62,									///< F5
		KK_F6 = 63,									///< F6
		KK_F7 = 64,									///< F7
		KK_F8 = 65,									///< F8
		KK_F9 = 66,									///< F9
		KK_F10 = 67,								///< F10
		KK_F11 = 68,								///< F11
		KK_F12 = 69,								///< F12

		KK_PRINTSCREEN = 70,						///< PrintScreen
		KK_SCROLLLOCK = 71,							///< Scroll Lock
		KK_PAUSE = 72,								///< Pause
		KK_INSERT = 73,								///< Insert
		KK_HOME = 74,								///< Home
		KK_PAGEUP = 75,								///< Page Up
		KK_DELETE = 76,								///< Delete Forward
		KK_END = 77,								///< End
		KK_PAGEDOWN = 78,							///< Page Down
		KK_RIGHT = 79,								///< Стрелка вправо
		KK_LEFT = 80,								///< Стрелка влево
		KK_DOWN = 81,								///< Стрелка вниз
		KK_UP = 82,									///< Стрелка вверх

		KK_NUMLOCK = 83,							///< Num Lock и Clear
		KK_KEYPAD_DIVIDE = 84,						///< Keypad /
		KK_KEYPAD_MULTIPLY = 85,					///< Keypad *
		KK_KEYPAD_MINUS = 86,						///< Keypad -
		KK_KEYPAD_PLUS = 87,						///< Keypad +
		KK_KEYPAD_ENTER = 88,						///< Keypad Enter
		KK_KEYPAD_1 = 89,							///< Keypad 1 и End
		KK_KEYPAD_2 = 90,							///< Keypad 2 и Стрелка вниз
		KK_KEYPAD_3 = 91,							///< Keypad 3 и PageDn
		KK_KEYPAD_4 = 92,							///< Keypad 4 и Стрелка влево
		KK_KEYPAD_5 = 93,							///< Keypad 5
		KK_KEYPAD_6 = 94,							///< Keypad 6 и Стрелка вправо
		KK_KEYPAD_7 = 95,							///< Keypad 7 и Home
		KK_KEYPAD_8 = 96,							///< Keypad 8 и Стрелка вверх
		KK_KEYPAD_9 = 97,							///< Keypad 9 и PageUp
		KK_KEYPAD_0 = 98,							///< Keypad 0 и Insert
		KK_KEYPAD_PERIOD = 99,						///< Keypad . и Delete

		KK_NONUS_BACKSLASH = 100,					///< Non-US \ и |
		KK_APPLICATION = 101,						///< Application (контекстное меню Windows, создать)
		KK_POWER = 102,								///< Power 
		KK_KEYPAD_EQUALS = 103,						///< Keypad =
		KK_F13 = 104,								///< F13 
		KK_F14 = 105,								///< F14
		KK_F15 = 106,								///< F15 
		KK_F16 = 107,								///< F16 
		KK_F17 = 108,								///< F17 
		KK_F18 = 109,								///< F18 
		KK_F19 = 110,								///< F19 
		KK_F20 = 111,								///< F20 
		KK_F21 = 112,								///< F21 
		KK_F22 = 113,								///< F22 
		KK_F23 = 114,								///< F23 
		KK_F24 = 115,								///< F24 
		KK_EXECUTE = 116,							///< Execute
		KK_HELP = 117,								///< Help
		KK_MENU = 118,								///< Menu
		KK_SELECT = 119,							///< Select
		KK_STOP = 120,								///< Stop
		KK_AGAIN = 121,								///< Again
		KK_UNDO = 122,								///< Undo
		KK_CUT = 123,								///< Cut
		KK_COPY = 124,								///< Copy
		KK_PASTE = 125,								///< Paste
		KK_FIND = 126,								///< Find
		KK_MUTE = 127,								///< Mute
		KK_VOLUMEUP = 128,							///< Volume Up
		KK_VOLUMEDOWN = 129,						///< Volume Down

		/* Не уверен, нужно ли это включать */
		/*     KK_LOCKING_CAPSLOCK = 130,  */
		/*     KK_LOCKING_NUMLOCK = 131, */
		/*     KK_LOCKING_SCROLLLOCK = 132, */

		KK_KEYPAD_COMMA = 133,						///< Comma			
		KK_KEYPAD_EQUALSIGN = 134,					///< Equal Sign

		KK_INTERNATIONAL1 = 135,					///< Используется на азиатских клавиатурах, см. сноски в USB doc
		KK_INTERNATIONAL2 = 136,					///< 
		KK_INTERNATIONAL3 = 137,					///< Yen
		KK_INTERNATIONAL4 = 138,					///< 
		KK_INTERNATIONAL5 = 139,					///< 
		KK_INTERNATIONAL6 = 140,					///< 
		KK_INTERNATIONAL7 = 141,					///< 
		KK_INTERNATIONAL8 = 142,					///<
		KK_INTERNATIONAL9 = 143,					///<
		KK_LANG1 = 144,								///< Hangul/English toggle
		KK_LANG2 = 145,								///< Hanja conversion
		KK_LANG3 = 146,								///< Katakana
		KK_LANG4 = 147,								///< Hiragana 
		KK_LANG5 = 148,								///< Zenkaku/Hankaku 
		KK_LANG6 = 149,								///< reserved 
		KK_LANG7 = 150,								///< reserved 
		KK_LANG8 = 151,								///< reserved 
		KK_LANG9 = 152,								///< reserved

		KK_ALTERASE = 153,							///< Alternate Erase
		KK_SYSREQ = 154,							///< SysReq/Attention
		KK_CANCEL = 155,							///< Cancel
		KK_CLEAR = 156,								///< Clear
		KK_PRIOR = 157,								///< Prior
		KK_RETURN = 158,							///< Return
		KK_SEPARATOR = 159,							///< Separator
		KK_OUT = 160,								///< Out
		KK_OPER = 161,								///< Oper
		KK_CLEARAGAIN = 162,						///< Clear/Again
		KK_CRSEL = 163,								///< CrSel/Props
		KK_EXSEL = 164,								///< ExSel

		KK_KEYPAD_00 = 176,							///< Keypad 00
		KK_KEYPAD_000 = 177,						///< Keypad 000
		KK_THOUSANDSSEPARATOR = 178,				///< Thousands Separator
		KK_DECIMALSEPARATOR = 179,					///< Decimal Separator
		KK_CURRENCYUNIT = 180,						///< Currency Unit
		KK_CURRENCYSUBUNIT = 181,					///< Currency Sub-unit
		KK_KEYPAD_LEFTPAREN = 182,					///< Keypad (
		KK_KEYPAD_RIGHTPAREN = 183,					///< Keypad )
		KK_KEYPAD_LEFTBRACE = 184,					///< Keypad {
		KK_KEYPAD_RIGHTBRACE = 185,					///< Keypad }
		KK_KEYPAD_TAB = 186,						///< Keypad Tab
		KK_KEYPAD_BACKSPACE = 187,					///< Keypad Backspace
		KK_KEYPAD_A = 188,							///< Keypad A
		KK_KEYPAD_B = 189,							///< Keypad B
		KK_KEYPAD_C = 190,							///< Keypad C
		KK_KEYPAD_D = 191,							///< Keypad D
		KK_KEYPAD_E = 192,							///< Keypad E
		KK_KEYPAD_F = 193,							///< Keypad F
		KK_KEYPAD_XOR = 194,						///< Keypad XOR
		KK_KEYPAD_POWER = 195,						///< Keypad ^
		KK_KEYPAD_PERCENT = 196,					///< Keypad %
		KK_KEYPAD_LESS = 197,						///< Keypad <
		KK_KEYPAD_GREATER = 198,					///< Keypad >
		KK_KEYPAD_AMPERSAND = 199,					///< Keypad &
		KK_KEYPAD_DOUBLE_AMPERSAND = 200,			///< Keypad &&
		KK_KEYPAD_VERTICALBAR = 201,				///< Keypad |
		KK_KEYPAD_DOUBLE_VERTICALBAR = 202,			///< Keypad ||
		KK_KEYPAD_COLON = 203,						///< Keypad :
		KK_KEYPAD_HASH = 204,						///< Keypad #
		KK_KEYPAD_SPACE = 205,						///< Keypad Space
		KK_KEYPAD_AT = 206,							///< Keypad @
		KK_KEYPAD_EXCLAM = 207,						///< Keypad !
		KK_KEYPAD_MEMSTORE = 208,					///< Keypad Memory Store
		KK_KEYPAD_MEMRECALL = 209,					///< Keypad Memory Recall
		KK_KEYPAD_MEMCLEAR = 210,					///< Keypad Memory Clear
		KK_KEYPAD_MEMADD = 211,						///< Keypad Memory Add
		KK_KEYPAD_MEMSUBTRACT = 212,				///< Keypad Memory Subtract
		KK_KEYPAD_MEMMULTIPLY = 213,				///< Keypad Memory Multiply
		KK_KEYPAD_MEMDIVIDE = 214,					///< Keypad Memory Divide
		KK_KEYPAD_PLUSMINUS = 215,					///< Keypad +/-
		KK_KEYPAD_CLEAR = 216,						///< Keypad Clear
		KK_KEYPAD_CLEARENTRY = 217,					///< Keypad Clear Entry
		KK_KEYPAD_BINARY = 218,						///< Keypad Binary
		KK_KEYPAD_OCTAL = 219,						///< Keypad Octal
		KK_KEYPAD_DECIMAL = 220,					///< Keypad Decimal
		KK_KEYPAD_HEXADECIMAL = 221,				///< Keypad Hexdecimal

		KK_LCTRL = 224,								///< Left Control
		KK_LSHIFT = 225,							///< Left Shift
		KK_LALT = 226,								///< Left Alt
		KK_LGUI = 227,								///< Left GUI (Window - Windows, Linux - Meta, etc)
		KK_RCTRL = 228,								///< Right Control
		KK_RSHIFT = 229,							///< Right Shift
		KK_RALT = 230,								///< Right Alt
		KK_RGUI = 231,								///< Right GUI (Window - Windows, Linux - Meta, etc)
		
		KK_MODE = 257,								///< Я не уверен, что он действительно не покрыт предыдущими 
													/// перечислениями, но так как есть специальный KMOD_MODE для него, то я добавлю его здесь :)

		//////////////////////////////////////////
		// Используемая страница 0x0С
		//
		// Эти значения взяты из страницы использования 0x0С (страница потребителя USB)

		KK_AUDIO_NEXT = 258,						///< Audio Next (Next Track media key)
		KK_AUDIO_PREV = 259,						///< Audio Prev (Previous Track media key)
		KK_AUDIO_STOP = 260,						///< Audio Stop (Stop media key)
		KK_AUDIO_PLAY = 261,						///< Audio Play (Play media key)
		KK_AUDIO_MUTE = 262,						///< Audio Mute (Mute volume key)
		KK_MEDIA_SELECT = 263,						///< Media Select (Media Select key)
		KK_WWW = 264,								///< WWW (WWW/World Wide Web key)
		KK_MAIL = 265,								///< Mail (Mail/eMail key)
		KK_CALCULATOR = 266,						///< Calculator (Calculator key)
		KK_COMPUTER = 267,							///< Computer (My Computer key)
		KK_AC_SEARCH = 268,							///< AC Search (Search key (клавиатура управления приложения))
		KK_AC_HOME = 269,							///< AC Home (Home key (клавиатура управления приложения))
		KK_AC_BACK = 270,							///< AC Back (Back key (клавиатура управления приложения))
		KK_AC_FORWARD = 271,						///< AC Forward (Forward key (клавиатура управления приложения))
		KK_AC_STOP = 272,							///< AC Stop (Stop key (клавиатура управления приложения))
		KK_AC_REFRESH = 273,						///< AC Refresh (Refresh key (клавиатура управления приложения))
		KK_AC_BOOKMARKS = 274,						///< AC Bookmarks (Bookmarks key (клавиатура управления приложения))

		//////////////////////////////////////////
		// Клавиши Вальтера
		//
		// Это значения, которые добавил Кристиан Вальтер (для клавиатуры mac?)

		KK_BRIGHTNESS_DOWN = 275,					///< Brightness Down (Brightness Down key)
		KK_BRIGHTNESS_UP = 276,						///< Brightness Up (Brightness Up key)
		KK_DISPLAY_SWITCH = 277,					///< Display Switch (отражение дисплея / двойной переключатель дисплея, видео-переключатель режима)
		KK_KBDILLUM_TOGGLE = 278,					///< KBDIllum Toggle (Клавиша переключения подсветки клавиатуры)
		KK_KBDILLUM_DOWN = 279,						///< KBDIllum Down (Клавиша подсветки клавиатуры вниз)
		KK_KBDILLUM_UP = 280,						///< KBDIllum Up (Клавиша подсветки клавиатуры вверх)
		KK_EJECT = 281,								///< Eject (Клавиша выбрасывания?)
		KK_SLEEP = 282,								///< Sleep (Клавиша сна)

		// Остальные клавиши добавлять суда

		KK_KEY_COUNT = 512							///< Количество кодов клавиш
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !KEYBOARD_KEY_H
