//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUABUTTONCODE_H
#define LUABUTTONCODE_H

#include "common/types.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAButtonCode
	{
	public:
		static void					Register( lua_State* LuaVM );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUABUTTONCODE_H