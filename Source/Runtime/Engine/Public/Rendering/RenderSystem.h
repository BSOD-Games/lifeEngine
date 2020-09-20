// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "System/Window.h"
#include "RHI/IRHI.h"
#include "Misc/Object.h"

namespace le
{
	class RenderSystem : public Object
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

		/* Create texture 2D */
		IRHITexture2D* CreateTexture2D( EImageFormat InImageFormat, uint32 InWidth, uint32 InHeight, uint32 InCountMipmap = 1 );

		/* Make current context */
		bool MakeCurrentContext( FRHIContext InRHIContext );

		/* Delete context */
		void DeleteContext( FRHIContext InRHIContext );

		/* Delete texture 2D */
		void DeleteTexture2D( IRHITexture2D*& InTexture2D );

		/* Start render scene */
		void Begin();

		/* End render scene */
		void End();

		/* Present scent */
		void Present();

		/* Set vertical sync */
		void SetVerticalSync( bool InIsEnable = true );

		/* Set viewport */
		void SetViewport( uint32 InX, uint32 InY, uint32 InWidth, uint32 InHeight );

	private:
		IRHI*				rhi;
		FRHIContext			currentContext;
	};
}

#endif // !RENDERSYSTEM_H
