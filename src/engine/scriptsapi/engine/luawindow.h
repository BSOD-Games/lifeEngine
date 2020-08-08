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

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAVector2D;
	class LUAImage;

	//---------------------------------------------------------------------//

	class LUAWindow
	{
	public:
		static void				Register( lua_State* LuaVM );
		static void				ResetCursor();

		static void				SetTitle( const char* Title );
		static void				SetIcon( const LUAImage& Image );
		static void				SetCursor( const LUAImage& Image );
		static void				SetSize( int Width, int Height );
		static void				SetShowCursor( bool IsShow = true );
		static void				SetFullscreen( bool IsFullscreen );

		static bool				IsOpen();
		static bool				IsShowingCursor();
		static bool				IsFullscreen();
		static LUAVector2D		GetSize();
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUACONCMD_H
