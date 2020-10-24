// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "World/Components/CameraComponent.h"

 // ------------------
 // CAMERA COMPONENT
 // ------------------

/**
 * Constructor
 */
le::CameraComponent::CameraComponent() :
	isNeedUpdateViewMatrix( true ),
	near( 0.f ),
	far( 0.f ),
	axisUp( 0.f, 1.f, 0.f ),
	axisRight( 1.f, 0.f, 0.f ),
	localAxisUp( 0.f, 1.f, 0.f ),
	targetDirection( 0.f, 0.f, -1.f ),
	localTargetDirection( 0.f, 0.f, -1.f ),
	projectionMatrix( 1.f ),
	viewMatrix( 1.f )
{
	transformComponent.GetEventChannelUpdate().Subscribe( this, &CameraComponent::OnUpdateTransformComponent );
}

/**
 * Destructor
 */
le::CameraComponent::~CameraComponent()
{
	transformComponent.GetEventChannelUpdate().Unsubscribe( this, &CameraComponent::OnUpdateTransformComponent );
}

/**
 * Rotate by mouse
 */
void le::CameraComponent::RotateByMouse( const FVector2D& InMouseOffset, float InSensitivity, bool InConstrainYaw )
{
	// 1.5708 radians = 90 degrees
	// 6.28319 radians = 360 degrees

	FVector2D		eulerRotation( 0.f, 0.f );
	if ( InMouseOffset.x != 0 )
	{
		eulerRotation.y += glm::radians( InMouseOffset.x * InSensitivity );
		if ( eulerRotation.x < -6.28319f || eulerRotation.x > 6.28319f )		eulerRotation.x = 0.f;
	}

	if ( InMouseOffset.y != 0 )
	{
		eulerRotation.x += glm::radians( InMouseOffset.y * InSensitivity );

		if ( InConstrainYaw )
		{
			if ( eulerRotation.x > 1.5708f )			eulerRotation.x = 1.5708f;
			else if ( eulerRotation.x < -1.5708f )		eulerRotation.x = -1.5708f;
		}
		else if ( eulerRotation.x < -6.28319f || eulerRotation.x > 6.28319f )	eulerRotation.x = 0.f;
	}

	if ( eulerRotation.x == 0 && eulerRotation.y == 0 )		return;

	transformComponent.Rotate( MathEulerAnglesToQuaternion( eulerRotation.x, eulerRotation.y, 0.f ) );
	isNeedUpdateViewMatrix = true;
}

/**
 * Update view matrix
 */
void le::CameraComponent::UpdateViewMatrix() const
{
	FVector3D		position = transformComponent.GetGlobalPosition();
	FQuaternion		rotation = transformComponent.GetGlobalRotation();

	targetDirection = localTargetDirection * rotation;
	axisUp = localAxisUp * rotation;
	axisRight = glm::normalize( glm::cross( targetDirection, FVector3D( 0.f, 1.f, 0.f ) ) );
	viewMatrix = glm::lookAt( position, position + targetDirection, axisUp );
	frustum.Update( projectionMatrix, viewMatrix );

	isNeedUpdateViewMatrix = false;
}

/**
 * Event: Update transform component
 */
void le::CameraComponent::OnUpdateTransformComponent( const TransformComponent::SEventUpdate& InEventUpdate )
{
	isNeedUpdateViewMatrix = true;
}

// ------------------
// FRUSTUM
// ------------------

enum EFrustumSide
{
	FS_Right,
	FS_Left,
	FS_Bottom,
	FS_Top,
	FS_Back,
	FS_Front
};

/**
 * Update frustum
 */
void le::CameraComponent::Frustum::Update( const FMatrix4x4& InProjectionMatrix, const FMatrix4x4& InViewMatrix )
{
	FMatrix4x4				clippingPlane = InProjectionMatrix * InViewMatrix;

	planes[ FS_Right ].x = clippingPlane[ 0 ].w - clippingPlane[ 0 ].x;
	planes[ FS_Right ].y = clippingPlane[ 1 ].w - clippingPlane[ 1 ].x;
	planes[ FS_Right ].z = clippingPlane[ 2 ].w - clippingPlane[ 2 ].x;
	planes[ FS_Right ].w = clippingPlane[ 3 ].w - clippingPlane[ 3 ].x;

	planes[ FS_Left ].x = clippingPlane[ 0 ].w + clippingPlane[ 0 ].x;
	planes[ FS_Left ].y = clippingPlane[ 1 ].w + clippingPlane[ 1 ].x;
	planes[ FS_Left ].z = clippingPlane[ 2 ].w + clippingPlane[ 2 ].x;
	planes[ FS_Left ].w = clippingPlane[ 3 ].w + clippingPlane[ 3 ].x;

	planes[ FS_Bottom ].x = clippingPlane[ 0 ].w + clippingPlane[ 0 ].y;
	planes[ FS_Bottom ].y = clippingPlane[ 1 ].w + clippingPlane[ 1 ].y;
	planes[ FS_Bottom ].z = clippingPlane[ 2 ].w + clippingPlane[ 2 ].y;
	planes[ FS_Bottom ].w = clippingPlane[ 3 ].w + clippingPlane[ 3 ].y;

	planes[ FS_Top ].x = clippingPlane[ 0 ].w - clippingPlane[ 0 ].y;
	planes[ FS_Top ].y = clippingPlane[ 1 ].w - clippingPlane[ 1 ].y;
	planes[ FS_Top ].z = clippingPlane[ 2 ].w - clippingPlane[ 2 ].y;
	planes[ FS_Top ].w = clippingPlane[ 3 ].w - clippingPlane[ 3 ].y;

	planes[ FS_Back ].x = clippingPlane[ 0 ].w + clippingPlane[ 0 ].z;
	planes[ FS_Back ].y = clippingPlane[ 1 ].w + clippingPlane[ 1 ].z;
	planes[ FS_Back ].z = clippingPlane[ 2 ].w + clippingPlane[ 2 ].z;
	planes[ FS_Back ].w = clippingPlane[ 3 ].w + clippingPlane[ 3 ].z;

	planes[ FS_Front ].x = clippingPlane[ 0 ].w - clippingPlane[ 0 ].z;
	planes[ FS_Front ].y = clippingPlane[ 1 ].w - clippingPlane[ 1 ].z;
	planes[ FS_Front ].z = clippingPlane[ 2 ].w - clippingPlane[ 2 ].z;
	planes[ FS_Front ].w = clippingPlane[ 3 ].w - clippingPlane[ 3 ].z;

	NormalizePlanes();
}

/**
 * Normalize planes
 */
void le::CameraComponent::Frustum::NormalizePlanes()
{
	for ( uint32 side = 0; side < 6; ++side )
	{
		float		magnitude = glm::sqrt(  planes[ side ].x * planes[ side ].x +
											planes[ side ].y * planes[ side ].y +
											planes[ side ].z * planes[ side ].z );

		planes[ side ].x /= magnitude;
		planes[ side ].y /= magnitude;
		planes[ side ].z /= magnitude;
		planes[ side ].w /= magnitude;
	}
}

/**
 * Is object visible
 */
bool le::CameraComponent::Frustum::IsVisible( const FVector3D& InMinPosition, const FVector3D& InMaxPosition ) const
{
	for ( uint32 side = 0; side < 6; ++side )
	{
		if ( planes[ side ].x * InMinPosition.x + planes[ side ].y * InMinPosition.y + planes[ side ].z * InMinPosition.z + planes[ side ].w > 0 )  continue;
		if ( planes[ side ].x * InMaxPosition.x + planes[ side ].y * InMinPosition.y + planes[ side ].z * InMinPosition.z + planes[ side ].w > 0 )  continue;
		if ( planes[ side ].x * InMaxPosition.x + planes[ side ].y * InMaxPosition.y + planes[ side ].z * InMinPosition.z + planes[ side ].w > 0 )  continue;
		if ( planes[ side ].x * InMinPosition.x + planes[ side ].y * InMaxPosition.y + planes[ side ].z * InMinPosition.z + planes[ side ].w > 0 )  continue;
		if ( planes[ side ].x * InMinPosition.x + planes[ side ].y * InMinPosition.y + planes[ side ].z * InMaxPosition.z + planes[ side ].w > 0 )  continue;
		if ( planes[ side ].x * InMaxPosition.x + planes[ side ].y * InMinPosition.y + planes[ side ].z * InMaxPosition.z + planes[ side ].w > 0 )  continue;
		if ( planes[ side ].x * InMaxPosition.x + planes[ side ].y * InMaxPosition.y + planes[ side ].z * InMaxPosition.z + planes[ side ].w > 0 )  continue;
		if ( planes[ side ].x * InMinPosition.x + planes[ side ].y * InMaxPosition.y + planes[ side ].z * InMaxPosition.z + planes[ side ].w > 0 )  continue;

		return false;
	}

	return true;
}