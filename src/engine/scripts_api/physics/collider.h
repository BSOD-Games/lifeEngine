//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_COLLIDER_H
#define API_COLLIDER_H

#include "../mathlib/mathlib.h"
#include "shapeboxdescriptor.h"
#include "shapecylinderdescriptor.h"
#include "shapecapsuledescriptor.h"
#include "shapespheredescriptor.h"
#include "shapemeshdescriptor.h"
#include "shapeconedescriptor.h"
#include "shapeconvexhulldescriptor.h"

//---------------------------------------------------------------------//

typedef void*			collider_t;

//---------------------------------------------------------------------//

/* Create collider */
extern collider_t			Collider_Create();

/* Delete collider */
extern void					Collider_Delete( collider_t Object );

/* Add box */
extern void					Collider_AddBox( collider_t Object, shapeBoxDescriptor_t Shape, mat4_t LocalTransformation );

/* Add cylinder */
extern void					Collider_AddCylinder( collider_t Object, shapeCylinderDescriptor_t Shape, mat4_t LocalTransformation );

/* Add capsule */
extern void					Collider_AddCapsule( collider_t Object, shapeCapsuleDescriptor_t Shape, mat4_t LocalTransformation );

/* Add sphere */
extern void					Collider_AddSphere( collider_t Object, shapeSphereDescriptor_t Shape, mat4_t LocalTransformation );

/* Add mesh */
extern void					Collider_AddMesh( collider_t Object, shapeMeshDescriptor_t Shape, mat4_t LocalTransformation );

/* Add cone */
extern void					Collider_AddCone( collider_t Object, shapeConeDescriptor_t Shape, mat4_t LocalTransformation );

/* Add convex hull */
extern void					Collider_AddConvexHull( collider_t Object, shapeConvexHullDescriptor_t Shape, mat4_t LocalTransformation );

/* Remove shape */
extern void					Collider_RemoveShape( collider_t Object, uint32_t Index );

/* Remove all shape */
extern void					Collider_RemoveAllShapes( collider_t Object );

/* Scale */
extern void					Collider_Scale( collider_t Object, vec3f_t FactorScale );

/* Set scale */
extern void					Collider_SetScale( collider_t Object, vec3f_t Scale );

/* Get AABB */
extern void					Collider_GetAABB( collider_t Object, vec3f_t* Min, vec3f_t* Max );

/* Get count shapes */
extern uint32_t				Collider_GetCountShapes( collider_t Object );

/* Get scale */
extern vec3f_t				Collider_GetScale( collider_t Object );

//---------------------------------------------------------------------//

#endif // API_COLLIDER_H
