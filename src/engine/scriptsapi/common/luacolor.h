//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUACOLOR_H
#define LUACOLOR_H

#include <string>
#include "common/types.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAColor
	{
	public:
		LUAColor( const Vector4D_t& Copy );
		LUAColor( float R = 0.f, float G = 0.f, float B = 0.f, float A = 0.f );

		static void					Register( lua_State* LuaVM );
		void						Set( float R, float G, float B, float A );
		std::string					ToString();	

		void						SetR( float R );
		void						SetG( float G );
		void						SetB( float B );
		void						SetA( float A );

		float						GetR() const;
		float						GetG() const;
		float						GetB() const;
		float						GetA() const;
		inline const Color_t&		GetHandle() const		{ return object; }

		LUAColor					operator+( const LUAColor& Right );
		LUAColor					operator-( const LUAColor& Right );
		LUAColor					operator/( const LUAColor& Right );
		LUAColor					operator*( const LUAColor& Right );

	private:
		Color_t					object;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUACOLOR_H