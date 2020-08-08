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
		LUAVector2D( Vector2D_t& Copy );
		LUAVector2D( float X = 0.f, float Y = 0.f );

		static void					Register( lua_State* LuaVM );
		static LUAVector2D			Normalize( const LUAVector2D& Vector );
		static float				Dot( const LUAVector2D& Left, const LUAVector2D& Right );
		void						Normalize();
		float						Dot(const LUAVector2D& Right );
		void						Set( float X, float Y );	
		std::string					ToString();	

		void						SetX( float X );
		void						SetY( float Y );

		float						GetX() const;
		float						GetY() const;

		LUAVector2D					operator+( const LUAVector2D& Right );
		LUAVector2D					operator-( const LUAVector2D& Right );
		LUAVector2D					operator/( const LUAVector2D& Right );
		LUAVector2D					operator*( const LUAVector2D& Right );

	private:
		Vector2D_t					object;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAVECTOR2D_H