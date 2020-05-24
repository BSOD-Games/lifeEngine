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
extern void							PhysicsModel_InitializeMesh( physicsModel_t Object, vec3f_t* Verteces, uint32_t CountVerteces, uint32_t* Indeces, uint32_t CountIndeces );

/* Clear mesh */
extern void							PhysicsModel_ClearMesh( physicsModel_t Object );

/* Is initialized mesh */
extern bool_t						PhysicsModel_IsInitializedMesh( physicsModel_t Object );

//---------------------------------------------------------------------//

#endif // API_PHYSICSMODEL_H
