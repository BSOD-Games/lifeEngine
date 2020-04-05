//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_STUDIORENDERPASS_H
#define API_STUDIORENDERPASS_H

#include "../common/types.h"
#include "../engine/materialproxy.h"
#include "shaderparameter.h"

//---------------------------------------------------------------------//

typedef enum
{
	CT_FRONT,
	CT_BACK
} cullFaceType_t;

typedef void*			studioRenderPass_t;

//---------------------------------------------------------------------//

/* Delete */
extern void					StudioRenderPass_Delete( studioRenderPass_t Object );

/* Add parameter */
extern void					StudioRenderPass_AddParameter( studioRenderPass_t Object, shaderParameter_t Parameter );

/* Remove parameter */
extern void					StudioRenderPass_RemoveParameter( studioRenderPass_t Object, uint32_t Index );

/* Add proxy */
extern void					StudioRenderPass_AddProxy( studioRenderPass_t Object, materialProxy_t MaterialProxy );

/* Remove proxy */
extern void					StudioRenderPass_RemoveProxy( studioRenderPass_t Object, uint32_t Index );

/* Clear */
extern void					StudioRenderPass_Clear( studioRenderPass_t Object );

/* Enable depth test */
extern void					StudioRenderPass_EnableDepthTest( studioRenderPass_t Object, bool_t Enable );

/* Enable depth write */
extern void					StudioRenderPass_EnableDepthWrite( studioRenderPass_t Object, bool_t Enable);

/* Enable blend */
extern void					StudioRenderPass_EnableBlend( studioRenderPass_t Object, bool_t Enable );

/* Enable cull face */
extern void					StudioRenderPass_EnableCullFace( studioRenderPass_t Object, bool_t Enable );

/* Find parameter */
extern shaderParameter_t	StudioRenderPass_FindParameter( studioRenderPass_t Object, const char* Name );

/* Set shader */
extern void					StudioRenderPass_SetShader( studioRenderPass_t Object, const char* NameShader );

/* Set cull face type */
extern void					StudioRenderPass_SetCullFaceType( studioRenderPass_t Object, cullFaceType_t CullFaceType );

/* Is depth test */
extern bool_t				StudioRenderPass_IsDepthTest( studioRenderPass_t Object );

/* Is depth write */
extern bool_t				StudioRenderPass_IsDepthWrite( studioRenderPass_t Object );

/* Is blend */
extern bool_t				StudioRenderPass_IsBlend( studioRenderPass_t Object );

/* Is cull face */
extern bool_t				StudioRenderPass_IsCullFace( studioRenderPass_t Object );

/* Get cull face type */
extern cullFaceType_t		StudioRenderPass_GetCullFaceType( studioRenderPass_t Object );

/* Get shader name */
extern const char*			StudioRenderPass_GetNameShader( studioRenderPass_t Object );

/* Get count parameter */
extern uint32_t				StudioRenderPass_GetCountParameters( studioRenderPass_t Object );

/* Get parameters */
extern shaderParameter_t*	StudioRenderPass_GetParameters( studioRenderPass_t Object );

/* Get parameter */
extern shaderParameter_t	StudioRenderPass_GetParameter( studioRenderPass_t Object, uint32_t Index );

/* Get count proxes */
extern uint32_t				StudioRenderPass_GetCountProxes( studioRenderPass_t Object );

/* Get proxes */
extern materialProxy_t*		StudioRenderPass_GetProxes( studioRenderPass_t Object );

/* Get proxy */
extern materialProxy_t		StudioRenderPass_GetProxy( studioRenderPass_t Object, uint32_t Index );

//---------------------------------------------------------------------//

#endif // !API_STUDIORENDERPASS_H
