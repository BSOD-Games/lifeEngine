// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include <Windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <vector>
#include <mutex>
#include <thread>
#include <string>
#include <exception>

#include "Logging/LogMacros.h"
#include "WGLContext.h"

struct SRHIContext
{
	/* Constructor */
	SRHIContext( HDC InDeviceContext, HGLRC InRenderContext ) :
		deviceContext( InDeviceContext ),
		renderContext( InRenderContext )
	{}

	HDC			deviceContext;
	HGLRC		renderContext;
};

SRHIContext*		GRHIContext = nullptr;

/**
 * Create context
 */
le::FRHIContext le::WinGL_CreateContext( FWindowHandle InWindowHandle, const SSettingsContext& InSettingsContext, FRHIContext InShareContext )
{
	PIXELFORMATDESCRIPTOR			pixelFormatDesc = {};
	uint32							colorBits = InSettingsContext.redBits + InSettingsContext.greenBits + InSettingsContext.blueBits + InSettingsContext.alphaBits;

	pixelFormatDesc.nSize = sizeof( PIXELFORMATDESCRIPTOR );
	pixelFormatDesc.nVersion = 1;
	pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDesc.iLayerType = PFD_MAIN_PLANE;
	pixelFormatDesc.cColorBits = static_cast< BYTE >( colorBits );
	pixelFormatDesc.cDepthBits = static_cast< BYTE >( InSettingsContext.depthBits );
	pixelFormatDesc.cStencilBits = static_cast< BYTE >( InSettingsContext.stencilBits );
	pixelFormatDesc.cAlphaBits = colorBits == 32 ? 8 : 0;

	HDC						deviceContext = GetDC( static_cast< HWND >( InWindowHandle ) );
	HGLRC					renderContext = nullptr;

	SRHIContext*			shareContext = InShareContext ? static_cast< SRHIContext* >( InShareContext ) : nullptr;
	int						pixelFormat = ChoosePixelFormat( deviceContext, &pixelFormatDesc );
	bool					isSetProfile = false;

	try
	{
		// Set pixel format and creating tempary render context
		if ( pixelFormat != 0 )
		{
			PIXELFORMATDESCRIPTOR			bestMatch_PixelFormatDescd;
			DescribePixelFormat( deviceContext, pixelFormat, sizeof( PIXELFORMATDESCRIPTOR ), &bestMatch_PixelFormatDescd );

			if ( bestMatch_PixelFormatDescd.cDepthBits < pixelFormatDesc.cDepthBits )
				LIFEENGINE_LOG_WARNING( "OpenGL4RHI", "Depth bits: %i not supported, seted to %i", pixelFormatDesc.cDepthBits, bestMatch_PixelFormatDescd.cDepthBits );

			if ( SetPixelFormat( deviceContext, pixelFormat, &pixelFormatDesc ) == FALSE )
				throw std::runtime_error( "Failed seted pixel fromat" );
		}
		else throw std::runtime_error( "Failed choose pixel format" );

		renderContext = wglCreateContext( deviceContext );
		if ( !renderContext )										throw std::runtime_error( "Creating temporary render context fail. Code error: " + std::to_string( GetLastError() ) );
		if ( !wglMakeCurrent( deviceContext, renderContext ) )		throw std::runtime_error( "Selecting temporary render context fail. Code error: " + std::to_string( GetLastError() ) );

		// Initialize GLEW
		glewExperimental = GL_TRUE;
		if ( glewInit() != GLEW_OK )			throw std::runtime_error( "OpenGL context is broken" );

		// Displaying video card parameters and supported extensions
		int			numberExtensions = 0;
		glGetIntegerv( GL_NUM_EXTENSIONS, &numberExtensions );

		LIFEENGINE_LOG_INFO( "OpenGL4RHI", "--- OpenGL info ---" );
		LIFEENGINE_LOG_INFO( "OpenGL4RHI", "  OpenGL version: %s", glGetString( GL_VERSION ) );
		LIFEENGINE_LOG_INFO( "OpenGL4RHI", "  OpenGL vendor: %s", glGetString( GL_VENDOR ) );
		LIFEENGINE_LOG_INFO( "OpenGL4RHI", "  OpenGL renderer: %s", glGetString( GL_RENDERER ) );
		LIFEENGINE_LOG_INFO( "OpenGL4RHI", "  OpenGL GLSL version: %s", glGetString( GL_SHADING_LANGUAGE_VERSION ) );
		LIFEENGINE_LOG_INFO( "OpenGL4RHI", "  OpenGL extensions: %s", glGetString( GL_EXTENSIONS ) );
		LIFEENGINE_LOG_INFO( "OpenGL4RHI", "--- OpenGL info end ---" );

		// Create extended context via wglCreateContextAttribsARB. If it is not supported,
		// leave the old context
		if ( wglewIsSupported( "WGL_ARB_create_context" ) == 1 )
		{
			std::vector<int>				attributes;

			// If the OpenGL version is greater than 1.1, then write the OpenGL version to the attributes
			if ( InSettingsContext.majorVersion > 1 || ( InSettingsContext.majorVersion == 1 && InSettingsContext.minorVersion > 1 ) )
			{
				attributes.push_back( WGL_CONTEXT_MAJOR_VERSION_ARB );
				attributes.push_back( InSettingsContext.majorVersion );
				attributes.push_back( WGL_CONTEXT_MINOR_VERSION_ARB );
				attributes.push_back( InSettingsContext.minorVersion );
			}

			// Choose a context profile (core, debug, etc)
			if ( wglewIsSupported( "WGL_ARB_create_context_profile" ) == 1 )
			{
				int				profile = ( InSettingsContext.attributeFlags & SSettingsContext::CA_Core ) ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
				int				debug = ( InSettingsContext.attributeFlags & SSettingsContext::CA_Debug ) ? WGL_CONTEXT_DEBUG_BIT_ARB : 0;

				attributes.push_back( WGL_CONTEXT_PROFILE_MASK_ARB );
				attributes.push_back( profile );
				attributes.push_back( WGL_CONTEXT_FLAGS_ARB );
				attributes.push_back( debug );

				isSetProfile = true;
			}
			else if ( ( InSettingsContext.attributeFlags & SSettingsContext::CA_Core ) || ( InSettingsContext.attributeFlags & SSettingsContext::CA_Debug ) )
				LIFEENGINE_LOG_WARNING( "OpenGL4RHI", "Selecting a profile during context creation is not supported, disabling comptibility and debug" );

			// Add an array terminator
			attributes.push_back( 0 );
			attributes.push_back( 0 );

			if ( shareContext )
			{
				static std::mutex		mutex;
				mutex.lock();

				if ( shareContext == GRHIContext )
				{
					if ( !wglMakeCurrent( shareContext->deviceContext, nullptr ) )
						throw std::runtime_error( "Failed to deactivate shared context before sharing. Code error: " + std::to_string( GetLastError() ) );

					GRHIContext = nullptr;
				}

				mutex.unlock();
			}

			HGLRC			oldContext = renderContext;
			renderContext = wglCreateContextAttribsARB( deviceContext, shareContext ? shareContext->renderContext : nullptr, &attributes[ 0 ] );

			if ( renderContext )		wglDeleteContext( oldContext );
			else						renderContext = oldContext;
		}
		else if ( shareContext )
		{
			static std::mutex		mutex;
			mutex.lock();

			if ( shareContext == GRHIContext )
			{
				if ( !wglMakeCurrent( shareContext->deviceContext, nullptr ) )
					throw std::runtime_error( "Failed to deactivate shared context before sharing. Code error: " + std::to_string( GetLastError() ) );

				GRHIContext = nullptr;
			}

			if ( !wglShareLists( shareContext->renderContext, renderContext ) )
				throw std::runtime_error( "Failed to share the OpenGL context. Code error: " + std::to_string( GetLastError() ) );

			mutex.unlock();
		}
	}
	catch ( std::exception& Exception )
	{
		LIFEENGINE_LOG_ERROR( "OpenGL4RHI", Exception.what() );
		return nullptr;
	}

	SRHIContext*			rhiContext = new SRHIContext( deviceContext, renderContext );
	WinGL_MakeCurrentContext( rhiContext );
	LIFEENGINE_LOG_INFO( "OpenGL4RHI", "Context OpenGL %s%s%s created", glGetString( GL_VERSION ),
						 ( isSetProfile && ( InSettingsContext.attributeFlags& SSettingsContext::CA_Core ) ? " core" : "" ),
						 ( isSetProfile && ( InSettingsContext.attributeFlags& SSettingsContext::CA_Debug ) ? " debug" : "" ) );
	
	return rhiContext;
}

/**
 * Make current context
 */
bool le::WinGL_MakeCurrentContext( FRHIContext InRHIContext )
{
	if ( static_cast< SRHIContext* >( InRHIContext ) == GRHIContext )
		return true;

	if ( !wglMakeCurrent( static_cast< SRHIContext* >( InRHIContext )->deviceContext, static_cast< SRHIContext* >( InRHIContext )->renderContext ) )
	{
		LIFEENGINE_LOG_ERROR( "OpenGL4RHI", "Failed in making current context 0x%p. Code error: %i", InRHIContext, GetLastError() );
		return false;
	}

	GRHIContext = static_cast< SRHIContext* >( InRHIContext );
	LIFEENGINE_LOG_INFO( "OpenGL4RHI", "Context 0x%p makeed current", InRHIContext );
	return true;
}

/**
 * Delete context
 */
void le::WinGL_DeleteContext( FRHIContext InRHIContext )
{
	if ( static_cast< SRHIContext* >( InRHIContext ) == GRHIContext )
	{
		wglMakeCurrent( nullptr, nullptr );
		GRHIContext = nullptr;
	}

	wglDeleteContext( static_cast< SRHIContext* >( InRHIContext )->renderContext );
	DeleteDC( static_cast< SRHIContext* >( InRHIContext )->deviceContext );
	delete static_cast< SRHIContext* >( InRHIContext );
	
	LIFEENGINE_LOG_INFO( "OpenGL4RHI", "Context 0x%p deleted", InRHIContext );
}

/**
 * Swap buffers
 */
void le::WinGL_SwapBuffers( FRHIContext InRHIContext )
{
	if ( static_cast< SRHIContext* >( InRHIContext ) != GRHIContext )		return;	
	SwapBuffers( static_cast< SRHIContext* >( InRHIContext )->deviceContext );
}

/**
 * Set vertical sync
 */
void le::WinGL_SetVerticalSync( bool InIsEnable )
{
	if ( wglewIsSupported( "WGL_EXT_swap_control" ) == 1 )
	{
		int			interval = 0;

		if ( InIsEnable )
			interval = wglewIsSupported( "WGL_EXT_swap_control_tear" ) == 1 ? -1 : 1;

		if ( !wglSwapIntervalEXT( interval ) )
			LIFEENGINE_LOG_ERROR( "OpenGL4RHI", "Setting vertical sync failed: %i", GetLastError() );
		else
			LIFEENGINE_LOG_INFO( "OpenGL4RHI", "Swap interval setted to %i", interval );
	}
	else
	{
		static bool			isWarned = false;
		if ( isWarned )
		{
			LIFEENGINE_LOG_WARNING( "OpenGL4RHI", "Setting vertical sync not supported" );
			isWarned = true;
		}
	}
}