//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#include "global.h"
#include "engine/consolesystem.h"
#include "engine/inputsystem.h"
#include "scriptsapi/luaenum.h"
#include "scriptsapi/mathlib/luavector2d.h"
#include "scriptsapi/engine/luainputsystem.h"

// ------------------------------------------------------------------------------------ //
// Register input system system in LUA
// ------------------------------------------------------------------------------------ //
void le::LUAInputSystem::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;
	
	// Register ButtonCode enums
	LuaEnum			luaButtonCode( LuaVM );
	luaButtonCode.AddValue( "None", BC_NONE ).
		AddValue( "Key0", BC_KEY_0 ).
		AddValue( "Key1", BC_KEY_1 ).
		AddValue( "Key2", BC_KEY_2 ).
		AddValue( "Key3", BC_KEY_3 ).
		AddValue( "Key4", BC_KEY_4 ).
		AddValue( "Key5", BC_KEY_5 ).
		AddValue( "Key6", BC_KEY_6 ).
		AddValue( "Key7", BC_KEY_7 ).
		AddValue( "Key8", BC_KEY_8 ).
		AddValue( "Key9", BC_KEY_9 ).
		AddValue( "KeyA", BC_KEY_A ).
		AddValue( "KeyB", BC_KEY_B ).
		AddValue( "KeyC", BC_KEY_C ).
		AddValue( "KeyD", BC_KEY_D ).
		AddValue( "KeyR", BC_KEY_E ).
		AddValue( "KeyF", BC_KEY_F ).
		AddValue( "KeyG", BC_KEY_G ).
		AddValue( "KeyH", BC_KEY_H ).
		AddValue( "KeyI", BC_KEY_I ).
		AddValue( "KeyJ", BC_KEY_J ).
		AddValue( "KeyK", BC_KEY_K ).
		AddValue( "KeyL", BC_KEY_L ).
		AddValue( "KeyM", BC_KEY_M ).
		AddValue( "KeyN", BC_KEY_N ).
		AddValue( "KeyO", BC_KEY_O ).
		AddValue( "KeyP", BC_KEY_P ).
		AddValue( "KeyQ", BC_KEY_Q ).
		AddValue( "KeyR", BC_KEY_R ).
		AddValue( "KeyS", BC_KEY_S ).
		AddValue( "KeyT", BC_KEY_T ).
		AddValue( "KeyU", BC_KEY_U ).
		AddValue( "KeyV", BC_KEY_V ).
		AddValue( "KeyW", BC_KEY_W ).
		AddValue( "KeyX", BC_KEY_X ).
		AddValue( "KeyY", BC_KEY_Y ).
		AddValue( "KeyZ", BC_KEY_Z ).
		AddValue( "KeyPad0", BC_KEY_PAD_0 ).
		AddValue( "KeyPad1", BC_KEY_PAD_1 ).
		AddValue( "KeyPad2", BC_KEY_PAD_2 ).
		AddValue( "KeyPad3", BC_KEY_PAD_3 ).
		AddValue( "KeyPad4", BC_KEY_PAD_4 ).
		AddValue( "KeyPad5", BC_KEY_PAD_5 ).
		AddValue( "KeyPad6", BC_KEY_PAD_6 ).
		AddValue( "KeyPad7", BC_KEY_PAD_7 ).
		AddValue( "KeyPad8", BC_KEY_PAD_8 ).
		AddValue( "KeyPad9", BC_KEY_PAD_9 ).
		AddValue( "KeyPadDivide", BC_KEY_PAD_DIVIDE ).
		AddValue( "KeyPadMultiply", BC_KEY_PAD_MULTIPLY ).
		AddValue( "KeyPadMinus", BC_KEY_PAD_MINUS ).
		AddValue( "KeyPadPlus", BC_KEY_PAD_PLUS ).
		AddValue( "KeyPadEnter", BC_KEY_PAD_ENTER ).
		AddValue( "KeyPadDecimal", BC_KEY_PAD_DECIMAL ).
		AddValue( "KeyLBracket", BC_KEY_LBRACKET ).
		AddValue( "KeyRBracket", BC_KEY_RBRACKET ).
		AddValue( "KeySemicolon", BC_KEY_SEMICOLON ).
		AddValue( "KeyApostrophe", BC_KEY_APOSTROPHE ).
		AddValue( "KeyBackquote", BC_KEY_BACKQUOTE ).
		AddValue( "KeyComma", BC_KEY_COMMA ).
		AddValue( "KeyPeriod", BC_KEY_PERIOD ).
		AddValue( "KeySlash", BC_KEY_SLASH ).
		AddValue( "KeyBackSlash", BC_KEY_BACKSLASH ).
		AddValue( "KeyMinus", BC_KEY_MINUS ).
		AddValue( "KeyEqual", BC_KEY_EQUAL ).
		AddValue( "KeyEnter", BC_KEY_ENTER ).
		AddValue( "KeySpace", BC_KEY_SPACE ).
		AddValue( "KeyBacksapce", BC_KEY_BACKSPACE ).
		AddValue( "KeyTab", BC_KEY_TAB ).
		AddValue( "KeyCapsLock", BC_KEY_CAPSLOCK ).
		AddValue( "KeyNumLock", BC_KEY_NUMLOCK ).
		AddValue( "KeyEscape", BC_KEY_ESCAPE ).
		AddValue( "KeyScrollLock", BC_KEY_SCROLLLOCK ).
		AddValue( "KeyInsert", BC_KEY_INSERT ).
		AddValue( "KeyDelete", BC_KEY_DELETE ).
		AddValue( "KeyHome", BC_KEY_HOME ).
		AddValue( "KeyEnd", BC_KEY_END ).
		AddValue( "KeyPageUp", BC_KEY_PAGEUP ).
		AddValue( "KeyPageDown", BC_KEY_PAGEDOWN ).
		AddValue( "KeyBreak", BC_KEY_BREAK ).
		AddValue( "KeyLShift", BC_KEY_LSHIFT ).
		AddValue( "KeyRShift", BC_KEY_RSHIFT ).
		AddValue( "KeyLAlt", BC_KEY_LALT ).
		AddValue( "KeyRAlt", BC_KEY_RALT ).
		AddValue( "KeyLControl", BC_KEY_LCONTROL ).
		AddValue( "KeyRControl", BC_KEY_RCONTROL ).
		AddValue( "KeyLWin", BC_KEY_LWIN ).
		AddValue( "KeyRWin", BC_KEY_RWIN ).
		AddValue( "KeyApp", BC_KEY_APP ).
		AddValue( "KeyUp", BC_KEY_UP ).
		AddValue( "KeyLeft", BC_KEY_LEFT ).
		AddValue( "KeyDown", BC_KEY_DOWN ).
		AddValue( "KeyRight", BC_KEY_RIGHT ).
		AddValue( "KeyF1", BC_KEY_F1 ).
		AddValue( "KeyF2", BC_KEY_F2 ).
		AddValue( "KeyF3", BC_KEY_F3 ).
		AddValue( "KeyF4", BC_KEY_F4 ).
		AddValue( "KeyF5", BC_KEY_F5 ).
		AddValue( "KeyF6", BC_KEY_F6 ).
		AddValue( "KeyF7", BC_KEY_F7 ).
		AddValue( "KeyF8", BC_KEY_F8 ).
		AddValue( "KeyF9", BC_KEY_F9 ).
		AddValue( "KeyF10", BC_KEY_F10 ).
		AddValue( "KeyF11", BC_KEY_F11 ).
		AddValue( "KeyF12", BC_KEY_F12 ).
		AddValue( "Mouse1", BC_MOUSE_1 ).
		AddValue( "Mouse2", BC_MOUSE_2 ).
		AddValue( "Mouse3", BC_MOUSE_3 ).
		AddValue( "Mouse4", BC_MOUSE_4 ).
		AddValue( "Mouse5", BC_MOUSE_5 ).
		AddValue( "Mouse6", BC_MOUSE_6 ).
		AddValue( "Mouse7", BC_MOUSE_7 ).
		AddValue( "Mouse8", BC_MOUSE_8 ).
		AddValue( "MouseWheelUp", BC_MOUSE_WHEEL_UP ).
		AddValue( "MouseWheelDown", BC_MOUSE_WHEEL_DOWN ).
		AddValue( "MouseLeft", BC_MOUSE_LEFT ).
		AddValue( "MouseRight", BC_MOUSE_RIGHT ).
		AddValue( "MouseMiddle", BC_MOUSE_MIDDLE );

	luabridge::setGlobal( LuaVM, luaButtonCode.GetHandle(), "ButtonCode" );

	// Registern input system
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAInputSystem>( "InputSystem" ).
		addStaticFunction( "IsKeyDown", &LUAInputSystem::IsKeyDown ).
		addStaticFunction( "IsKeyUp", &LUAInputSystem::IsKeyUp ).
		addStaticFunction( "IsMouseKeyDown", &LUAInputSystem::IsMouseKeyDown ).
		addStaticFunction( "IsMouseKeyUp", &LUAInputSystem::IsMouseKeyUp ).
		addStaticFunction( "IsMouseWheel", &LUAInputSystem::IsMouseWheel ).
		addStaticFunction( "GetMousePosition", &LUAInputSystem::GetMousePosition ).
		addStaticFunction( "GetMouseOffset", &LUAInputSystem::GetMouseOffset ).
		addStaticFunction( "GetMouseSensitivity", &LUAInputSystem::GetMouseSensitivity ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Is key down
// ------------------------------------------------------------------------------------ //
bool le::LUAInputSystem::IsKeyDown( UInt32_t Key )
{
	return g_inputSystem->IsKeyDown( ( BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is key up
// ------------------------------------------------------------------------------------ //
bool le::LUAInputSystem::IsKeyUp( UInt32_t Key )
{
	return g_inputSystem->IsKeyUp( ( BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is mouse key down
// ------------------------------------------------------------------------------------ //
bool le::LUAInputSystem::IsMouseKeyDown( UInt32_t Key )
{
	return g_inputSystem->IsMouseKeyDown( ( BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is mouse key up
// ------------------------------------------------------------------------------------ //
bool le::LUAInputSystem::IsMouseKeyUp( UInt32_t Key )
{
	return g_inputSystem->IsMouseKeyUp( ( BUTTON_CODE ) Key );
}

// ------------------------------------------------------------------------------------ //
// Is mouse wheel
// ------------------------------------------------------------------------------------ //
bool le::LUAInputSystem::IsMouseWheel( UInt32_t Wheel )
{
	return g_inputSystem->IsMouseWheel( ( BUTTON_CODE ) Wheel );
}

// ------------------------------------------------------------------------------------ //
// Get mouse position
// ------------------------------------------------------------------------------------ //
le::LUAVector2D le::LUAInputSystem::GetMousePosition()
{
	return LUAVector2D( g_inputSystem->GetMousePosition() );
}

// ------------------------------------------------------------------------------------ //
// Get mouse offset
// ------------------------------------------------------------------------------------ //
le::LUAVector2D le::LUAInputSystem::GetMouseOffset()
{
	return LUAVector2D( g_inputSystem->GetMouseOffset() );
}

// ------------------------------------------------------------------------------------ //
// Get mouse sensitivity
// ------------------------------------------------------------------------------------ //
float le::LUAInputSystem::GetMouseSensitivity()
{
	return g_inputSystem->GetMouseSensitivity();
}
