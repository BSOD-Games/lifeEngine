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
#include "engine/window.h"
#include "scriptsapi/mathlib/luavector2d.h"
#include "scriptsapi/common/luaimage.h"
#include "scriptsapi/engine/luawindow.h"

// ------------------------------------------------------------------------------------ //
// Register window in LUA
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Registern input system
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAWindow>( "Window" ).
		addStaticFunction( "ResetCursor", &LUAWindow::ResetCursor ).
		addStaticFunction( "SetTitle", &LUAWindow::SetTitle ).
		addStaticFunction( "SetIcon", &LUAWindow::SetIcon ).
		addStaticFunction( "SetCursor", &LUAWindow::SetCursor ).
		addStaticFunction( "SetSize", &LUAWindow::SetSize ).
		addStaticFunction( "SetShowCursor", &LUAWindow::SetShowCursor ).
		addStaticFunction( "SetFullscreen", &LUAWindow::SetFullscreen ).
		addStaticFunction( "IsOpen", &LUAWindow::IsOpen ).
		addStaticFunction( "IsShowingCursor", &LUAWindow::IsShowingCursor ).
		addStaticFunction( "IsFullscreen", &LUAWindow::IsFullscreen ).
		addStaticFunction( "GetSize", &LUAWindow::GetSize ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Reset cursor
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::ResetCursor()
{
	g_window->ResetCursor();
}

// ------------------------------------------------------------------------------------ //
// Set title
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::SetTitle( const char* Title )
{
	if ( !Title ) return;
	g_window->SetTitle( Title );
}

// ------------------------------------------------------------------------------------ //
// Set icon
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::SetIcon( const LUAImage& Image )
{
	g_window->SetIcon( Image.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set cursor
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::SetCursor( const LUAImage& Image )
{
	g_window->SetCursor( Image.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set size
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::SetSize( int Width, int Height )
{
	g_window->SetSize( Width, Height );
}

// ------------------------------------------------------------------------------------ //
// Set show cursor
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::SetShowCursor( bool IsShow )
{
	g_window->SetShowCursor( IsShow );
}

// ------------------------------------------------------------------------------------ //
// Set fullscreen
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::SetFullscreen( bool IsFullscreen )
{
	g_window->SetFullscreen( IsFullscreen );
}

// ------------------------------------------------------------------------------------ //
// Is open
// ------------------------------------------------------------------------------------ //
bool le::LUAWindow::IsOpen()
{
	return g_window->IsOpen();
}

// ------------------------------------------------------------------------------------ //
// Is showing cursor
// ------------------------------------------------------------------------------------ //
bool le::LUAWindow::IsShowingCursor()
{
	return g_window->IsShowingCursor();
}

// ------------------------------------------------------------------------------------ //
// Is fullscreen
// ------------------------------------------------------------------------------------ //
bool le::LUAWindow::IsFullscreen()
{
	return g_window->IsFullscreen();
}

// ------------------------------------------------------------------------------------ //
// Get size
// ------------------------------------------------------------------------------------ //
le::LUAVector2D le::LUAWindow::GetSize()
{
	UInt32_t			width = 0;
	UInt32_t			height = 0;
	
	g_window->GetSize( width, height );
	return LUAVector2D( width, height );
}
