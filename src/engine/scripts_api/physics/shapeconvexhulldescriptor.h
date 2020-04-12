//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_SHAPECONVEXHULLDESCRIPTOR_H
#define API_SHAPECONVEXHULLDESCRIPTOR_H

#include "../common/types.h"
#include "../mathlib/mathlib.h"

//---------------------------------------------------------------------//

typedef struct
{
	uint32_t            countVerteces;
	vec3f_t*			verteces;
} shapeConvexHullDescriptor_t;

//---------------------------------------------------------------------//

#endif // !API_SHAPECONVEXHULLDESCRIPTOR_H
