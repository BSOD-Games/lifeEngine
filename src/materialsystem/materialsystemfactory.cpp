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

#include "materialsystemfactory.h"
#include "materialvar.h"
#include "material.h"

// ------------------------------------------------------------------------------------ //
// Создать объект
// ------------------------------------------------------------------------------------ //
void* le::MaterialSystemFactory::Create( const char* NameInterface )
{
	if ( strcmp( NameInterface, MATERIAL_INTERFACE_VERSION ) == 0 )		return new Material();

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Удалить объект
// ------------------------------------------------------------------------------------ //
void le::MaterialSystemFactory::Delete( void* Object )
{
	if ( !Object ) return;
	delete Object;
}