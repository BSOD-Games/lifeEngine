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
#include "engine/iengineinternal.h"

#include "engine/consolesystem.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Engine : public IEngineInternal
	{
	public:
		// IEngine
		virtual bool				LoadConfig( const char* FilePath );
		virtual bool				SaveConfig( const char* FilePath );
		
		virtual void				RunSimulation();
		virtual void				StopSimulation();
		virtual void				SetConfig( const Configurations& Configurations );
		
		virtual bool				IsRunSimulation() const;
		virtual IConsoleSystem*		GetConsoleSystem() const;
		
		// IEngineInternal
		virtual bool				Initialize( WindowHandle_t WindowHandle = nullptr );
		virtual bool				LoadGame( const char* DirGame );
		virtual void				UnloadGame();

		// Engine
		Engine();
		~Engine();

	private:
		bool				isRunSimulation;

		ConsoleSystem		consoleSystem;
		GameInfo			gameInfo;
		Configurations		configurations;
		Version				version;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ENGINE_H
