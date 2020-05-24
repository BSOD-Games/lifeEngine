//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_CHARCTERCONTROLLER_H
#define API_CHARCTERCONTROLLER_H

#include "../common/types.h"
#include "../mathlib/mathlib.h"
#include "shapecapsuledescriptor.h"

//---------------------------------------------------------------------//

/* Create charcter controller */
extern charcterController_t					CharcterController_Create();

/* Delete charcter controller */
extern void									CharcterController_Delete( charcterController_t Object );

/* Initialize */
extern void									CharcterController_Initialize( charcterController_t Object, shapeCapsuleDescriptor_t ShapeCapsuleDescriptor );

/* Walk */
extern void									CharcterController_Walk( charcterController_t Object, vec3f_t WalkDirection );

/* Jump */
extern void									CharcterController_Jump( charcterController_t Object, vec3f_t JumpForce );

/* Set velocity */
extern void									CharcterController_SetVelocity( charcterController_t Object, vec3f_t Velocity );

/* Set position */
extern void									CharcterController_SetPosition( charcterController_t Object, vec3f_t Position );

/* Set rotation */
extern void									CharcterController_SetRotation( charcterController_t Object, quat_t Rotation );

/* Can jump */
extern bool_t								CharcterController_CanJump( charcterController_t Object );

/* On ground */
extern bool_t								CharcterController_OnGround( charcterController_t Object );

/* Get velocity */
extern vec3f_t								CharcterController_GetVelocity( charcterController_t Object );

/* Get position */
extern vec3f_t								CharcterController_GetPosition( charcterController_t Object );

/* Get rotation */
extern quat_t								CharcterController_GetRotation( charcterController_t Object );

//---------------------------------------------------------------------//

#endif // API_CHARCTERCONTROLLER_H
