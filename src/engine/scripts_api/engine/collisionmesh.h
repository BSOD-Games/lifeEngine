//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_COLLISIONMESH_H
#define API_COLLISIONMESH_H

#include "../common/types.h"
#include "../mathlib/mathlib.h"

//---------------------------------------------------------------------//

typedef struct
{
	vec3f_t*			verteces;
	uint32_t			countVerteces;
	uint32_t*			indeces;
	uint32_t			countIndeces;
} collisionMesh_t;

//---------------------------------------------------------------------//

#endif // !API_COLLISIONMESH_H
