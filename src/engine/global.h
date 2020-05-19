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

	class IFactory;
	extern IFactory*				g_engineFactory;

	//---------------------------------------------------------------------//

	class IStudioRender;
	extern IStudioRender*			g_studioRender;
	extern IFactory*				g_studioRenderFactory;

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
	extern IFactory*				g_physicsSystemFactory;

	//---------------------------------------------------------------------//

	class ScriptSystem;
	extern ScriptSystem*			g_scriptSystem;

	//---------------------------------------------------------------------//

	class MaterialSystem;
	extern MaterialSystem*			g_materialSystem;

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !GLOBAL_H

