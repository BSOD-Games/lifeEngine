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

//---------------------------------------------------------------------//

typedef struct
{
	float x, y, z;
} vec3_t;

typedef struct
{
	float x, y, z, w;
} vec4_t;
typedef vec4_t quat_t;

/* Move model */
extern void			Model_Move( void* Model, vec3_t FactorMove );

/* Rotate model by euler angles */
extern void			Model_EulerRotate( void* Model, vec3_t FactorRotate );

/* Rotate model by quaternion */
extern void			Model_QuatRotate( void* Model, quat_t FactorRotate );

/* Scale model */
extern void			Model_Scale( void* Model, vec3_t FactorScale );

/* Set position model */
extern void			Model_SetPosition( void* Model, vec3_t Position );

/* Set rotation by euler angles model */
extern void			Model_SetEulerRotation( void* Model, vec3_t Rotation );

/* Set rotation by quaternion model */
extern void			Model_SetQuatRotation( void* Model, quat_t Rotation );

/* Set model scale */
extern void			Model_SetScale( void* Model, vec3_t Scale );

/* Get model position */
extern vec3_t		Model_GetPosition( void* Model );

/* Get quaternion model rotation */
extern quat_t		Model_GetRotation( void* Model );

/* Get scale model */
extern vec3_t		Model_GetScale( void* Model );

//---------------------------------------------------------------------//

#endif // !API_MODEL_H
