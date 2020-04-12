//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_BODY_H
#define API_BODY_H

#include "../common/types.h"
#include "../mathlib/mathlib.h"
#include "collider.h"

//---------------------------------------------------------------------//

typedef void*				body_t;

//---------------------------------------------------------------------//

/* Create body */
extern body_t				Body_Create();

/* Delete body */
extern void					Body_Delete( body_t Object );

/* Initialize body */
extern bool_t				Body_Initialize( body_t Object, collider_t Collider, float Mass, vec3f_t Inertia, bool_t IsStatic );

/* Activate body */
extern void					Body_Activate( body_t Object );

/* Apply impulse */
extern void					Body_ApplyImpulse( body_t Object, vec3f_t Factor, vec3f_t CenterBody );

/* Apply force */
extern void					Body_ApplyForce( body_t Object, vec3f_t Factor, vec3f_t CenterBody );

/* Freeze rotation */
extern void					Body_FreezeRotation( body_t Object, bool_t X, bool_t Y, bool_t Z );

/* Set position */
extern void					Body_SetPosition( body_t Object, vec3f_t Position );

/* Set rotation */
extern void					Body_SetRotation( body_t Object, quat_t Rotation );

/* Is static */
extern bool_t				Body_IsStatic( body_t Object );

/* Get collider */
extern collider_t			Body_GetCollider( body_t Object );

/* Get mass */
extern float				Body_GetMass( body_t Object );

/* Get inertia */
extern vec3f_t				Body_GetInertia( body_t Object );

/* Get position */
extern vec3f_t				Body_GetPosition( body_t Object );

/* Get rotation */
extern quat_t				Body_GetRotation( body_t Object );

//---------------------------------------------------------------------//

#endif // API_BODY_H
