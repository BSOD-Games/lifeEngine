//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "physicssystemfactory.h"
#include "body.h"
#include "charactercontroller.h"
#include "collider.h"
#include "physicsmodel.h"

// ------------------------------------------------------------------------------------ //
// Создать объект
// ------------------------------------------------------------------------------------ //
void* le::PhysicsSystemFactory::Create( const char* NameInterface )
{
	if ( strcmp( NameInterface, BODY_INTERFACE_VERSION ) == 0 )								return new Body();
	else if ( strcmp( NameInterface, CHARCTERCONTROLLER_INTERFACE_VERSION ) == 0 )			return new CharcterController();
	else if ( strcmp( NameInterface, COLLIDER_INTERFACE_VERSION ) == 0 )					return new Collider();
	else if ( strcmp( NameInterface, PHYSICSMODEL_INTERFACE_VERSION ) == 0 )				return new PhysicsModel();
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Удалить объект
// ------------------------------------------------------------------------------------ //
void le::PhysicsSystemFactory::Delete( void* Object )
{
    if ( !Object ) return;
    delete Object;
}
