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

#include "consolesystemfactory.h"
#include "concmd.h"
#include "convar.h"

// ------------------------------------------------------------------------------------ //
// Создать объект
// ------------------------------------------------------------------------------------ //
void* le::ConsoleSystemFactory::Create( const char* NameInterface )
{
	if ( strcmp( NameInterface, CONCMD_INTERFACE_VERSION ) == 0 )				return new ConCmd();
	else if ( strcmp( NameInterface, CONVAR_INTERFACE_VERSION ) == 0 )			return new ConVar();

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Удалить объект
// ------------------------------------------------------------------------------------ //
void le::ConsoleSystemFactory::Delete( const char* NameInterface, void* Object )
{
	if ( !Object ) return;
	delete Object;
}