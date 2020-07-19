//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_PHYSICSMODEL_H
#define API_PHYSICSMODEL_H

#include "../common/types.h"
#include "../mathlib/mathlib.h"

//---------------------------------------------------------------------//

/* Physics model create */
extern physicsModel_t				PhysicsModel_Create();

/* Physics model delete */
extern void							PhysicsModel_Delete( physicsModel_t Object );

/* Initialize mesh */
extern void							PhysicsModel_Initialize( physicsModel_t Object, vec3f_t* Verteces, uint32_t CountVerteces, uint32_t* Indeces, uint32_t CountIndeces, bool_t IsStatic );

/* Clear mesh */
extern void							PhysicsModel_Clear( physicsModel_t Object );

/* Set masa */
extern void							PhysicsModel_SetMasa( physicsModel_t Object, float Masa );

/* Set inertia */
extern void							PhysicsModel_SetInertia( physicsModel_t Object, vec3f_t Inertia );

/* Is static */
extern bool_t						PhysicsModel_IsStatic( physicsModel_t Object );

/* Is initialized mesh */
extern bool_t						PhysicsModel_IsInitialized( physicsModel_t Object );

/* Get masa */
extern float						PhysicsModel_GetMasa( physicsModel_t Object );

/* Get inertia */
extern vec3f_t						PhysicsModel_GetInertia( physicsModel_t Object );

//---------------------------------------------------------------------//

#endif // API_PHYSICSMODEL_H
