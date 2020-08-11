//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUAINPUTSYSTEM_H
#define LUAINPUTSYSTEM_H

#include <string>
#include "common/buttoncode.h"

//---------------------------------------------------------------------//

struct lua_State;
namespace luabridge
{
	class LuaRef;
}

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAVector2D;

	//---------------------------------------------------------------------//

	class LUAInputSystem
	{
	public:
		static void				Register( lua_State* LuaVM );
		static bool				IsKeyDown( const luabridge::LuaRef& Key );
		static bool				IsKeyUp( UInt32_t Key );
		static bool				IsMouseKeyDown( UInt32_t Key );
		static bool				IsMouseKeyUp( UInt32_t Key );
		static bool				IsMouseWheel( UInt32_t Wheel );

		static LUAVector2D		GetMousePosition();
		static LUAVector2D		GetMouseOffset();
		static float			GetMouseSensitivity();
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAINPUTSYSTEM_H
