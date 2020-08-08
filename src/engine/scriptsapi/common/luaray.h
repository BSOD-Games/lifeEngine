//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUARAY_H
#define LUARAY_H

#include "scriptsapi/mathlib/luavector3d.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct Ray;

	//---------------------------------------------------------------------//

	struct LUARay
	{
	public:
		static void				Register( lua_State* LuaVM );

		LUARay( Ray& Ray );
		LUARay( const LUAVector3D& Origin = LUAVector3D(), const LUAVector3D& Direction = LUAVector3D() );
		void				Set( const LUAVector3D& Origin, const LUAVector3D& Direction );

		LUAVector3D			origin;
		LUAVector3D			direction;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUARAY_H
