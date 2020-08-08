//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUACONSOLESYSTEM_H
#define LUACONSOLESYSTEM_H

#include <string>

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAConsoleSystem
	{
	public:
		//static void				RegisterCommand( IConCmd* ConCmd );
		//static void				UnregisterCommand( const std::string& Name );
		static void				Exec( const std::string& Command );
		static void				PrintInfo( const std::string& Message );
		static void				PrintWarning( const std::string& Message );
		static void				PrintError( const std::string& Message );
		static void				Register( lua_State* LuaVM );

		//static IConCmd*			GetCommand( const char* Name );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUACONSOLESYSTEM_H
