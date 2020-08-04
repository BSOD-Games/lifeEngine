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

#include "common/buttoncode.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAInputSystem
	{
	public:
		static void					Register( lua_State* LuaVM );

		static bool					IsKeyDown( UInt32_t Key );
		static bool					IsKeyUp( UInt32_t Key );
		static bool					IsMouseKeyDown( UInt32_t Key );
		static bool					IsMouseKeyUp( UInt32_t Key );
		static bool					IsMouseWheel( UInt32_t Wheel );
		static float				GetMouseSensitivity();
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAINPUTSYSTEM_H