//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/ifactory.h"
#include "engine/global.h"
#include "engine/level.h"

namespace scripts_api
{
#include "level.h"
}

#define LEVEL( Object )  static_cast< le::Level* >( Object )

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void scripts_api::Level_Clear( level_t Object )
{
	if ( !Object ) return;
	LEVEL( Object )->Clear();
}


// ------------------------------------------------------------------------------------ //
// Add camera
// ------------------------------------------------------------------------------------ //
void scripts_api::Level_AddCamera( level_t Object, camera_t Camera )
{
	if ( !Object || !Camera ) return;
	LEVEL( Object )->AddCamera( ( le::Camera* ) Camera );
}


// ------------------------------------------------------------------------------------ //
// Remove camera by poiner
// ------------------------------------------------------------------------------------ //
void scripts_api::Level_RemoveCameraByPtr( level_t Object, camera_t Camera )
{
	if ( !Object || !Camera ) return;
	LEVEL( Object )->RemoveCamera( ( le::Camera* ) Camera );
}


// ------------------------------------------------------------------------------------ //
// Remove camera by index
// ------------------------------------------------------------------------------------ //
void scripts_api::Level_RemoveCameraByIndex(level_t Object, uint32_t Index)
{
	if ( !Object ) return;
	LEVEL( Object )->RemoveCamera( Index );
}


// ------------------------------------------------------------------------------------ //
// Is loaded level
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::Level_IsLoaded( level_t Object )
{
	if ( !Object ) return B_FALSE;
	return LEVEL( Object )->IsLoaded() ? B_TRUE : B_FALSE;
}


// ------------------------------------------------------------------------------------ //
// Get name format level
// ------------------------------------------------------------------------------------ //
const char* scripts_api::Level_GetNameFormat( level_t Object )
{
	if ( !Object ) return "unknown";
	return LEVEL( Object )->GetNameFormat();
}


// ------------------------------------------------------------------------------------ //
// Get count cameras
// ------------------------------------------------------------------------------------ //
scripts_api::uint32_t scripts_api::Level_GetCountCameras( level_t Object )
{
	if ( !Object ) return 0;
	return LEVEL( Object )->GetCountCameras();
}


// ------------------------------------------------------------------------------------ //
// Get camera
// ------------------------------------------------------------------------------------ //
scripts_api::camera_t scripts_api::Level_GetCamera( level_t Object, uint32_t Index )
{
	if ( !Object ) return nullptr;
	return LEVEL( Object )->GetCamera( Index );
}

// ------------------------------------------------------------------------------------ //
// Add entity
// ------------------------------------------------------------------------------------ //
void scripts_api::Level_AddEntity( level_t Object, void* Entity )
{
	if ( !Object || !Entity ) return;
	LEVEL( Object )->AddEntity( ( le::IEntity* ) Entity );
}

// ------------------------------------------------------------------------------------ //
// Remove entity by pointer
// ------------------------------------------------------------------------------------ //
void scripts_api::Level_RemoveEntityByPtr( level_t Object, void* Entity )
{
	if ( !Object || !Entity ) return;
	LEVEL( Object )->RemoveEntity( ( le::IEntity* ) Entity );
}

// ------------------------------------------------------------------------------------ //
// Remove entity by index
// ------------------------------------------------------------------------------------ //
void scripts_api::Level_RemoveEntityByIndex( level_t Object, uint32_t Index )
{
	if ( !Object ) return;
	LEVEL( Object )->RemoveEntity( Index );
}

// ------------------------------------------------------------------------------------ //
// Get count entityes
// ------------------------------------------------------------------------------------ //
scripts_api::uint32_t scripts_api::Level_GetCountEntityes( level_t Object )
{
	if ( !Object ) return 0;
	return LEVEL( Object )->GetCountEntityes();
}

// ------------------------------------------------------------------------------------ //
// Get entity
// ------------------------------------------------------------------------------------ //
void* scripts_api::Level_GetEntity( level_t Object, uint32_t Index )
{
	if ( !Object ) return nullptr;
	return LEVEL( Object )->GetEntity( Index );
}
