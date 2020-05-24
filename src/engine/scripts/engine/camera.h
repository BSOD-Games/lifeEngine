//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_CAMERA_H
#define API_CAMERA_H

#include "../common/types.h"
#include "../mathlib/mathlib.h"

//---------------------------------------------------------------------//

typedef enum
{
	CSM_NONE			= 0,
	CSM_FORWARD			= 1 << 0,
	CSM_BACKWARD		= 1 << 1,
	CSM_LEFT			= 1 << 2,
	CSM_RIGHT			= 1 << 3
} cameraSideMove_t;

//---------------------------------------------------------------------//

/* Create */
extern camera_t					Camera_Create();

/* Delete */
extern void						Camera_Delete( camera_t Object );

/* Init projection perspective */
extern void						Camera_InitProjectionPerspective( camera_t Object, float FOV, float Aspect, float Near, float Far );

/* Init projection ortho */
extern void						Camera_InitProjectionOrtho( camera_t Object, float Left, float Right, float Bottom, float Top, float Near, float Far );

/* Move by vector */
extern void						Camera_MoveByVector( camera_t Object, vec3f_t FactorMove );

/* Move by side */
extern void						Camera_MoveBySide( camera_t Object, cameraSideMove_t SideMove, float MoveSpeed );

/* Rotate by vector*/
extern void						Camera_RotateByVector( camera_t Object, vec3f_t FactorRotate );

/* Rotate by vector */
extern void						Camera_RotateByMouse( camera_t Object, vec3f_t MouseOffset, float MouseSensitivit, bool_t ConstrainYaw );

/* Scree to world */
extern ray_t					Camera_ScreenToWorld( camera_t Object, vec2f_t Coords, vec2f_t ViewportSize );

/* World to screen */
extern vec3f_t					Camera_WorldToScreen( camera_t Object, vec3f_t Coords, vec2f_t ViewportSize );

/* Set position */
extern void						Camera_SetPosition( camera_t Object, vec3f_t Position );

/* Set rotation */
extern void						Camera_SetRotation( camera_t Object, vec3f_t Rotation );

/* Set target direction */
extern void						Camera_SetTargetDirection( camera_t Object, vec3f_t TargetDirection );

/* Set up */
extern void						Camera_SetUp( camera_t Object, vec3f_t Up );

/* Is visible BBox */
extern bool_t					Camera_IsVisibleBBox( camera_t Object, vec3f_t minPosition, vec3f_t maxPosition );

/* Is visible sphere */
extern bool_t					Camera_IsVisibleSphere( camera_t Object, vec3f_t Position, float Radius );

/* Get near */
extern float					Camera_GetNear( camera_t Object );

/* Get far */
extern float					Camera_GetFar( camera_t Object );

/* Get position */
extern vec3f_t					Camera_GetPosition( camera_t Object );

/* Get up */
extern vec3f_t					Camera_GetUp( camera_t Object );

/* Get right */
extern vec3f_t					Camera_GetRight( camera_t Object );

/* Get direction move */
extern vec3f_t					Camera_GetDirectionMove( camera_t Object, cameraSideMove_t SideMove );

/* Get quat rotation */
extern quat_t					Camera_GetQuatRotation( camera_t Object );

/* Get euler rotation */
extern vec3f_t					Camera_GetEulerRotation( camera_t Object );

/* Get target directon */
extern vec3f_t					Camera_GetTargetDirection( camera_t Object );

//---------------------------------------------------------------------//

#endif // API_CAMERA_H
