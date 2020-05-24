//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_SHADERPARAMETER_H
#define API_SHADERPARAMETER_H

#include "../common/types.h"
#include "../mathlib/mathlib.h"

//---------------------------------------------------------------------//

typedef enum
{
	SPT_TEXTURE,
	SPT_FLOAT,
	SPT_INT,
	SPT_SHADER_FLAG,
	SPT_VECTOR_2D,
	SPT_VECTOR_3D,
	SPT_VECTOR_4D,
	SPT_MATRIX
} shaderParameterType_t;

//---------------------------------------------------------------------//

/* Create */
extern shaderParameter_t					ShaderParameter_Create();

/* Delete */
extern void									ShaderParameter_Delete( shaderParameter_t Object );

/* Clear */
extern void									ShaderParameter_Clear( shaderParameter_t Object );

/* Set name */
extern void									ShaderParameter_SetName( shaderParameter_t Object, const char* Name );

/* Set value int */
extern void									ShaderParameter_SetValueInt( shaderParameter_t Object, int Value );

/* Set value float */
extern void									ShaderParameter_SetValueFloat( shaderParameter_t Object, float Value );

/* Set value shader flag */
extern void									ShaderParameter_SetValueShaderFlag( shaderParameter_t Object, bool_t Value );

/* Set value vec2 */
extern void									ShaderParameter_SetValueVec2( shaderParameter_t Object, vec2f_t Value );

/* Set value vec3 */
extern void									ShaderParameter_SetValueVec3( shaderParameter_t Object, vec3f_t Value );

/* Set value vec4 */
extern void									ShaderParameter_SetValueVec4( shaderParameter_t Object, vec4f_t Value );

/* Set value mat4 */
extern void									ShaderParameter_SetValueMat4( shaderParameter_t Object, mat4_t Value );

/* Set value texture */
extern void									ShaderParameter_SetValueTexture( shaderParameter_t Object, texture_t Value );

/* Is defined */
extern bool_t								ShaderParameter_IsDefined( shaderParameter_t Object );

/* Get name */
extern const char*							ShaderParameter_GetName( shaderParameter_t Object );

/* Get type */
extern shaderParameterType_t				ShaderParameter_GetType( shaderParameter_t Object );

/* Get value int */
extern int									ShaderParameter_GetValueInt( shaderParameter_t Object );

/* Get value float */
extern float								ShaderParameter_GetValueFloat( shaderParameter_t Object );

/* Get shader flag */
extern bool_t								ShaderParameter_GetValueShaderFlag( shaderParameter_t Object );

/* Get vec2f */
extern vec2f_t								ShaderParameter_GetValueVec2( shaderParameter_t Object );

/* Get vec3f */
extern vec3f_t								ShaderParameter_GetValueVec3( shaderParameter_t Object );

/* Get vec4f */
extern vec4f_t								ShaderParameter_GetValueVec4( shaderParameter_t Object );

/* Get mat4 */
extern mat4_t								ShaderParameter_GetValueMat4( shaderParameter_t Object );

/* Get texture */
extern texture_t							ShaderParameter_GetValueTexture( shaderParameter_t Object );

//---------------------------------------------------------------------//

#endif // !API_SHADERPARAMETER_H
