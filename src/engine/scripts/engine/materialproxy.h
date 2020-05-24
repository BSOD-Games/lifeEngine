//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_MATERIALPROXY_H
#define API_MATERIALPROXY_H

#include "../common/types.h"
#include "materialproxyvar.h"

//---------------------------------------------------------------------//

/* Create */
extern materialProxy_t			MaterialProxy_Create( const char* Name );

/* Delete */
extern void						MaterialProxy_Delete( materialProxy_t Object );

/* Update */
extern void						MaterialProxy_Update( materialProxy_t Object );

/* NeadUpdate */
extern void						MaterialProxy_NeadUpdate( materialProxy_t Object );

/* Clear var */
extern void						MaterialProxy_ClearVar( materialProxy_t Object, const char* Name );

/* Clear all vars */
extern void						MaterialProxy_ClearAllVars( materialProxy_t Object );

/* Set var */
extern void						MaterialProxy_SetVar( materialProxy_t Object, materialProxyVar_t MaterialProxyVar );

/* Is nead update */
extern bool_t					MaterialProxy_IsNeadUpdate( materialProxy_t Object );

/* Get name */
extern const char*				MaterialProxy_GetName( materialProxy_t Object );

/* Get var */
extern materialProxyVar_t		MaterialProxy_GetVar( materialProxy_t Object, const char* Name );

//---------------------------------------------------------------------//

#endif // API_MATERIALPROXY_H
