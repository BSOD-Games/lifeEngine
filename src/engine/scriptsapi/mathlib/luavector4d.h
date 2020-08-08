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
		LUAVector4D( const Vector4D_t& Copy );
		LUAVector4D( float X = 0.f, float Y = 0.f, float Z = 0.f, float W = 0.f );

		static void					Register( lua_State* LuaVM );
		static LUAVector4D			Normalize( const LUAVector4D& Vector );
		static float				Dot( const LUAVector4D& Left, const LUAVector4D& Right );
		void						Normalize();
		float						Dot( const LUAVector4D& Right );
		void						Set( float X, float Y, float Z, float W );
		std::string					ToString();	

		void						SetX( float X );
		void						SetY( float Y );
		void						SetZ( float Z );
		void						SetW( float W );

		float						GetX() const;
		float						GetY() const;
		float						GetZ() const;
		float						GetW() const;
		inline const Vector4D_t&	GetHandle() const		{ return object; }

		LUAVector4D					operator+( const LUAVector4D& Right );
		LUAVector4D					operator-( const LUAVector4D& Right );
		LUAVector4D					operator/( const LUAVector4D& Right );
		LUAVector4D					operator*( const LUAVector4D& Right );

	private:
		Vector4D_t					object;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAVECTOR4D_H