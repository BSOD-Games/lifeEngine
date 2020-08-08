//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUACONCMD_H
#define LUACONCMD_H

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <string>
#include <LuaBridge/LuaBridge.h>

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAConCmd
	{
	public:
		LUAConCmd( const char* Name, const char* HelpText, luabridge::LuaRef ExecCallback );
		~LUAConCmd();

		static void				Register( lua_State* LuaVM );
		void					Exec( luabridge::LuaRef Arguments );

		void					SetExecCallback( luabridge::LuaRef ExecCallback );
		void					SetName( const char* Name );

		const char*				GetName() const;
		const char*				GetHelpText() const;

	private:
		luabridge::LuaRef		execCallback;
		std::string				name;
		std::string				helpText;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUACONCMD_H
