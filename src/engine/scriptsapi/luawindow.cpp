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
#include "common/image.h"
#include "engine/lifeengine.h"
#include "engine/engine.h"
#include "engine/window.h"
#include "scriptsapi/luawindow.h"

le::Window*			g_window = nullptr;

// ------------------------------------------------------------------------------------ //
// Register window
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;
	
	LIFEENGINE_ASSERT( g_engine );
	g_window = g_engine->GetWindow();

	// Register window
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAWindow>( "Window" ).
		addStaticFunction( "ResetCursor", &LUAWindow::ResetCursor ).
		addStaticFunction( "SetTitle", &LUAWindow::SetTitle ).
		addStaticFunction( "SetSize", &LUAWindow::SetSize ).
		addStaticFunction( "SetShowCursor", &LUAWindow::SetShowCursor ).
		addStaticFunction( "SetFullscreen", &LUAWindow::SetFullscreen ).
		addStaticFunction( "SetIcon", &LUAWindow::SetIcon ).
		addStaticFunction( "SetCursor", &LUAWindow::SetCursor ).
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
	if ( !g_window ) return;
	g_window->ResetCursor();
}

// ------------------------------------------------------------------------------------ //
// Set window title
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::SetTitle( const char* Title )
{
	if ( !g_window ) return;
	g_window->SetTitle( Title ? Title : "" );
}

// ------------------------------------------------------------------------------------ //
// Set icon
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::SetIcon( const Image& Image )
{
	if ( !g_window ) return;
	g_window->SetIcon( Image );
}

// ------------------------------------------------------------------------------------ //
// Set cyrsor
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::SetCursor( const Image& Image )
{
	if ( !g_window ) return;
	g_window->SetCursor( Image );
}

// ------------------------------------------------------------------------------------ //
// Set window size
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::SetSize( int Width, int Height )
{
	if ( !g_window ) return;
	g_window->SetSize( Width, Height );
}

// ------------------------------------------------------------------------------------ //
// Set show cursor
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::SetShowCursor( bool IsShow )
{
	if ( !g_window ) return;
	g_window->SetShowCursor( IsShow );
}

// ------------------------------------------------------------------------------------ //
// Set fullscreen
// ------------------------------------------------------------------------------------ //
void le::LUAWindow::SetFullscreen( bool IsFullscreen )
{
	if ( !g_window ) return;
	g_window->SetFullscreen( IsFullscreen );
}

// ------------------------------------------------------------------------------------ //
// Is open
// ------------------------------------------------------------------------------------ //
bool le::LUAWindow::IsOpen()
{
	if ( !g_window ) return false;
	return g_window->IsOpen();
}

// ------------------------------------------------------------------------------------ //
// Is showing cursor
// ------------------------------------------------------------------------------------ //
bool le::LUAWindow::IsShowingCursor()
{
	if ( !g_window ) return false;
	return g_window->IsShowingCursor();
}

// ------------------------------------------------------------------------------------ //
// Is fullscreen
// ------------------------------------------------------------------------------------ //
bool le::LUAWindow::IsFullscreen()
{
	if ( !g_window ) return false;
	return g_window->IsFullscreen();
}

// ------------------------------------------------------------------------------------ //
// Get size
// ------------------------------------------------------------------------------------ //
le::Vector2D_t le::LUAWindow::GetSize()
{
	if ( !g_window )	return Vector2D_t( 0.f, 0.f );

	UInt32_t		width = 0;
	UInt32_t		height = 0;
	
	g_window->GetSize( width, height );
	return Vector2D_t( width, height );
}
