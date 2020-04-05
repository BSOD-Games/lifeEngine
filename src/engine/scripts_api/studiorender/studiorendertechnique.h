//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_STUDIORENDERTECHNIQUE_H
#define API_STUDIORENDERTECHNIQUE_H

#include "../common/types.h"
#include "studiorenderpass.h"

//---------------------------------------------------------------------//

typedef enum
{
	RT_UNKNOW,
	RT_DEFFERED_SHADING
} renderTechniqueType_t;

typedef void*		studioRenderTechnique_t;

//---------------------------------------------------------------------//

/* Delete */
extern void							StudioRenderTechnique_Delete( studioRenderTechnique_t Object );

/* Add pass */
extern void							StudioRenderTechnique_AddPass( studioRenderTechnique_t Object, studioRenderPass_t Pass );

/* Remove pass */
extern void							StudioRenderTechnique_RemovePass( studioRenderTechnique_t Object, uint32_t Index );

/* Clear */
extern void							StudioRenderTechnique_Clear( studioRenderTechnique_t Object );

/* Set type */
extern void							StudioRenderTechnique_SetType( studioRenderTechnique_t Object, renderTechniqueType_t Technique );

/* Get type */
extern renderTechniqueType_t		StudioRenderTechnique_GetType( studioRenderTechnique_t Object );

/* Get count passes */
extern uint32_t						StudioRenderTechnique_GetCountPasses( studioRenderTechnique_t Object );

/* Get passes */
extern studioRenderPass_t*			StudioRenderTechnique_GetPasses( studioRenderTechnique_t Object );

/* Get pass */
extern studioRenderPass_t			StudioRenderTechnique_GetPass( studioRenderTechnique_t Object, uint32_t Index );

//---------------------------------------------------------------------//

#endif // !API_STUDIORENDERTECHNIQUE_H
