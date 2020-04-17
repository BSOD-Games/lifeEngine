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

#include "scriptsystemfactory.h"
#include "script.h"

// ------------------------------------------------------------------------------------ //
// Создать объект
// ------------------------------------------------------------------------------------ //
void* le::ScriptSystemFactory::Create( const char* NameInterface )
{
	if ( strcmp( NameInterface, SCRIPT_INTERFACE_VERSION ) == 0 )    return new Script();

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Удалить объект
// ------------------------------------------------------------------------------------ //
void le::ScriptSystemFactory::Delete( void* Object )
{
	if ( !Object ) return;
	delete Object;
}
