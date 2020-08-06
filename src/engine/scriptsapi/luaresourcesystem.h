//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUARESOURCESYSTEM_H
#define LUARESOURCESYSTEM_H

#include "common/types.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct Image;

	//---------------------------------------------------------------------//

	class LUAResourceSystem
	{
	public:
		static void				Register( lua_State* LuaVM );
		static Image			LoadImage( const char* Path, bool IsFlipVertical = false, bool IsSwitchRedAndBlueChannels = false );
		static void				UnloadImage( Image& Image );
		static void				UnloadAll();
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUARESOURCESYSTEM_H
