//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUAQUATERNION_H
#define LUAQUATERNION_H

#include <string>
#include "common/types.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAVector3D;

	//---------------------------------------------------------------------//

	class LUAQuaternion
	{
	public:
		LUAQuaternion( const Quaternion_t& Copy );
		LUAQuaternion( float X = 0.f, float Y = 0.f, float Z = 0.f, float W = 0.f );

		static void					Register( lua_State* LuaVM );
		static LUAQuaternion		Normalize( const LUAQuaternion& Quternion );
		static float				Dot( const LUAQuaternion& Left, const LUAQuaternion& Right );
		static LUAQuaternion		Cross( const LUAQuaternion& Left, const LUAQuaternion& Right );
		static LUAQuaternion		Cross( const LUAQuaternion& Left, const LUAVector3D& Right );
		static LUAQuaternion		Cross( const LUAVector3D& Left, const LUAQuaternion& Right );
		void						Normalize();
		float						Dot( const LUAQuaternion& Right );
		void						Set( float X, float Y, float Z, float W );
		LUAQuaternion				Cross( const LUAQuaternion& Right );
		LUAQuaternion				Cross( const LUAVector3D& Right );
		std::string					ToString();	

		void						SetX( float X );
		void						SetY( float Y );
		void						SetZ( float Z );
		void						SetW( float W );

		float						GetX() const;
		float						GetY() const;
		float						GetZ() const;
		float						GetW() const;
		inline const Quaternion_t&	GetHandle() const		{ return object; }

		LUAQuaternion					operator+( const LUAQuaternion& Right );
		LUAQuaternion					operator-( const LUAQuaternion& Right );
		LUAQuaternion					operator*( const LUAQuaternion& Right );
	private:
		Quaternion_t					object;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAQUATERNION_H