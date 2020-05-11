//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_SPRITE_H
#define API_SPRITE_H

#include "../common/types.h"
#include "../mathlib/mathlib.h"
#include "../studiorender/material.h"

//---------------------------------------------------------------------//

typedef enum
{
	ST_SPRITE_STATIC,
	ST_SPRITE_ROTATING,
	ST_SPRITE_ROTATING_ONLY_VERTICAL
} spriteType_t;

typedef void*			sprite_t;

//---------------------------------------------------------------------//

/* Create sprite */
extern sprite_t						Sprite_Create();

/* Delete sprite */
extern void							Sprite_Delete( sprite_t Object );

/* Initialize */
extern bool_t						Sprite_Initialize( sprite_t Object, vec2f_t Size, material_t Material, spriteType_t SpriteType );

/* Set material */
extern void							Sprite_SetMaterial( sprite_t Object, material_t Material );

/* Set size */
extern void							Sprite_SetSize( sprite_t Object, vec2f_t Size );

/* Set type */
extern void							Sprite_SetType( sprite_t Object, spriteType_t SpriteType );

/* Is created */
extern bool_t						Sprite_IsCreate( sprite_t Object );

/* Get material */
extern material_t					Sprite_GetMaterial( sprite_t Object );

/* Get min */
extern vec3f_t						Sprite_GetMin( sprite_t Object );

/* Get max */
extern vec3f_t						Sprite_GetMax( sprite_t Object );

/* Get size */
extern vec2f_t						Sprite_GetSize( sprite_t Object );

/* Get type */
extern spriteType_t					Sprite_GetType( sprite_t Object );

/* Move */
extern void							Sprite_Move( sprite_t Object, vec3f_t FactorMove );

/* Rotate sprite by euler angles */
extern void							Sprite_EulerRotate( sprite_t Object, vec3f_t FactorRotate );

/* Rotate sprite by quaternion */
extern void							Sprite_QuatRotate( sprite_t Object, quat_t FactorRotate );

/* Scale sprite */
extern void							Sprite_Scale( sprite_t Object, vec3f_t FactorScale );

/* Set position sprite */
extern void							Sprite_SetPosition( sprite_t Object, vec3f_t Position );

/* Set rotation by euler angles sprite */
extern void							Sprite_SetEulerRotation( sprite_t Object, vec3f_t Rotation );

/* Set rotation by quaternion sprite */
extern void							Sprite_SetQuatRotation( sprite_t Object, quat_t Rotation );

/* Set sprite scale */
extern void							Sprite_SetScale( sprite_t Object, vec3f_t Scale );

/* Get sprite position */
extern vec3f_t						Sprite_GetPosition( sprite_t Object );

/* Get quaternion sprite rotation */
extern quat_t						Sprite_GetRotation( sprite_t Object );

/* Get scale sprite */
extern vec3f_t						Sprite_GetScale( sprite_t Object );

//---------------------------------------------------------------------//

#endif // API_SPRITE_H
