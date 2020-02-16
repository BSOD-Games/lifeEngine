//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include <mutex>
#include <Windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>

#include "engine/iconsolesystem.h"
#include "wglcontext.h"
#include "settingscontext.h"
#include "global.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct ContextDescriptor
	{
		ContextDescriptor( HDC DeviceContext, HGLRC RenderContext ) :
			deviceContext( DeviceContext ),
			renderContext( RenderContext )
		{}

		HDC			deviceContext;
		HGLRC		renderContext;
	};

	//---------------------------------------------------------------------//
}

static le::ContextDescriptor*				currentContext = nullptr;

// ------------------------------------------------------------------------------------ //
// Создать контекст
// ------------------------------------------------------------------------------------ //
bool le::WinGL_CreateContext( WindowHandle_t WindowHandle, const SettingsContext& SettingsContext, ContextDescriptor_t& ContextDescriptor, ContextDescriptor_t* ShareContext )
{
	PIXELFORMATDESCRIPTOR				pixelFormatDesc = {};
	UInt32_t							colorBits = SettingsContext.redBits + SettingsContext.greenBits + SettingsContext.blueBits + SettingsContext.alphaBits;

	pixelFormatDesc.nSize = sizeof( PIXELFORMATDESCRIPTOR );
	pixelFormatDesc.nVersion = 1;
	pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDesc.iLayerType = PFD_MAIN_PLANE;
	pixelFormatDesc.cColorBits = static_cast<BYTE>( colorBits );
	pixelFormatDesc.cDepthBits = static_cast<BYTE>( SettingsContext.depthBits );
	pixelFormatDesc.cStencilBits = static_cast<BYTE>( SettingsContext.stencilBits );
	pixelFormatDesc.cAlphaBits = colorBits == 32 ? 8 : 0;

	HDC						deviceContext = GetDC( static_cast< HWND >( WindowHandle ) );
	HGLRC					renderContext = nullptr;

	le::ContextDescriptor*	shareContext = ShareContext ? static_cast< le::ContextDescriptor* >( *ShareContext ) : nullptr;
	int						pixelFormat = ChoosePixelFormat( deviceContext, &pixelFormatDesc );
	bool					isSetProfile = false;

	try
	{
		// Создаем временый контекст

		if ( pixelFormat != 0 )
		{
			PIXELFORMATDESCRIPTOR			bestMatch_PixelFormatDescd;
			DescribePixelFormat( deviceContext, pixelFormat, sizeof( PIXELFORMATDESCRIPTOR ), &bestMatch_PixelFormatDescd );

			if ( bestMatch_PixelFormatDescd.cDepthBits < pixelFormatDesc.cDepthBits )
				g_consoleSystem->PrintWarning( "Depth bits: %i not supported, seted to %i", pixelFormatDesc.cDepthBits, bestMatch_PixelFormatDescd.cDepthBits );

			if ( SetPixelFormat( deviceContext, pixelFormat, &pixelFormatDesc ) == FALSE )
				throw "Failed seted pixel fromat";
		}
		else throw "Failed choose pixel format";

		renderContext = wglCreateContext( deviceContext );
		if ( !renderContext )
			throw "Creating temporary render context fail. Code error: " + to_string( GetLastError() );

		if ( !wglMakeCurrent( deviceContext, renderContext ) )
			throw "Selecting temporary render context fail. Code error: " + to_string( GetLastError() );

		// Инициализируем GLEW

		glewExperimental = GL_TRUE;
		if ( glewInit() != GLEW_OK )	throw "OpenGL context is broken";

		// Выводим параметры видеокарты и поддерживаемы расширения

		int			numberExtensions = 0;
		glGetIntegerv( GL_NUM_EXTENSIONS, &numberExtensions );

		g_consoleSystem->PrintInfo( "*** OpenGL info ***" );
		g_consoleSystem->PrintInfo( "  OpenGL version: %s", glGetString( GL_VERSION ) );
		g_consoleSystem->PrintInfo( "  OpenGL vendor: %s", glGetString( GL_VENDOR ) );
		g_consoleSystem->PrintInfo( "  OpenGL renderer: %s", glGetString( GL_RENDERER ) );
		g_consoleSystem->PrintInfo( "  OpenGL GLSL version: %s", glGetString( GL_SHADING_LANGUAGE_VERSION ) );
		g_consoleSystem->PrintInfo( "  OpenGL extensions:" ); 

		for ( uint32_t index = 0; index < numberExtensions; ++index )
		{
			const uint8_t*		extension = glGetStringi( GL_EXTENSIONS, index );
			g_consoleSystem->PrintInfo( ( char* ) extension );
		}

		g_consoleSystem->PrintInfo( "*** OpenGL info end ***" );

		// Создаем расширеный контекст через wglCreateContextAttribsARB. Если он не поддерживается,
		// оставляем старый контекст

		if ( wglewIsSupported( "WGL_ARB_create_context" ) == 1 )
		{
			std::vector<int>				attributes;

			// Если версия OpenGL больше 1.1, то записываем в атрибуты версию OpenGL

			if ( SettingsContext.majorVersion > 1 || ( SettingsContext.majorVersion == 1 && SettingsContext.minorVersion > 1 ) )
			{
				attributes.push_back( WGL_CONTEXT_MAJOR_VERSION_ARB );
				attributes.push_back( SettingsContext.majorVersion );
				attributes.push_back( WGL_CONTEXT_MINOR_VERSION_ARB );
				attributes.push_back( SettingsContext.minorVersion );
			}

			// Выбираем профиль контекста (core, debug, etc)

			if ( wglewIsSupported( "WGL_ARB_create_context_profile" ) == 1 )
			{
				int				profile = ( SettingsContext.attributeFlags & le::SettingsContext::CA_CORE ) ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
				int				debug = ( SettingsContext.attributeFlags & le::SettingsContext::CA_DEBUG ) ? WGL_CONTEXT_DEBUG_BIT_ARB : 0;

				attributes.push_back( WGL_CONTEXT_PROFILE_MASK_ARB );
				attributes.push_back( profile );
				attributes.push_back( WGL_CONTEXT_FLAGS_ARB );
				attributes.push_back( debug );

				isSetProfile = true;
			}
			else if ( ( SettingsContext.attributeFlags & le::SettingsContext::CA_CORE ) || ( SettingsContext.attributeFlags & le::SettingsContext::CA_DEBUG ) )
					g_consoleSystem->PrintWarning( "Selecting a profile during context creation is not supported, disabling comptibility and debug" );

			// Добавляем признак конца массива

			attributes.push_back( 0 );
			attributes.push_back( 0 );

			if ( shareContext )
			{
				static mutex		mutex;
				mutex.lock();

				if ( shareContext == currentContext )
				{
					if ( !wglMakeCurrent( shareContext->deviceContext, nullptr ) )
						throw "Failed to deactivate shared context before sharing. Code error: " + to_string( GetLastError() );

					currentContext = nullptr;
				}

				mutex.unlock();
			}

			HGLRC			oldContext = renderContext;
			renderContext = wglCreateContextAttribsARB( deviceContext, shareContext ? shareContext->renderContext : nullptr, &attributes[ 0 ] );

			if ( renderContext )
				wglDeleteContext( oldContext );
			else
				renderContext = oldContext;
		}
		else if ( shareContext )
		{
			static mutex		mutex;
			mutex.lock();

			if ( shareContext == currentContext )
			{
				if ( !wglMakeCurrent( shareContext->deviceContext, nullptr ) )
					throw "Failed to deactivate shared context before sharing. Code error: " + std::to_string( GetLastError() );

				currentContext = nullptr;
			}

			if ( !wglShareLists( shareContext->renderContext, renderContext ) )
				throw "Failed to share the OpenGL context. Code error: " + std::to_string( GetLastError() );

			mutex.unlock();
		}

	}
	catch ( const string& Message )
	{
		g_consoleSystem->PrintError( Message.c_str() );
		return false;
	}

	le::ContextDescriptor*			contextDescriptor = new le::ContextDescriptor( deviceContext, renderContext );
	ContextDescriptor = contextDescriptor;
	WinGL_MakeCurrentContext( contextDescriptor );

	g_consoleSystem->PrintInfo( "Context OpenGL %s%s%s created", glGetString( GL_VERSION ), 
		( isSetProfile && ( SettingsContext.attributeFlags& le::SettingsContext::CA_CORE ) ? " core" : "" ),
		( isSetProfile && ( SettingsContext.attributeFlags& le::SettingsContext::CA_DEBUG ) ? " debug" : "" ) );

	return true;
}

// ------------------------------------------------------------------------------------ //
// Сделать текущим контекст
// ------------------------------------------------------------------------------------ //
bool le::WinGL_MakeCurrentContext( const ContextDescriptor_t& ContextDescriptor )
{
	if ( static_cast< le::ContextDescriptor* >( ContextDescriptor ) == currentContext ) return true;

	if ( !wglMakeCurrent( static_cast< le::ContextDescriptor* >( ContextDescriptor )->deviceContext, static_cast< le::ContextDescriptor* >( ContextDescriptor )->renderContext ) )
		return false;

	currentContext = static_cast< le::ContextDescriptor* >( ContextDescriptor );
	return true;
}

// ------------------------------------------------------------------------------------ //
// Удалить контекст
// ------------------------------------------------------------------------------------ //
void le::WinGL_DeleteContext( ContextDescriptor_t& ContextDescriptor )
{
	if ( static_cast< le::ContextDescriptor* >( ContextDescriptor ) == currentContext )
	{
		wglMakeCurrent( nullptr, nullptr );
		currentContext = nullptr;
	}

	wglDeleteContext( static_cast< le::ContextDescriptor* >( ContextDescriptor )->renderContext );
	DeleteDC( static_cast< le::ContextDescriptor* >( ContextDescriptor )->deviceContext );
	delete static_cast< le::ContextDescriptor* >( ContextDescriptor );
	ContextDescriptor = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Сменить буферы
// ------------------------------------------------------------------------------------ //
void le::WinGL_SwapBuffers( const ContextDescriptor_t& ContextDescriptor )
{
	if ( static_cast< le::ContextDescriptor* >( ContextDescriptor ) != currentContext ) return;
	SwapBuffers( static_cast< le::ContextDescriptor* >( ContextDescriptor )->deviceContext );
}

// ------------------------------------------------------------------------------------ //
// Включить вертикальную синхронизацию
// ------------------------------------------------------------------------------------ //
void le::WinGL_SetVerticalSync( bool IsEnable )
{		
	if ( wglewIsSupported( "WGL_EXT_swap_control" ) == 1 )
	{
		int			interval = 0;

		if ( IsEnable )
			interval = wglewIsSupported( "WGL_EXT_swap_control_tear" ) == 1 ? -1 : 1;

		if ( !wglSwapIntervalEXT( interval ) )
			g_consoleSystem->PrintError( "Setting vertical sync failed: %i", GetLastError() );
	}
	else
	{
		static bool			isWarned = false;

		if ( isWarned )
		{
			g_consoleSystem->PrintWarning( "Setting vertical sync not supported" );
			isWarned = true;
		}
	}
}

//---------------------------------------------------------------------//