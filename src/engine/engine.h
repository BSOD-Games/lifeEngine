//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ENGINE_H
#define ENGINE_H

#include "common/configurations.h"
#include "common/version.h"
#include "common/gameinfo.h"
#include "engine/lifeengine.h"
#include "engine/iengineinternal.h"
#include "engine/consolesystem.h"
#include "engine/resourcesystem.h"
#include "engine/window.h"
#include "engine/enginefactory.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IStudioRenderInternal;
	class IMaterialSystemInternal;
	class IConCmd;

	//---------------------------------------------------------------------//

	struct StudioRenderDescriptor
	{
		void*								handle;
		LE_CreateStudioRenderFn_t			LE_CreateStudioRender;
		LE_DeleteStudioRenderFn_t			LE_DeleteStudioRender;
		LE_SetCriticalErrorFn_t				LE_SetCriticalError;
	};

	//---------------------------------------------------------------------//

	struct MaterialSystemDescriptor
	{
		void*								handle;
		LE_CreateMaterialSystemFn_t			LE_CreateMaterialSystem;
		LE_DeleteMaterialSystemFn_t			LE_DeleteMaterialSystem;
		LE_SetCriticalErrorFn_t				LE_SetCriticalError;
	};

	//---------------------------------------------------------------------//

	struct GameDescriptor
	{
		void*								handle;
		LE_CreateGameFn_t					LE_CreateGame;
		LE_DeleteGameFn_t					LE_DeleteGame;
		LE_SetCriticalErrorFn_t				LE_SetCriticalError;
	};

	//---------------------------------------------------------------------//

	class Engine : public IEngineInternal
	{
	public:
		// IEngine
		virtual bool					LoadConfig( const char* FilePath );
		virtual bool					SaveConfig( const char* FilePath );		
		virtual void					RunSimulation();
		virtual void					StopSimulation();		

		virtual void					SetConfig( const Configurations& Configurations );

		virtual bool					IsRunSimulation() const;
		virtual IConsoleSystem*			GetConsoleSystem() const;
		virtual IStudioRender*			GetStudioRender() const;
		virtual IMaterialSystem*		GetMaterialSystem() const;
		virtual IResourceSystem*		GetResourceSystem() const;
		virtual IWindow*				GetWindow() const;
		virtual IFactory*				GetFactory() const;
		virtual const Configurations&	GetConfigurations() const;
		virtual const Version&			GetVersion() const;

		// IEngineInternal
		virtual bool					Initialize( WindowHandle_t WindowHandle = nullptr );
		virtual bool					LoadGame( const char* DirGame );
		virtual void					UnloadGame();

		// Engine
		Engine();
		~Engine();

	private:
		bool							LoadModule_StudioRender( const char* PathDLL );
		void							UnloadModule_StudioRender();
		bool							LoadModule_MaterialSystem( const char* PathDLL );
		void							UnloadModule_MaterialSystem();
		bool							LoadGameInfo( const char* GameDir );
		bool							LoadModule_Game( const char* PathDLL );
		void							UnloadModule_Game();

		bool							isInit;
		bool							isRunSimulation;

		IConCmd*						cmd_Exit;
		IConCmd*						cmd_Version;

		IStudioRenderInternal*			studioRender;
		StudioRenderDescriptor			studioRenderDescriptor;

		IMaterialSystemInternal*		materialSystem;
		MaterialSystemDescriptor		materialSystemDescriptor;

		IGame*							game;
		GameDescriptor					gameDescriptor;

		CriticalErrorCallbackFn_t		criticalError;
		ConsoleSystem					consoleSystem;
		ResourceSystem					resourceSystem;
		Window							window;
		EngineFactory					engineFactory;
		GameInfo						gameInfo;
		Configurations					configurations;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ENGINE_H
