// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "Misc/EngineDefines.h"
#include "Misc/Path.h"
#include "Misc/Object.h"
#include "System/Window.h"
#include "System/IGame.h"
#include "Rendering/RHI/IRHI.h"

namespace le
{
	class Engine : public Object
	{
	public:
		/* Constructor */
		Engine();

		/* Destructor */
		~Engine();

		/* Initialize engine */
		bool Initialize( const std::string& InWindowTitle, uint32 InWindowWidth, uint32 InWindowHeight, const Path& InLogPath, const Path& InRootPath = "./" );
		bool Initialize( FWindowHandle InWindowHandle, const Path& InLogPath, const Path& InRootPath = "./" );
	
		/* Start game */
		FORCEINLINE void StartGame( IGame* InGame )			
		{ 
			LIFEENGINE_ASSERT( InGame );								
			game = InGame;
		}

		/* Stop simulation */
		void StopGame();

		/* Tick game */
		void Tick();

		/* Render frame */
		void RenderFrame();

		/* Is played game */
		FORCEINLINE bool IsPlayedGame() const					{ return game; }

		/* Get main RHIContext */
		FORCEINLINE FRHIContext GetMainRHIContext() const		{ return mainRHIContext; }

		/* Get game */
		FORCEINLINE IGame* GetGame() const						{ return game; }

	private:
		float           currentTick;
		float           lastTick;
		float           delayFrame;
		float			deltaTime;
		
		FRHIContext		mainRHIContext;
		IGame*			game;
	};
}

#endif // !ENGINE_