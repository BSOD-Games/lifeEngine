//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_LEVEL_H
#define API_LEVEL_H

#include "../common/types.h"
#include "camera.h"

//---------------------------------------------------------------------//

typedef void*				level_t;

//---------------------------------------------------------------------//

/* Clear */
extern void					Level_Clear( level_t Object );

/* Add camera */
extern void					Level_AddCamera( level_t Object, camera_t Camera );

/* Add entity */
extern void					Level_AddEntity( level_t Object, void* Entity );

/* Remove camera */
extern void					Level_RemoveCameraByPtr( level_t Object, camera_t Camera );

/* Remove camera */
extern void					Level_RemoveCameraByIndex( level_t Object, uint32_t Index );

/* Remove entity */
extern void					Level_RemoveEntityByPtr( level_t Object, void* Entity );

/* Remove entity */
extern void					Level_RemoveEntityByIndex( level_t Object, uint32_t Index );

/* Is loaded */
extern bool_t				Level_IsLoaded( level_t Object );

/* Get name format */
extern const char*			Level_GetNameFormat( level_t Object );

/* Get count cameras */
extern uint32_t				Level_GetCountCameras( level_t Object );

/* Get camera */
extern camera_t				Level_GetCamera( level_t Object, uint32_t Index );

/* Get count entityes */
extern uint32_t				Level_GetCountEntityes( level_t Object );

/* Get entity */
extern void*				Level_GetEntity( level_t Object, uint32_t Index );

//---------------------------------------------------------------------//

#endif // API_LEVEL_H
