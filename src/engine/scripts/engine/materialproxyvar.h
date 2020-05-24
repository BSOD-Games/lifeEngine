//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_MATERIALPROXYVAR_H
#define API_MATERIALPROXYVAR_H

#include "../common/types.h"
#include "../mathlib/mathlib.h"
#include "shaderparameter.h"

//---------------------------------------------------------------------//

typedef enum
{
	MPVT_NONE,
	MPVT_FLOAT,
	MPVT_INT,
	MPVT_BOOL,
	MPVT_VECTOR_2D,
	MPVT_VECTOR_3D,
	MPVT_VECTOR_4D,
	MPVT_ARRAY_FLOAT,
	MPVT_ARRAY_INT,
	MPVT_ARRAY_VECTOR_2D,
	MPVT_ARRAY_VECTOR_3D,
	MPVT_ARRAY_VECTOR_4D,
	MPVT_SHADER_PARAMETER
} materialProxyVarType_t;

//---------------------------------------------------------------------//

/* Create */
extern materialProxyVar_t			MaterialProxyVar_Create();

/* Delete */
extern void							MaterialProxyVar_Delete( materialProxyVar_t Object );

/* Clear */
extern void							MaterialProxyVar_Clear( materialProxyVar_t Object );

/* Set name */
extern void							MaterialProxyVar_SetName( materialProxyVar_t Object, const char* Name );

/* Set value int */
extern void							MaterialProxyVar_SetValueInt( materialProxyVar_t Object, int Value );

/* Set value float */
extern void							MaterialProxyVar_SetValueFloat( materialProxyVar_t Object, float Value );

/* Set value bool */
extern void							MaterialProxyVar_SetValueBool( materialProxyVar_t Object, bool_t Value );

/* Set value vec2 */
extern void							MaterialProxyVar_SetValueVec2( materialProxyVar_t Object, vec2f_t Value );

/* Set value vec3 */
extern void							MaterialProxyVar_SetValueVec3( materialProxyVar_t Object, vec3f_t Value );

/* Set value vec4 */
extern void							MaterialProxyVar_SetValueVec4( materialProxyVar_t Object, vec4f_t Value );

/* Set value shader parameter */
extern void							MaterialProxyVar_SetValueShaderParameter( materialProxyVar_t Object, shaderParameter_t Value );

/* Set value array float */
extern void							MaterialProxyVar_SetValueArrayFloat( materialProxyVar_t Object, float* Array, uint32_t Count );

/* Set value array int */
extern void							MaterialProxyVar_SetValueArrayInt( materialProxyVar_t Object, int* Array, uint32_t Count );

/* Set value array vec2 */
extern void							MaterialProxyVar_SetValueArrayVec2( materialProxyVar_t Object, vec2f_t* Array, uint32_t Count );

/* Set value array vec3 */
extern void							MaterialProxyVar_SetValueArrayVec3( materialProxyVar_t Object, vec3f_t* Array, uint32_t Count );

/* Set value array vec4 */
extern void							MaterialProxyVar_SetValueArrayVec4( materialProxyVar_t Object, vec4f_t* Array, uint32_t Count );

/* Is defined */
extern bool_t						MaterialProxyVar_IsDefined( materialProxyVar_t Object );

/* Get name */
extern const char*					MaterialProxyVar_GetName( materialProxyVar_t Object );

/* Get type */
extern materialProxyVarType_t		MaterialProxyVar_GetType( materialProxyVar_t Object );

/* Get value int */
extern int							MaterialProxyVar_GetValueInt( materialProxyVar_t Object );

/* Get value float */
extern float						MaterialProxyVar_GetValueFloat( materialProxyVar_t Object );

/* Get value bool */
extern bool_t						MaterialProxyVar_GetValueBool( materialProxyVar_t Object );

/* Get value vec2f */
extern vec2f_t						MaterialProxyVar_GetValueVec2( materialProxyVar_t Object );

/* Get value vec3f */
extern vec3f_t						MaterialProxyVar_GetValueVec3( materialProxyVar_t Object );

/* Get value vec4f */
extern vec4f_t						MaterialProxyVar_GetValueVec4( materialProxyVar_t Object );

/* Get value shader parameter */
extern shaderParameter_t			MaterialProxyVar_GetValueShaderParameter( materialProxyVar_t Object );

/* Get value array float */
extern float*						MaterialProxyVar_GetValueArrayFloat( materialProxyVar_t Object, uint32_t* Count );

/* Get value array int */
extern int*							MaterialProxyVar_GetValueArrayInt( materialProxyVar_t Object, uint32_t* Count );

/* Get value array vec2f */
extern vec2f_t*						MaterialProxyVar_GetValueArrayVec2( materialProxyVar_t Object, uint32_t* Count );

/* Get value array vec3f */
extern vec3f_t*						MaterialProxyVar_GetValueArrayVec3( materialProxyVar_t Object, uint32_t* Count );

/* Get value array vec4f */
extern vec4f_t*						MaterialProxyVar_GetValueArrayVec4( materialProxyVar_t Object, uint32_t* Count );

//---------------------------------------------------------------------//

#endif // API_MATERIALPROXYVAR_H
