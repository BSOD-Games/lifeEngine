//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ICAMERA_H
#define ICAMERA_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct Ray;

	//---------------------------------------------------------------------//

	enum CAMERA_SIDE_MOVE
	{
		CSM_NONE			= 0,
		CSM_FORWARD			= 1 << 0,
		CSM_BACKWARD		= 1 << 1,
		CSM_LEFT			= 1 << 2,   
		CSM_RIGHT			= 1 << 3   
	};

	//---------------------------------------------------------------------//

	class ICamera
	{
	public:
		virtual ~ICamera() {}

		virtual void							InitProjection_Perspective( float FOV, float Aspect, float Near, float Far ) = 0;
		virtual void							InitProjection_Ortho( float Left, float Right, float Bottom, float Top, float Near = 0.f, float Far = 1.f ) = 0;
		virtual void							Move( const Vector3D_t& FactorMove ) = 0;
		virtual void							Move( CAMERA_SIDE_MOVE SideMove, float MoveSpeed ) = 0;
		virtual void							Rotate( const Vector3D_t& FactorRotate ) = 0;
		virtual void							RotateByMouse( const Vector2D_t& MouseOffset, float MouseSensitivit, bool ConstrainYaw = true ) = 0;
		virtual Ray								ScreenToWorld( const Vector2D_t& Coords, const Vector2D_t& ViewportSize ) = 0;
		virtual Vector3D_t						WorldToScreen( const Vector3D_t& Coords, const Vector2D_t& ViewportSize ) = 0;

		virtual void							SetPosition( const Vector3D_t& Position ) = 0;
		virtual void							SetRotation( const Vector3D_t& Rotation ) = 0;
		virtual void							SetTargetDirection( const Vector3D_t& TargetDirection ) = 0;
		virtual void							SetUp( const Vector3D_t& Up ) = 0;

		virtual bool							IsVisible( const Vector3D_t& MinPosition, const Vector3D_t& MaxPosition ) = 0;
		virtual bool							IsVisible( const Vector3DInt_t& MinPosition, const Vector3DInt_t& MaxPosition ) = 0;
		virtual bool							IsVisible( const Vector3DInt_t& Position, float Radius ) = 0;

		virtual float							GetNear() const = 0;
		virtual float							GetFar() const = 0;
		virtual const Vector3D_t&				GetPosition() const = 0;
		virtual const Vector3D_t&				GetUp() const = 0;
		virtual const Vector3D_t&				GetRight() const = 0;
		virtual const Quaternion_t&				GetQuatRotation() const = 0;
		virtual const Vector3D_t&				GetEulerRotation() const = 0;
		virtual const Vector3D_t&				GetTargetDirection() const = 0;
		virtual const Matrix4x4_t&				GetViewMatrix() = 0;
		virtual const Matrix4x4_t&				GetProjectionMatrix() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define CAMERA_INTERFACE_VERSION "LE_Camera004"

//---------------------------------------------------------------------//

#endif // !ICAMERA_H

