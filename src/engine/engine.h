//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
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
		bool							LoadStudioRender( const char* PathDLL );
		void							UnloadStudioRender();

		bool							isInit;
		bool							isRunSimulation;

		IConCmd*						cmd_Exit;
		IConCmd*						cmd_Version;

		IStudioRenderInternal*			studioRender;
		StudioRenderDescriptor			studioRenderDescriptor;

		CriticalErrorCallbackFn_t		criticalError;
		ConsoleSystem					consoleSystem;
		Window							window;
		GameInfo						gameInfo;
		Configurations					configurations;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ENGINE_H
