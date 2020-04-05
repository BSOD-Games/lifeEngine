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
#include "../studiorender/studiorendertechnique.h"

//---------------------------------------------------------------------//

typedef void*			material_t;

//---------------------------------------------------------------------//

/* Delete */
extern void								Material_Delete( material_t Object );

/* Add technique */
extern void								Material_AddTechnique( material_t Object, studioRenderTechnique_t Tecnhique );

/* Remove technique */
extern void								Material_RemoveTechnique( material_t Object, uint32_t Index );

/* Clear */
extern void								Material_Clear( material_t Object );

/* Set surface name */
extern void								Material_SetSurfaceName( material_t Object, const char* Name );

/* Get surface name */
extern const char*						Material_GetSurfaceName( material_t Object );

/* Get count techiques */
extern uint32_t							Material_GetCountTechniques( material_t Object );

/* Get techniques */
extern studioRenderTechnique_t*			Material_GetTechniques( material_t Object );

/* Get technique */
extern studioRenderTechnique_t			Material_GetTechniqueByIndex( material_t Object, uint32_t Index );

/* Get technique */
extern studioRenderTechnique_t			Material_GetTechniqueByType( material_t Object, renderTechniqueType_t Type );

//---------------------------------------------------------------------//

#endif // API_MATERIAL_H
