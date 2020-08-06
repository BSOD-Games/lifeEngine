//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUAWINDOW_H
#define LUAWINDOW_H

#include "common/types.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct Image;

	//---------------------------------------------------------------------//

	class LUAWindow
	{
	public:
		static void				Register( lua_State* LuaVM );
		static void				ResetCursor();

		static void				SetTitle( const char* Title );
		static void				SetIcon( const Image& Image );
		static void				SetCursor( const Image& Image );
		static void				SetSize( int Width, int Height );
		static void				SetShowCursor( bool IsShow = true );
		static void				SetFullscreen( bool IsFullscreen );

		static bool				IsOpen();
		static bool				IsShowingCursor();
		static bool				IsFullscreen();
		static Vector2D_t		GetSize();
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAWINDOW_H
