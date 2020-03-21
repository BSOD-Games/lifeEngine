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

// ------------------------------------------------------------------------------------ //
// Создать объект
// ------------------------------------------------------------------------------------ //
void* le::PhysicsSystemFactory::Create( const char* NameInterface )
{
    if ( strcmp( NameInterface, BODY_INTERFACE_VERSION ) == 0 )                           return new Body();
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
