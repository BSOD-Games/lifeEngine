// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHell
#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "Templates/Singleton.h"
#include "System/Window.h"
#include "RHI/IRHI.h"

namespace le
{
	class RenderSystem : public TSingleton<RenderSystem>
	{
	public:
		/* Constructor */
		RenderSystem();

		/* Destructor */
		~RenderSystem();

		/* Initialize render system */
		bool Initialize();

		/* Create render context */
		FRHIContext CreateContext( FWindowHandle InWindowHandle, FRHIContext InShareContext = nullptr );

		/* Make current context */
		bool MakeCurrentContext( FRHIContext InRHIContext );

		/* Delete context */
		void DeleteContext( FRHIContext InRHIContext );

		/* Start render scene */
		void Begin();

		/* End render scene */
		void End();

		/* Present scent */
		void Present();

		/* Set vertical sync */
		void SetVerticalSync( bool InIsEnable = true );

	private:
		IRHI*				rhi;
		FRHIContext			currentContext;
	};
}

#endif // !RENDERSYSTEM_H
