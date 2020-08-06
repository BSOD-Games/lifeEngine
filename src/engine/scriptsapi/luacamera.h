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
#include "common/ray.h"
#include "engine/icamera.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUACamera
	{
	public:
		static void							Register( lua_State* LuaVM );
		/*static void							InitProjection_Perspective( ICamera* Object, float FOV, float Aspect, float Near, float Far );
		static void							InitProjection_Ortho( ICamera* Object, float Left, float Right, float Bottom, float Top, float Near = 0.f, float Far = 1.f );
		static void							Move( ICamera* Object, const Vector3D_t& FactorMove );
		static void							Move( ICamera* Object, CAMERA_SIDE_MOVE SideMove, float MoveSpeed );
		static void							Rotate( ICamera* Object, const Vector3D_t& FactorRotate );
		static void							RotateByMouse( ICamera* Object, const Vector2D_t& MouseOffset, float MouseSensitivit, bool ConstrainYaw = true );
		static Ray							ScreenToWorld( ICamera* Object, const Vector2D_t& Coords, const Vector2D_t& ViewportSize );
		static Vector3D_t					WorldToScreen( ICamera* Object, const Vector3D_t& Coords, const Vector2D_t& ViewportSize );

		static void							SetPosition( ICamera* Object, const Vector3D_t& Position );
		static void							SetRotation( ICamera* Object, const Vector3D_t& Rotation );
		static void							SetTargetDirection( ICamera* Object, const Vector3D_t& TargetDirection );
		static void							SetUp( ICamera* Object, const Vector3D_t& Up );

		static bool							IsVisible( ICamera* Object, const Vector3D_t& MinPosition, const Vector3D_t& MaxPosition );
		static bool							IsVisible( ICamera* Object, const Vector3DInt_t& MinPosition, const Vector3DInt_t& MaxPosition );
		static bool							IsVisible( ICamera* Object, const Vector3DInt_t& Position, float Radius );

		static float						GetNear( ICamera* Object );
		static float						GetFar( ICamera* Object );
		static const Vector3D_t&			GetPosition( ICamera* Object );
		static const Vector3D_t&			GetUp( ICamera* Object );
		static const Vector3D_t&			GetRight( ICamera* Object );
		static Vector3D_t					GetDirectionMove( ICamera* Object, CAMERA_SIDE_MOVE SideMove );
		//static const Quaternion_t&			GetQuatRotation( ICamera* Object );
		static const Vector3D_t&			GetEulerRotation( ICamera* Object );
		static const Vector3D_t&			GetTargetDirection( ICamera* Object );
		//static const Matrix4x4_t&			GetViewMatrix( ICamera* Object );
		//static const Matrix4x4_t&			GetProjectionMatrix( ICamera* Object );*/
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUACAMERA_H
