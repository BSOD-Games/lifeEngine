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

#include "engine/iconsolesystem.h"
#include "engine/iengine.h"
#include "engine/istudiorender.h"
#include "engine/iwindow.h"
#include "engine/iconcmd.h"
#include "engine/iconvar.h"

#include "enginefactory.h"
#include "concmd.h"
#include "convar.h"
#include "global.h"

// ------------------------------------------------------------------------------------ //
// Создать объект
// ------------------------------------------------------------------------------------ //
void* le::EngineFactory::Create( const char* NameInterface )
{
	if ( strcmp( NameInterface, ENGINE_INTERFACE_VERSION ) == 0 )					return g_engine;
	else if ( strcmp( NameInterface, CONSOLE_SYSTEM_INTERFACE_VERSION ) == 0 )		return g_consoleSystem;
	else if ( strcmp( NameInterface, STUDIO_RENDER_INTERFACE_VERSION ) == 0 )		return g_studioRender;
	else if ( strcmp( NameInterface, WINDOW_INTERFACE_VERSION ) == 0 )				return g_window;
	else if ( strcmp( NameInterface, CONCMD_INTERFACE_VERSION ) == 0 )				return new ConCmd();
	else if ( strcmp( NameInterface, CONVAR_INTERFACE_VERSION ) == 0 )				return new ConVar();

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Удалить объект
// ------------------------------------------------------------------------------------ //
void le::EngineFactory::Delete( const char* NameInterface, void* Object )
{
	if ( !Object ) return;

	if ( strcmp( NameInterface, CONCMD_INTERFACE_VERSION ) == 0 ||
		 strcmp( NameInterface, CONVAR_INTERFACE_VERSION ) == 0 )
		delete Object;
}