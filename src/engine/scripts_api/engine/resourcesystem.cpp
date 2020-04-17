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

namespace scripts_api
{
	#include "resourcesystem.h"
}

// ------------------------------------------------------------------------------------ //
// Load texture
// ------------------------------------------------------------------------------------ //
scripts_api::texture_t scripts_api::ResourceSystem_LoadTexture( const char* Name, const char* Path )
{
	return le::g_resourceSystem->LoadTexture( Name, Path );
}

// ------------------------------------------------------------------------------------ //
// Load material
// ------------------------------------------------------------------------------------ //
scripts_api::material_t scripts_api::ResourceSystem_LoadMaterial( const char* Name, const char* Path )
{
	return le::g_resourceSystem->LoadMaterial( Name, Path );
}

// ------------------------------------------------------------------------------------ //
// Load mesh
// ------------------------------------------------------------------------------------ //
scripts_api::mesh_t scripts_api::ResourceSystem_LoadMesh( const char* Name, const char* Path )
{
	return le::g_resourceSystem->LoadMesh( Name, Path );
}

// ------------------------------------------------------------------------------------ //
// Load font
// ------------------------------------------------------------------------------------ //
scripts_api::font_t scripts_api::ResourceSystem_LoadFont( const char* Name, const char* Path )
{
	return le::g_resourceSystem->LoadFont( Name, Path );
}

// ------------------------------------------------------------------------------------ //
// Unload texture
// ------------------------------------------------------------------------------------ //
void scripts_api::ResourceSystem_UnloadTexture( const char* Name )
{
	le::g_resourceSystem->UnloadTexture( Name );
}

// ------------------------------------------------------------------------------------ //
// Unload material
// ------------------------------------------------------------------------------------ //
void scripts_api::ResourceSystem_UnloadMaterial( const char* Name )
{
	le::g_resourceSystem->UnloadMaterial( Name );
}

// ------------------------------------------------------------------------------------ //
// Unload mesh
// ------------------------------------------------------------------------------------ //
void scripts_api::ResourceSystem_UnloadMesh( const char* Name )
{
	le::g_resourceSystem->UnloadMesh( Name );
}

// ------------------------------------------------------------------------------------ //
// Unload font
// ------------------------------------------------------------------------------------ //
void scripts_api::ResourceSystem_UnloadFont( const char* Name )
{
	le::g_resourceSystem->UnloadFont( Name );
}

// ------------------------------------------------------------------------------------ //
// Unload textures
// ------------------------------------------------------------------------------------ //
void scripts_api::ResourceSystem_UnloadTextures()
{
	le::g_resourceSystem->UnloadTextures();
}

// ------------------------------------------------------------------------------------ //
// Unload materials
// ------------------------------------------------------------------------------------ //
void scripts_api::ResourceSystem_UnloadMaterials()
{
	le::g_resourceSystem->UnloadMaterials();
}

// ------------------------------------------------------------------------------------ //
// Unload meshes
// ------------------------------------------------------------------------------------ //
void scripts_api::ResourceSystem_UnloadMeshes()
{
	le::g_resourceSystem->UnloadMeshes();
}

// ------------------------------------------------------------------------------------ //
// Unload fonts
// ------------------------------------------------------------------------------------ //
void scripts_api::ResourceSystem_UnloadFonts()
{
	le::g_resourceSystem->UnloadFonts();
}

// ------------------------------------------------------------------------------------ //
// Unload all
// ------------------------------------------------------------------------------------ //
void scripts_api::ResourceSystem_UnloadAll()
{
	le::g_resourceSystem->UnloadAll();
}

// ------------------------------------------------------------------------------------ //
// Get texture
// ------------------------------------------------------------------------------------ //
scripts_api::texture_t scripts_api::ResourceSystem_GetTexture( const char* Name )
{
	return le::g_resourceSystem->GetTexture( Name );
}

// ------------------------------------------------------------------------------------ //
// Get material
// ------------------------------------------------------------------------------------ //
scripts_api::material_t scripts_api::ResourceSystem_GetMaterial( const char* Name )
{
	return le::g_resourceSystem->GetMaterial( Name );
}

// ------------------------------------------------------------------------------------ //
// Get mesh
// ------------------------------------------------------------------------------------ //
scripts_api::mesh_t scripts_api::ResourceSystem_GetMesh( const char* Name )
{
	return le::g_resourceSystem->GetMesh( Name );
}

// ------------------------------------------------------------------------------------ //
// Get font
// ------------------------------------------------------------------------------------ //
scripts_api::font_t scripts_api::ResourceSystem_GetFont( const char* Name )
{
	return le::g_resourceSystem->GetFont( Name );
}

// ------------------------------------------------------------------------------------ //
// Load physics model
// ------------------------------------------------------------------------------------ //
scripts_api::physicsModel_t scripts_api::ResourceSystem_LoadPhysicsModel( const char* Name, const char* Path )
{
	return le::g_resourceSystem->LoadPhysicsModel( Name, Path );
}

// ------------------------------------------------------------------------------------ //
// Unload physics model
// ------------------------------------------------------------------------------------ //
void scripts_api::ResourceSystem_UnloadPhysicsModel( const char* Name )
{
	return le::g_resourceSystem->UnloadPhysicsModel( Name );
}

// ------------------------------------------------------------------------------------ //
// Unload physics models
// ------------------------------------------------------------------------------------ //
void scripts_api::ResourceSystem_UnloadPhysicsModels()
{
	return le::g_resourceSystem->UnloadPhysicsModels();
}

// ------------------------------------------------------------------------------------ //
// Get physics model
// ------------------------------------------------------------------------------------ //
scripts_api::physicsModel_t scripts_api::ResourceSystem_GetPhysicsModel( const char* Name )
{
	return le::g_resourceSystem->GetPhysicsModel( Name );
}
