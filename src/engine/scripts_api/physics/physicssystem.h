//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_PHYSICSSYSTEM_H
#define API_PHYSICSSYSTEM_H

#include "../common/types.h"
#include "../mathlib/mathlib.h"
#include "../engine/camera.h"
#include "body.h"
#include "charctercontroller.h"

//---------------------------------------------------------------------//

/* Add body */
extern void						PhysicsSystem_AddBody( body_t Body );

/* Remove body */
extern void						PhysicsSystem_RemoveBody( body_t Body );

/* Remove all bodies */
extern void						PhysicsSystem_RemoveAllBodies();

/* Add charcter controller */
extern void						PhysicsSystem_AddCharcterController( charcterController_t CharcterController );

/* Remove charcter controller */
extern void						PhysicsSystem_RemoveCharcterController( charcterController_t CharcterController );

/* Remove all charcter controllers */
extern void						PhysicsSystem_RemoveAllCharcterControllers();

/* Set debug camera */
extern void						PhysicsSystem_SetDebugCamera( camera_t Camera );

/* Set gravity */
extern void						PhysicsSystem_SetGravity( vec3f_t Gravity );

/* Get gravity */
extern vec3f_t					PhysicsSystem_GetGravity();

/* Get count bodes */
extern uint32_t					PhysicsSystem_GetCountBodes();

/* Get body */
extern body_t					PhysicsSystem_GetBody( uint32_t Index );

/* Get bodies */
extern body_t*					PhysicsSystem_GetBodies();

/* Get count charcter controllers */
extern uint32_t					PhysicsSystem_GetCountCharcterControllers();

/* Get charcter controller */
extern charcterController_t		PhysicsSystem_GetCharcterController( uint32_t Index );

/* Get charcter controllers */
extern charcterController_t*	PhysicsSystem_GetCharcterControllers();

//---------------------------------------------------------------------//

#endif // API_PHYSICSSYSTEM_H
