//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "enginefactory.h"
#include "camera.h"
#include "model.h"
#include "material.h"
#include "sprite.h"

// ------------------------------------------------------------------------------------ //
// Создать объект
// ------------------------------------------------------------------------------------ //
void* le::EngineFactory::Create( const char* NameInterface )
{
	if ( strcmp( NameInterface, CAMERA_INTERFACE_VERSION ) == 0 )				return new Camera();
	else if ( strcmp( NameInterface, MODEL_INTERFACE_VERSION ) == 0 )			return new Model();
	else if ( strcmp( NameInterface, MATERIAL_INTERFACE_VERSION ) == 0 )		return new Material();
	else if ( strcmp( NameInterface, SPRITE_INTERFACE_VERSION ) == 0 )			return new Sprite();

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Удалить объект
// ------------------------------------------------------------------------------------ //
void le::EngineFactory::Delete( void* Object )
{
	if ( !Object ) return;
	delete Object;
}