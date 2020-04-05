//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_RESOURCESYSTEM_H
#define API_RESOURCESYSTEM_H

#include "../common/types.h"
#include "material.h"
#include "model.h"

//---------------------------------------------------------------------//

/* Load texture */
extern texture_t	ResourceSystem_LoadTexture( const char* Name, const char* Path );

/* Load material */
extern material_t	ResourceSystem_LoadMaterial( const char* Name, const char* Path );

/* Load mesh */
extern mesh_t		ResourceSystem_LoadMesh( const char* Name, const char* Path );

/* Load level */
//TODO: implement this

/* Load font */
extern font_t		ResourceSystem_LoadFont( const char* Name, const char* Path );

/* Load script */
//TODO: implement this

/* Unload texture */
extern void			ResourceSystem_UnloadTexture( const char* Name );

/* Unload material */
extern void			ResourceSystem_UnloadMaterial( const char* Name );

/* Unload mesh */
extern void			ResourceSystem_UnloadMesh( const char* Name );

/* Unload level */
//TODO: implement this

/* Unload font */
extern void			ResourceSystem_UnloadFont( const char* Name );

/* Unload script */
//TODO: implement this

/* Unload textures */
extern void			ResourceSystem_UnloadTextures();

/* Unload materials */
extern void			ResourceSystem_UnloadMaterials();

/* Unload meshes */
extern void			ResourceSystem_UnloadMeshes();

/* Unload levels */
//TODO: implement this

/* Unload fonts */
extern void			ResourceSystem_UnloadFonts();

/* Unload scripts */
//TODO: implement this

/* Unload all */
extern void			ResourceSystem_UnloadAll();

/* Get texture */
extern texture_t	ResourceSystem_GetTexture( const char* Name );

/* Get material */
extern material_t	ResourceSystem_GetMaterial( const char* Name );

/* Get mesh */
extern mesh_t		ResourceSystem_GetMesh( const char* Name );

/* Get level */
//TODO: implement this

/* Get font */
extern font_t		ResourceSystem_GetFont( const char* Name );

/* Get script */
//TODO: implement this

//---------------------------------------------------------------------//

#endif // API_RESOURCESYSTEM_H
