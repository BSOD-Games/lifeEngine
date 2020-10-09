// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "World/Components/CameraComponent.h"

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
	quatRotation( 1.f, 0.f, 0.f, 0.f ),
	eulerRotation( 0.f, 0.f, 0.f ),
	projectionMatrix( 1.f ),
	viewMatrix( 1.f )
{}

/**
 * Rotate
 */
void le::CameraComponent::Rotate( const FVector3D& InFactorRotate )
{
	// 6.28319 radians = 360 degrees

	if ( InFactorRotate.z != 0.f )
	{
		eulerRotation.z += glm::radians( InFactorRotate.z );
		if ( eulerRotation.z < -6.28319f || eulerRotation.z > 6.28319f )		eulerRotation.z = 0.f;
	}

	if ( InFactorRotate.x != 0.f )
	{
		eulerRotation.x += glm::radians( InFactorRotate.x );
		if ( eulerRotation.x < -6.28319f || eulerRotation.x > 6.28319f )		eulerRotation.x = 0.f;
	}

	if ( InFactorRotate.y != 0.f )
	{
		eulerRotation.y += glm::radians( InFactorRotate.y );
		if ( eulerRotation.y < -6.28319f || eulerRotation.y > 6.28319f )		eulerRotation.y = 0.f;
	}

	quatRotation =
		glm::angleAxis( eulerRotation.z, glm::vec3( 0.f, 0.f, 1.f ) ) *
		glm::angleAxis( eulerRotation.x, glm::vec3( 1.f, 0.f, 0.f ) ) *
		glm::angleAxis( eulerRotation.y, glm::vec3( 0.f, 1.f, 0.f ) );

	isNeedUpdateViewMatrix = true;
}

/**
 * Rotate by mouse
 */
void le::CameraComponent::RotateByMouse( const FVector2D& InMouseOffset, float InSensitivity, bool InConstrainYaw )
{
	// 1.5708 radians = 90 degrees
	// 6.28319 radians = 360 degrees

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

	quatRotation =
		glm::angleAxis( eulerRotation.z, glm::vec3( 0.f, 0.f, 1.f ) ) *
		glm::angleAxis( eulerRotation.x, glm::vec3( 1.f, 0.f, 0.f ) ) *
		glm::angleAxis( eulerRotation.y, glm::vec3( 0.f, 1.f, 0.f ) );

	isNeedUpdateViewMatrix = true;
}

/**
 * Set rotation
 */
void le::CameraComponent::SetRotation( const FVector3D& InRotation )
{
	// 6.28319 radians = 360 degrees

	eulerRotation = le::FVector3D( glm::radians( InRotation.x ), glm::radians( InRotation.y ), glm::radians( InRotation.z ) );

	if ( eulerRotation.z < -6.28319f || eulerRotation.z > 6.28319f )	eulerRotation.z = 0.f;
	if ( eulerRotation.x < -6.28319f || eulerRotation.x > 6.28319f )	eulerRotation.x = 0.f;
	if ( eulerRotation.y < -6.28319f || eulerRotation.y > 6.28319f )	eulerRotation.y = 0.f;

	quatRotation =
		glm::angleAxis( eulerRotation.z, glm::vec3( 0.f, 0.f, 1.f ) ) *
		glm::angleAxis( eulerRotation.x, glm::vec3( 1.f, 0.f, 0.f ) ) *
		glm::angleAxis( eulerRotation.y, glm::vec3( 0.f, 1.f, 0.f ) );

	isNeedUpdateViewMatrix = true;
}

/**
 * Update view matrix
 */
void le::CameraComponent::UpdateViewMatrix() const
{
	FVector3D		globalPosition = GetGlobalPosition();

	targetDirection = localTargetDirection * quatRotation;
	axisUp = localAxisUp * quatRotation;
	axisRight = glm::normalize( glm::cross( targetDirection, FVector3D( 0.f, 1.f, 0.f ) ) );
	viewMatrix = glm::lookAt( globalPosition, globalPosition + targetDirection, axisUp );

	isNeedUpdateViewMatrix = false;
}
