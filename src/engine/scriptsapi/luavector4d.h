//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUAVECTOR4D_H
#define LUAVECTOR4D_H

#include <string>
#include "common/types.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAVector4D
	{
	public:
		static Vector4D_t			Normalize( const Vector4D_t& Vector );
		static float				Dot( const Vector4D_t& Left, const Vector4D_t& Right );

		static void					Normalize( Vector4D_t* Object );
		static float				Dot( Vector4D_t* Object, const Vector4D_t& Right );
		static void					Set( Vector4D_t* Object, float X, float Y, float Z, float W );
		static void					Set( Vector4DInt_t* Object, int X, int Y, int Z, int W );
		static std::string			ToString( Vector4D_t* Object );
		static std::string			ToString( Vector4DInt_t* Object );

		static void					Register( lua_State* LuaVM );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAVECTOR4D_H