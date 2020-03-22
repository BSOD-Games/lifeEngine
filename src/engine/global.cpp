//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "global.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	ConsoleSystem*			g_consoleSystem = nullptr;
	Engine*					g_engine = nullptr;
	IStudioRender*			g_studioRender = nullptr;
	IWindow*				g_window = nullptr;
	InputSystem*			g_inputSystem = nullptr;
	ResourceSystem*			g_resourceSystem = nullptr;
	IPhysicsSystem*			g_physicsSystem = nullptr;

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//
