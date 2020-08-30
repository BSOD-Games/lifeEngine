// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IRHI_H
#define IRHI_H

#include "Misc/Types.h"
#include "System/Window.h"

namespace le
{
	typedef void* FRHIContext;

	class IRHI
	{
	public:
		/* Destructor */
		virtual ~IRHI() {}

		/* Create render context */
		virtual FRHIContext CreateContext( FWindowHandle InWindowHandle, FRHIContext InShareContext = nullptr ) = 0;

		/* Make current context */
		virtual bool MakeCurrentContext( FRHIContext InRHIContext ) = 0;

		/* Delete context */
		virtual void DeleteContext( FRHIContext InRHIContext ) = 0;

		/* Swap buffers */
		virtual void SwapBuffers( FRHIContext InRHIContext ) = 0;

		/* Set vertical sync */
		virtual void SetVerticalSync( bool InIsEnable = true ) = 0;
	};
}

#endif // !IRHI_H
