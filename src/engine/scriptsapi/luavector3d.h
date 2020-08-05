//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUAVECTOR3D_H
#define LUAVECTOR3D_H

#include <string>
#include "common/types.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAVector3D
	{
	public:
		static Vector3D_t			Normalize( const Vector3D_t& Vector );
		static float				Dot( const Vector3D_t& Left, const Vector3D_t& Right );
		static Vector3D_t			Cross( const Vector3D_t& Left, const Vector3D_t& Right );

		static void					Normalize( Vector3D_t* Object );
		static float				Dot( Vector3D_t* Object, const Vector3D_t& Right );
		static Vector3D_t			Cross( Vector3D_t* Object, const Vector3D_t& Right );
		static void					Set( Vector3D_t* Object, float X, float Y, float Z );
		static void					Set( Vector3DInt_t* Object, int X, int Y, int Z );
		static std::string			ToString( Vector3D_t* Object );
		static std::string			ToString( Vector3DInt_t* Object );

		static void					Register( lua_State* LuaVM );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAVECTOR3D_H