//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUACAMERA_H
#define LUACAMERA_H

#include "common/types.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ICamera;
	class LUAVector2D;
	class LUAVector3D;
	struct LUARay;

	//---------------------------------------------------------------------//

	class LUACamera
	{
	public:
		static void				Register( lua_State* LuaVM );
		
		LUACamera();
		LUACamera( const LUACamera& Copy );
		~LUACamera();
		void					InitProjection_Perspective( float FOV, float Aspect, float Near, float Far );
		void					InitProjection_Ortho( float Left, float Right, float Bottom, float Top, float Near = 0.f, float Far = 1.f );
		void					Move( const LUAVector3D& FactorMove );
		void					Move( UInt32_t SideMove, float MoveSpeed );
		void					Rotate( const LUAVector3D& FactorRotate );
		void					RotateByMouse( const LUAVector2D& MouseOffset, float MouseSensitivity, bool ConstrainYaw = true );
		LUARay					ScreenToWorld( const LUAVector2D& Coords, const LUAVector2D& ViewportSize );
		LUAVector3D				WorldToScreen( const LUAVector3D& Coords, const LUAVector2D& ViewportSize );

		void					SetPosition( const LUAVector3D& Position );
		void					SetRotation( const LUAVector3D& Rotation );
		void					SetTargetDirection( const LUAVector3D& TargetDirection );
		void					SetUp( const LUAVector3D& Up );

		bool					IsVisible( const LUAVector3D& MinPosition, const LUAVector3D& MaxPosition );
		bool					IsVisible( const LUAVector3D& Position, float Radius );

		float					GetNear() const;
		float					GetFar() const;
		LUAVector3D				GetPosition() const;
		LUAVector3D				GetUp() const;
		LUAVector3D				GetRight() const;
		LUAVector3D				GetDirectionMove( UInt32_t SideMove ) const;
		//const Quaternion_t		GetQuatRotation() const;
		LUAVector3D				GetEulerRotation() const;
		LUAVector3D				GetTargetDirection() const;
		inline ICamera*			GetHandle() const				{ return camera; }

		//const Matrix4x4_t		GetViewMatrix();
		//const Matrix4x4_t		GetProjectionMatrix() const;

	private:
		ICamera*				camera;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUACAMERA_H
