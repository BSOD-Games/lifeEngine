// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "OpenGL4RHI.h"

// Windows Context
#ifdef PLATFORM_WINDOWS
	#include "Platform/Windows/WGLContext.h"
#else
	#error Unsupported platform
#endif

/**
 * Constructor
 */
le::OpenGL4RHI::OpenGL4RHI()
{}

/**
 * Destructor
 */
le::OpenGL4RHI::~OpenGL4RHI()
{}

/**
 * Create render context
 */
le::FRHIContext le::OpenGL4RHI::CreateContext( FWindowHandle InWindowHandle, FRHIContext InShareContext )
{
	SSettingsContext		settingsContext;
	settingsContext.majorVersion = 4;
	settingsContext.minorVersion = 5;
	settingsContext.depthBits = 24;
	settingsContext.stencilBits = 8;

#ifdef LIFEENGINE_DEBUG
	settingsContext.attributeFlags = SSettingsContext::CA_Core | SSettingsContext::CA_Debug;
#else
	settingsContext.attributeFlags = SSettingsContext::CA_Core;
#endif // LIFEENGINE_DEBUG

#ifdef PLATFORM_WINDOWS
	return WinGL_CreateContext( InWindowHandle, settingsContext, InShareContext );
#else
	return nullptr;
#endif // PLATFORM_WINDOWS	
}

/**
 * Make current context
 */
bool le::OpenGL4RHI::MakeCurrentContext( FRHIContext InRHIContext )
{
#ifdef PLATFORM_WINDOWS
	return WinGL_MakeCurrentContext( InRHIContext );
#else
	return false;
#endif // PLATFORM_WINDOWS	
}

/**
 * Delete context
 */
void le::OpenGL4RHI::DeleteContext( FRHIContext InRHIContext )
{
#ifdef PLATFORM_WINDOWS
	return WinGL_DeleteContext( InRHIContext );
#endif // PLATFORM_WINDOWS	
}

/**
 * Swap buffers
 */
void le::OpenGL4RHI::SwapBuffers( FRHIContext InRHIContext )
{
#ifdef PLATFORM_WINDOWS
	return WinGL_SwapBuffers( InRHIContext );
#endif // PLATFORM_WINDOWS	
}

/**
 * Set vertical sync
 */
void le::OpenGL4RHI::SetVerticalSync( bool InIsEnable )
{
#ifdef PLATFORM_WINDOWS
	return WinGL_SetVerticalSync( InIsEnable );
#endif // PLATFORM_WINDOWS	
}