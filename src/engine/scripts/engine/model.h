//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_MODEL_H
#define API_MODEL_H

#include "../common/types.h"
#include "../mathlib/mathlib.h"

//---------------------------------------------------------------------//

/* Create */
extern model_t		Model_Create();

/* Delete */
extern void			Model_Delete( model_t Model );

/* Move model */
extern void			Model_Move( model_t Model, vec3f_t FactorMove );

/* Rotate model by euler angles */
extern void			Model_EulerRotate( model_t Model, vec3f_t FactorRotate );

/* Rotate model by quaternion */
extern void			Model_QuatRotate( model_t Model, quat_t FactorRotate );

/* Scale model */
extern void			Model_Scale( model_t Model, vec3f_t FactorScale );

/* Set position model */
extern void			Model_SetPosition( model_t Model, vec3f_t Position );

/* Set rotation by euler angles model */
extern void			Model_SetEulerRotation( model_t Model, vec3f_t Rotation );

/* Set rotation by quaternion model */
extern void			Model_SetQuatRotation( model_t Model, quat_t Rotation );

/* Set model scale */
extern void			Model_SetScale( model_t Model, vec3f_t Scale );

/* Get model position */
extern vec3f_t		Model_GetPosition( model_t Model );

/* Get quaternion model rotation */
extern quat_t		Model_GetRotation( model_t Model );

/* Get scale model */
extern vec3f_t		Model_GetScale( model_t Model );

/* Set mesh */
extern void			Model_SetMesh( model_t Model, mesh_t Mesh );

/* Set start face */
extern void			Model_SetStartFace( model_t Model, uint32_t StartFace );

/* Set count face */
extern void			Model_SetCountFace( model_t Model, uint32_t CountFace );

/* Get mesh */
extern mesh_t		Model_GetMesh( model_t Model );

/* Get start face */
extern uint32_t		Model_GetStartFace( model_t Model );

/* Get count face */
extern uint32_t		Model_GetCountFace( model_t Model );

//---------------------------------------------------------------------//

#endif // !API_MODEL_H
