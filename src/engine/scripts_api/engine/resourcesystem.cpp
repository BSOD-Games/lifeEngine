//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/global.h"
#include "engine/resourcesystem.h"
#include "resourcesystem.h"

// ------------------------------------------------------------------------------------ //
// Load texture
// ------------------------------------------------------------------------------------ //
texture_t ResourceSystem_LoadTexture( const char* Name, const char* Path )
{
	return le::g_resourceSystem->LoadTexture( Name, Path );
}

// ------------------------------------------------------------------------------------ //
// Load material
// ------------------------------------------------------------------------------------ //
material_t ResourceSystem_LoadMaterial( const char* Name, const char* Path )
{
	return le::g_resourceSystem->LoadMaterial( Name, Path );
}

// ------------------------------------------------------------------------------------ //
// Load mesh
// ------------------------------------------------------------------------------------ //
mesh_t ResourceSystem_LoadMesh( const char* Name, const char* Path )
{
	return le::g_resourceSystem->LoadMesh( Name, Path );
}

// ------------------------------------------------------------------------------------ //
// Load font
// ------------------------------------------------------------------------------------ //
font_t ResourceSystem_LoadFont( const char* Name, const char* Path )
{
	return le::g_resourceSystem->LoadFont( Name, Path );
}

// ------------------------------------------------------------------------------------ //
// Unload texture
// ------------------------------------------------------------------------------------ //
void ResourceSystem_UnloadTexture( const char* Name )
{
	le::g_resourceSystem->UnloadTexture( Name );
}

// ------------------------------------------------------------------------------------ //
// Unload material
// ------------------------------------------------------------------------------------ //
void ResourceSystem_UnloadMaterial( const char* Name )
{
	le::g_resourceSystem->UnloadMaterial( Name );
}

// ------------------------------------------------------------------------------------ //
// Unload mesh
// ------------------------------------------------------------------------------------ //
void ResourceSystem_UnloadMesh( const char* Name )
{
	le::g_resourceSystem->UnloadMesh( Name );
}

// ------------------------------------------------------------------------------------ //
// Unload font
// ------------------------------------------------------------------------------------ //
void ResourceSystem_UnloadFont( const char* Name )
{
	le::g_resourceSystem->UnloadFont( Name );
}

// ------------------------------------------------------------------------------------ //
// Unload textures
// ------------------------------------------------------------------------------------ //
void ResourceSystem_UnloadTextures()
{
	le::g_resourceSystem->UnloadTextures();
}

// ------------------------------------------------------------------------------------ //
// Unload materials
// ------------------------------------------------------------------------------------ //
void ResourceSystem_UnloadMaterials()
{
	le::g_resourceSystem->UnloadMaterials();
}

// ------------------------------------------------------------------------------------ //
// Unload meshes
// ------------------------------------------------------------------------------------ //
void ResourceSystem_UnloadMeshes()
{
	le::g_resourceSystem->UnloadMeshes();
}

// ------------------------------------------------------------------------------------ //
// Unload fonts
// ------------------------------------------------------------------------------------ //
void ResourceSystem_UnloadFonts()
{
	le::g_resourceSystem->UnloadFonts();
}

// ------------------------------------------------------------------------------------ //
// Unload all
// ------------------------------------------------------------------------------------ //
void ResourceSystem_UnloadAll()
{
	le::g_resourceSystem->UnloadAll();
}

// ------------------------------------------------------------------------------------ //
// Get texture
// ------------------------------------------------------------------------------------ //
texture_t ResourceSystem_GetTexture( const char* Name )
{
	return le::g_resourceSystem->GetTexture( Name );
}

// ------------------------------------------------------------------------------------ //
// Get material
// ------------------------------------------------------------------------------------ //
material_t ResourceSystem_GetMaterial( const char* Name )
{
	return le::g_resourceSystem->GetMaterial( Name );
}

// ------------------------------------------------------------------------------------ //
// Get mesh
// ------------------------------------------------------------------------------------ //
mesh_t ResourceSystem_GetMesh( const char* Name )
{
	return le::g_resourceSystem->GetMesh( Name );
}

// ------------------------------------------------------------------------------------ //
// Get font
// ------------------------------------------------------------------------------------ //
font_t ResourceSystem_GetFont( const char* Name )
{
	return le::g_resourceSystem->GetFont( Name );
}
