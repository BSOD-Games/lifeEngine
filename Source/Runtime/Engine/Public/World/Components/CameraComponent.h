// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Misc/EngineDefines.h"
#include "Math/Rect.h"
#include "World/Components/BaseComponent.h"

namespace le
{
	class CameraComponent : public BaseComponent
	{
	public:

		class Frustum
		{
		public:
			/* Update frustum */
			void Update( const FMatrix4x4& InProjectionMatrix, const FMatrix4x4& InViewMatrix );

			/* Is object visible */
			bool IsVisible( const FVector3D& InMinPosition, const FVector3D& InMaxPosition ) const;

		private:
			/* Normalize planes in frustum */
			void NormalizePlanes();

			FVector4D		planes[ 6 ];
		};

		/* Constructor */
		CameraComponent();

		/* Move camera */
		FORCEINLINE void Move( const FVector3D& InFactorMove )
		{
			isNeedUpdateViewMatrix = true;
			position += InFactorMove;
		}

		/* Rotate camera */
		void Rotate( const FVector3D& InFactorRotate );
		
		/* Rotate by mouse */
		void RotateByMouse( const FVector2D& InMouseOffset, float InSensitivity, bool InConstrainYaw = true );

		/* Set perspective projection */
		FORCEINLINE void SetPerspectiveProjection( float InFOV, float InAspect, float InNear, float InFar )
		{
			projectionMatrix = glm::perspectiveRH_ZO( glm::radians( InFOV ), InAspect, InNear, InFar );
			near = InNear;
			far = InFar;
		}

		/* Set ortho projection */
		FORCEINLINE void SetOrthoProjection( float InLeft, float InRight, float InBottom, float InTop, float InNear = 0.f, float InFar = 1.f )
		{
			projectionMatrix = glm::orthoRH_ZO( InLeft, InRight, InBottom, InTop, InNear, InFar );
			near = InNear;
			far = InFar;
		}

		/* Set position */		
		FORCEINLINE void SetPosition( const FVector3D& InPosition )
		{
			isNeedUpdateViewMatrix = true;
			position = InPosition;
		}

		/* Set rotation */
		void SetRotation( const FVector3D& InRotation );

		/* Set target direction */
		FORCEINLINE void SetTargetDirection( const FVector3D& InTargetDirection )
		{
			isNeedUpdateViewMatrix = true;
			targetDirection = InTargetDirection;
		}

		/* Set axis up */
		FORCEINLINE void SetAxisUp( const FVector3D& InAxisUp )
		{
			isNeedUpdateViewMatrix = true;
			axisUp = InAxisUp;
		}

		/* Get near */
		FORCEINLINE float GetNear() const								{ return near; }

		/* Get far */
		FORCEINLINE float GetFar() const								{ return far; }

		/* Get axis up */
		FORCEINLINE const FVector3D& GetAxisUp() const					{ return axisUp; }

		/* Get axis right */
		FORCEINLINE const FVector3D& GetAxisRight() const				{ return axisRight; }

		/* Get quaterion rotation */
		FORCEINLINE const FQuaternion& GetQuatRotation() const			{ return quatRotation; }

		/* Get euler rotation */
		FORCEINLINE const FVector3D& GetEulerRotation() const			{ return eulerRotation; }

		/* Get target direction */
		FORCEINLINE const FVector3D& GetTargetDirection() const			{ return targetDirection; }

		/* Get view matrix */
		FORCEINLINE const FMatrix4x4& GetViewMatrix() const
		{
			if ( isNeedUpdateViewMatrix )		UpdateViewMatrix();
			return viewMatrix;
		}

		/* Get projection matrix */
		FORCEINLINE const FMatrix4x4& GetProjectionMatrix() const		{ return projectionMatrix; }
 
		/* Get frustum */
		FORCEINLINE const Frustum& GetFrustum() const					
		{ 
			if ( isNeedUpdateViewMatrix )		UpdateViewMatrix();
			return frustum; 
		}

	private:
		/* Update view matrix */
		void UpdateViewMatrix() const;

		mutable bool			isNeedUpdateViewMatrix;
		float					near;
		float					far;

		mutable FVector3D		targetDirection;
		mutable FVector3D		axisUp;
		mutable FVector3D		axisRight;
		FVector3D				eulerRotation;
		FQuaternion				quatRotation;
		mutable Frustum			frustum;

		FVector3D				localTargetDirection;
		FVector3D				localAxisUp;

		mutable FMatrix4x4		viewMatrix;
		FMatrix4x4				projectionMatrix;
	};
}

#endif // !CAMERACOMPONENT_H
