//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUAIMAGE_H
#define LUAIMAGE_H

#include "common/types.h"

//---------------------------------------------------------------------//

struct lua_State;
struct Image;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAImage
	{
	public:
		static void				Register( lua_State* LuaVM );
		static bool				IsEmpty( Image* Object );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAIMAGE_H
