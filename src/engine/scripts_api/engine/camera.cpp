//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/global.h"
#include "engine/ifactory.h"
#include "engine/camera.h"
#include "common/ray.h"

namespace scripts_api
{
#include "camera.h"
}

#define CAMERA( Object )		static_cast< le::Camera* >( Object )

// ------------------------------------------------------------------------------------ //
// Init projection perspective
// ------------------------------------------------------------------------------------ //
void scripts_api::Camera_InitProjectionPerspective( camera_t Object, float FOV, float Aspect, float Near, float Far )
{
	if ( !Object ) return;
	CAMERA( Object )->InitProjection_Perspective( FOV, Aspect, Near, Far );
}

// ------------------------------------------------------------------------------------ //
// Init projection ortho
// ------------------------------------------------------------------------------------ //
void scripts_api::Camera_InitProjectionOrtho( camera_t Object, float Left, float Right, float Bottom, float Top, float Near, float Far )
{
	if ( !Object ) return;
	CAMERA( Object )->InitProjection_Ortho( Left, Right, Bottom, Top, Near, Far );
}

// ------------------------------------------------------------------------------------ //
// Move by vector
// ------------------------------------------------------------------------------------ //
void scripts_api::Camera_MoveByVector( camera_t Object, vec3f_t FactorMove )
{
	if ( !Object ) return;
	CAMERA( Object )->Move( { FactorMove.x, FactorMove.y, FactorMove.z } );
}

// ------------------------------------------------------------------------------------ //
// Move by side
// ------------------------------------------------------------------------------------ //
void scripts_api::Camera_MoveBySide( camera_t Object, cameraSideMove_t SideMove, float MoveSpeed )
{
	if ( !Object ) return;
	CAMERA( Object )->Move( ( le::CAMERA_SIDE_MOVE ) SideMove, MoveSpeed );
}

// ------------------------------------------------------------------------------------ //
// Rotate by vector
// ------------------------------------------------------------------------------------ //
void scripts_api::Camera_RotateByVector( camera_t Object, vec3f_t FactorRotate )
{
	if ( !Object ) return;
	CAMERA( Object )->Rotate( { FactorRotate.x, FactorRotate.y, FactorRotate.z } );
}

// ------------------------------------------------------------------------------------ //
// Rotate by mouse
// ------------------------------------------------------------------------------------ //
void scripts_api::Camera_RotateByMouse( camera_t Object, vec3f_t MouseOffset, float MouseSensitivit, bool_t ConstrainYaw )
{
	if ( !Object ) return;
	CAMERA( Object )->RotateByMouse( { MouseOffset.x, MouseOffset.y }, MouseSensitivit, ConstrainYaw );
}

// ------------------------------------------------------------------------------------ //
// Screen to world
// ------------------------------------------------------------------------------------ //
scripts_api::ray_t scripts_api::Camera_ScreenToWorld( camera_t Object, vec2f_t Coords, vec2f_t ViewportSize )
{
	if ( !Object ) return {};
	le::Ray			ray = CAMERA( Object )->ScreenToWorld( { Coords.x, Coords.y }, { ViewportSize.x, ViewportSize.y } );
	return { { ray.origin.x, ray.origin.y, ray.origin.z }, { ray.direction.x, ray.direction.y, ray.direction.z } };
}

// ------------------------------------------------------------------------------------ //
// World to screen
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Camera_WorldToScreen( camera_t Object, vec3f_t Coords, vec2f_t ViewportSize )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };
	le::Vector3D_t			vec = CAMERA( Object )->WorldToScreen( { Coords.x, Coords.y, Coords.z }, { ViewportSize.x, ViewportSize.y } );
	return { vec.x, vec.y, vec.z };
}

// ------------------------------------------------------------------------------------ //
// Set position
// ------------------------------------------------------------------------------------ //
void scripts_api::Camera_SetPosition( camera_t Object, vec3f_t Position )
{
	if ( !Object ) return;
	CAMERA( Object )->SetPosition( { Position.x, Position.y, Position.z } );
}

// ------------------------------------------------------------------------------------ //
// Set rotation
// ------------------------------------------------------------------------------------ //
void scripts_api::Camera_SetRotation( camera_t Object, vec3f_t Rotation )
{
	if ( !Object ) return;
	CAMERA( Object )->SetRotation( { Rotation.x, Rotation.y, Rotation.z } );
}

// ------------------------------------------------------------------------------------ //
// Set target direction
// ------------------------------------------------------------------------------------ //
void scripts_api::Camera_SetTargetDirection( camera_t Object, vec3f_t TargetDirection )
{
	if ( !Object ) return;
	CAMERA( Object )->SetTargetDirection( { TargetDirection.x, TargetDirection.y, TargetDirection.z } );
}

// ------------------------------------------------------------------------------------ //
// Set up
// ------------------------------------------------------------------------------------ //
void scripts_api::Camera_SetUp( camera_t Object, vec3f_t Up )
{
	if ( !Object ) return;
	CAMERA( Object )->SetUp( { Up.x, Up.y, Up.z } );
}

// ------------------------------------------------------------------------------------ //
// Is visible BBox
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::Camera_IsVisibleBBox( camera_t Object, vec3f_t minPosition, vec3f_t maxPosition )
{
	if ( !Object ) return B_FALSE;
	return CAMERA( Object )->IsVisible( le::Vector3D_t( minPosition.x, minPosition.y, minPosition.z ), le::Vector3D_t( maxPosition.x, maxPosition.y, maxPosition.z ) ) ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Is visible sphere
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::Camera_IsVisibleSphere( camera_t Object, vec3f_t Position, float Radius )
{
	if ( !Object ) return B_FALSE;
	return CAMERA( Object )->IsVisible( le::Vector3D_t( Position.x, Position.y, Position.z ), Radius ) ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Get near
// ------------------------------------------------------------------------------------ //
float scripts_api::Camera_GetNear( camera_t Object )
{
if ( !Object ) return B_FALSE;
return CAMERA( Object )->GetNear();
}

// ------------------------------------------------------------------------------------ //
// Get far
// ------------------------------------------------------------------------------------ //
float scripts_api::Camera_GetFar( camera_t Object )
{
if ( !Object ) return B_FALSE;
return CAMERA( Object )->GetFar();
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Camera_GetPosition( camera_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&			vec = CAMERA( Object )->GetPosition();
	return { vec.x, vec.y, vec.z };
}

// ------------------------------------------------------------------------------------ //
// Get up
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Camera_GetUp( camera_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&			vec = CAMERA( Object )->GetUp();
	return { vec.x, vec.y, vec.z };
}

// ------------------------------------------------------------------------------------ //
// Get right
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Camera_GetRight( camera_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&			vec = CAMERA( Object )->GetRight();
	return { vec.x, vec.y, vec.z };
}

// ------------------------------------------------------------------------------------ //
// Get direction move
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Camera_GetDirectionMove( camera_t Object, cameraSideMove_t SideMove )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&			vec = CAMERA( Object )->GetDirectionMove( ( le::CAMERA_SIDE_MOVE ) SideMove );
	return { vec.x, vec.y, vec.z };
}

// ------------------------------------------------------------------------------------ //
// Get quat ritatuin
// ------------------------------------------------------------------------------------ //
scripts_api::quat_t scripts_api::Camera_GetQuatRotation( camera_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f, 0.f };

	const le::Quaternion_t&			quat = CAMERA( Object )->GetQuatRotation();
	return { quat.x, quat.y, quat.z, quat.w };
}

// ------------------------------------------------------------------------------------ //
// Get euler rotation
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Camera_GetEulerRotation( camera_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&			vec = CAMERA( Object )->GetEulerRotation();
	return { vec.x, vec.y, vec.z };
}

// ------------------------------------------------------------------------------------ //
// Get target direction
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Camera_GetTargetDirection( camera_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&			vec = CAMERA( Object )->GetTargetDirection();
	return { vec.x, vec.y, vec.z };
}

// ------------------------------------------------------------------------------------ //
// Create
// ------------------------------------------------------------------------------------ //
scripts_api::camera_t scripts_api::Camera_Create()
{
	return le::g_engineFactory->Create( CAMERA_INTERFACE_VERSION );
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void scripts_api::Camera_Delete( camera_t Object )
{
	if ( !Object ) return;

	le::Camera*		camera = static_cast< le::Camera* >( Object );
	if ( camera->GetCountReferences() <= 1 )
		camera->Release();
	else
		camera->DecrementReference();

	Object = nullptr;
}
