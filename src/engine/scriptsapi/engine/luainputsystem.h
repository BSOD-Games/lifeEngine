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

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAInputSystem
	{
	public:
		static void				Register( lua_State* LuaVM );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAINPUTSYSTEM_H
