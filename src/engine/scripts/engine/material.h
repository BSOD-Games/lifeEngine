//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_MATERIAL_H
#define API_MATERIAL_H

#include "../common/types.h"
#include "materialproxy.h"
#include "shaderparameter.h"

//---------------------------------------------------------------------//

typedef enum
{
	CT_FRONT,
	CT_BACK
} cullFaceType_t;

//---------------------------------------------------------------------//

/* Create */
extern material_t						Material_Create();

/* Delete */
extern void								Material_Delete( material_t Object );

/* Clear */
extern void								Material_Clear( material_t Object );

/* Add parameter */
extern void								Material_AddParameter( material_t Object, shaderParameter_t Parameter );

/* Remove parameter */
extern void								Material_RemoveParameter( material_t Object, uint32_t Index );

/* Add proxy */
extern void								Material_AddProxy( material_t Object, materialProxy_t MaterialProxy );

/* Remove proxy */
extern void								Material_RemoveProxy( material_t Object, uint32_t Index );

/* Enable depth test */
extern void								Material_EnableDepthTest( material_t Object, bool_t Enable );

/* Enable depth write */
extern void								Material_EnableDepthWrite( material_t Object, bool_t Enable );

/* Enable blend */
extern void								Material_EnableBlend( material_t Object, bool_t Enable );

/* Enable cull face */
extern void								Material_EnableCullFace( material_t Object, bool_t Enable );

/* Find parameter */
extern shaderParameter_t				Material_FindParameter( material_t Object, const char* Name );

/* Set shader */
extern void								Material_SetShader( material_t Object, const char* NameShader );

/* Set cull face type */
extern void								Material_SetCullFaceType( material_t Object, cullFaceType_t CullFaceType );

/* Is depth test */
extern bool_t							Material_IsDepthTest( material_t Object );

/* Is depth write */
extern bool_t							Material_IsDepthWrite( material_t Object );

/* Is blend */
extern bool_t							Material_IsBlend( material_t Object );

/* Is cull face */
extern bool_t							Material_IsCullFace( material_t Object );

/* Get cull face type */
extern cullFaceType_t					Material_GetCullFaceType( material_t Object );

/* Get count parameter */
extern uint32_t							Material_GetCountParameters( material_t Object );

/* Get parameters */
extern shaderParameter_t*				Material_GetParameters( material_t Object );

/* Get parameter */
extern shaderParameter_t				Material_GetParameter( material_t Object, uint32_t Index );

/* Get count proxes */
extern uint32_t							Material_GetCountProxes( material_t Object );

/* Get proxes */
extern materialProxy_t*					Material_GetProxes( material_t Object );

/* Get proxy */
extern materialProxy_t					Material_GetProxy( material_t Object, uint32_t Index );

/* Set surface name */
extern void								Material_SetSurfaceName( material_t Object, const char* Name );

/* Get surface name */
extern const char*						Material_GetSurfaceName( material_t Object );

//---------------------------------------------------------------------//

#endif // API_MATERIAL_H
