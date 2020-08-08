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
		LUAVector3D( const Vector3D_t& Copy );
		LUAVector3D( float X = 0.f, float Y = 0.f, float Z = 0.f );

		static void					Register( lua_State* LuaVM );
		static LUAVector3D			Normalize( const LUAVector3D& Vector );
		static float				Dot( const LUAVector3D& Left, const LUAVector3D& Right );
		static LUAVector3D			Cross( const LUAVector3D& Left, const LUAVector3D& Right );
		void						Normalize();
		float						Dot( const LUAVector3D& Right );
		LUAVector3D					Cross( const LUAVector3D& Right );
		void						Set( float X, float Y, float Z );
		std::string					ToString();	

		void						SetX( float X );
		void						SetY( float Y );
		void						SetZ( float Z );

		float						GetX() const;
		float						GetY() const;
		float						GetZ() const;
		inline const Vector3D_t&	GetHandle() const			{ return object; }

		LUAVector3D					operator+( const LUAVector3D& Right );
		LUAVector3D					operator-( const LUAVector3D& Right );
		LUAVector3D					operator/( const LUAVector3D& Right );
		LUAVector3D					operator*( const LUAVector3D& Right );

	private:
		Vector3D_t					object;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAVECTOR3D_H