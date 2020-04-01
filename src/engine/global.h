//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef GLOBAL_H
#define GLOBAL_H

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ConsoleSystem;
	extern ConsoleSystem*			g_consoleSystem;

	//---------------------------------------------------------------------//

	class Engine;
	extern Engine*					g_engine;

	//---------------------------------------------------------------------//

	class IStudioRender;
	extern IStudioRender*			g_studioRender;

	//---------------------------------------------------------------------//

	class IWindow;
	extern IWindow*					g_window;

	//---------------------------------------------------------------------//

	class InputSystem;
	extern InputSystem*				g_inputSystem;

	//---------------------------------------------------------------------//

	class ResourceSystem;
	extern ResourceSystem*			g_resourceSystem;

	//---------------------------------------------------------------------//

	class IPhysicsSystem;
	extern IPhysicsSystem*			g_physicsSystem;

	//---------------------------------------------------------------------//

	class ScriptSystem;
	extern ScriptSystem*			g_scriptSystem;

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !GLOBAL_H

