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
#include "concmd.h"
#include "convar.h"

// ------------------------------------------------------------------------------------ //
// Создать объект
// ------------------------------------------------------------------------------------ //
void* le::EngineFactory::Create( const char* NameInterface )
{
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