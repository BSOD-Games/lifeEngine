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
#include "engine/window.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IStudioRenderInternal;

	//---------------------------------------------------------------------//

	struct StudioRenderDescriptor
	{
		void*								handle;
		LE_CreateStudioRenderFn_t			LE_CreateStudioRender;
		LE_DeleteStudioRenderFn_t			LE_DeleteStudioRender;
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
		virtual IWindow*				GetWindow() const;
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
		void							LoadStudioRender( const char* PathDLL );
		void							UnloadStudioRender();

		bool						isInit;
		bool						isRunSimulation;

		IStudioRenderInternal*		studioRender;
		StudioRenderDescriptor		studioRenderDescriptor;

		ConsoleSystem				consoleSystem;
		Window						window;
		GameInfo					gameInfo;
		Configurations				configurations;
		Version						version;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ENGINE_H
