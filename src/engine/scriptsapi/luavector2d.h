//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUAVECTOR2D_H
#define LUAVECTOR2D_H

#include <string>
#include "common/types.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAVector2D
	{
	public:
		static Vector2D_t			Normalize( const Vector2D_t& Vector );
		static float				Dot( const Vector2D_t& Left, const Vector2D_t& Right );

		static void					Normalize( Vector2D_t* Object );
		static float				Dot( Vector2D_t* Object, const Vector2D_t& Right );	
		static void					Set( Vector2D_t* Object, float X, float Y );
		static void					Set( Vector2DInt_t* Object, int X, int Y );
		static std::string			ToString( Vector2D_t* Object );
		static std::string			ToString( Vector2DInt_t* Object );

		static void					Register( lua_State* LuaVM );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAVECTOR2D_H