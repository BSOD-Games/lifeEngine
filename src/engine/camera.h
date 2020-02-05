//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef CAMERA_H
#define CAMERA_H

#include "engine/icamera.h"
#include "frustum.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Camera : public ICamera
	{
	public:
		// ICamera
		virtual void							InitProjection_Perspective( float FOV, float Aspect, float Near, float Far );
		virtual void							InitProjection_Ortho( float Left, float Right, float Bottom, float Top, float Near = 0.f, float Far = 1.f );
		virtual void							Move( const Vector3D_t& FactorMove );
		virtual void							Move( CAMERA_SIDE_MOVE SideMove, float MoveSpeed );
		virtual void							Rotate( const Vector3D_t& FactorRotate );
		virtual void							RotateByMouse( const Vector2D_t& MouseOffset, float MouseSensitivit, bool ConstrainYaw = true );
		virtual Ray								ScreenToWorld( const Vector2D_t& Coords, const Vector2D_t& ViewportSize );
		virtual	Vector3D_t						WorldToScreen( const Vector3D_t& Coords, const Vector2D_t& ViewportSize );

		virtual void							SetPosition( const Vector3D_t& Position );
		virtual void							SetRotation( const Vector3D_t& Rotation );
		virtual void							SetTargetDirection( const Vector3D_t& TargetDirection );
		virtual void							SetUp( const Vector3D_t& Up );

		virtual bool							IsVisible( const Vector3D_t& MinPosition, const Vector3D_t& MaxPosition );
		virtual bool							IsVisible( const Vector3DInt_t& MinPosition, const Vector3DInt_t& MaxPosition );
		virtual bool							IsVisible( const Vector3DInt_t& Position, float Radius );

		virtual float							GetNear() const;
		virtual float							GetFar() const;
		virtual const Vector3D_t&				GetPosition() const;
		virtual const Vector3D_t&				GetUp() const;
		virtual const Vector3D_t&				GetRight() const;
		virtual const Quaternion_t&				GetQuatRotation() const;
		virtual const Vector3D_t&				GetEulerRotation() const;
		virtual const Vector3D_t&				GetTargetDirection() const;
		virtual const Matrix4x4_t&				GetViewMatrix();
		virtual const Matrix4x4_t&				GetProjectionMatrix() const;

		// Camera
		Camera();
		~Camera();

		inline const Frustum&					GetFrusrum() const
		{
			return frustum;
		};

	private:
		void									Update();

		bool				isNeedUpdate;
		float				near;
		float				far;

		Vector3D_t			position;
		Vector3D_t			targetDirection;
		Vector3D_t			up;
		Vector3D_t			right;
		Vector3D_t			eulerRotation;
		Quaternion_t		quatRotation;
		Frustum				frustum;
			
		Vector3D_t			localTargetDirection;
		Vector3D_t			localUp;

		Matrix4x4_t			matrixView;
		Matrix4x4_t			matrixProjection;

	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !CAMERA_H

