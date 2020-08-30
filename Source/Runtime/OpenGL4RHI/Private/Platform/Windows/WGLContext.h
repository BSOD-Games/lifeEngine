// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef WGLCONTEXT_H
#define WGLCONTEXT_H

#include "Rendering/RHI/IRHI.h"
#include "SettingsContext.h"

namespace le
{
	/* Create context */
	FRHIContext	WinGL_CreateContext( FWindowHandle InWindowHandle, const SSettingsContext& InSettingsContext, FRHIContext InShareContext = nullptr );

	/* Make current context */
	bool WinGL_MakeCurrentContext( FRHIContext InRHIContext );

	/* Delete context */
	void WinGL_DeleteContext( FRHIContext InRHIContext );

	/* Swap buffers */
	void WinGL_SwapBuffers( FRHIContext InRHIContext );

	/* Set vertical sync */
	void WinGL_SetVerticalSync( bool InIsEnable = true );
}

#endif // !WGLCONTEXT_H
