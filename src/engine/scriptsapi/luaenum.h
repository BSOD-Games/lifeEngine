//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUAENUM_H
#define LUAENUM_H

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <string>
#include <LuaBridge/LuaBridge.h>

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LuaEnum
	{
	public:
		LuaEnum( lua_State* LuaVM ) :
			luaVM( LuaVM ),
			luaEnum( luabridge::newTable( LuaVM ) )
		{}

		template< typename T >
		LuaEnum&				AddValue( const std::string& Name, T Value )
		{
			luaEnum[ Name ] = ( int ) Value;
			return *this;
		}

		luabridge::LuaRef		GetHandle()
		{
			return luaEnum;
		}

	private:
		luabridge::LuaRef			luaEnum;
		lua_State*					luaVM;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAENUM_H
