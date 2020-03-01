//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "settingscontext.h"
#include "rendercontext.h"

#if defined( PLATFORM_WINDOWS )
#	include "win32/wglcontext.h"
#elif defined( PLATFORM_LINUX )
#   include "unix/glxcontext.h"
#else
#	error Not supported platform
#endif

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::RenderContext::RenderContext() :
	isCreated( false ),
	windowHandle( nullptr ),
	contextDescriptor( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::RenderContext::~RenderContext()
{
	if ( isCreated )		Destroy();
}

// ------------------------------------------------------------------------------------ //
// Создать контекст
// ------------------------------------------------------------------------------------ //
bool le::RenderContext::Create( WindowHandle_t WindowHandle, const SettingsContext& SettingsContext )
{
	if ( isCreated ) return true;

#if defined( PLATFORM_WINDOWS )
	isCreated = WinGL_CreateContext( WindowHandle, SettingsContext, contextDescriptor );
#elif defined( PLATFORM_LINUX )
    isCreated = GLX_CreateContext( WindowHandle, SettingsContext, contextDescriptor );
#endif

	return isCreated;
}

// ------------------------------------------------------------------------------------ //
// Сделать текущим контекстом
// ------------------------------------------------------------------------------------ //
void le::RenderContext::MakeCurrent()
{
	if ( !isCreated )	return;

#if defined( PLATFORM_WINDOWS )
	WinGL_MakeCurrentContext( contextDescriptor );
#elif defined( PLATFORM_LINUX )
    GLX_MakeCurrentContext( contextDescriptor );
#endif
}

// ------------------------------------------------------------------------------------ //
// Уничтожить контекст
// ------------------------------------------------------------------------------------ //
void le::RenderContext::Destroy()
{
	if ( !isCreated )	return;
	isCreated = false;

#if defined( PLATFORM_WINDOWS )
	WinGL_DeleteContext( contextDescriptor );
#elif defined( PLATFORM_LINUX )
    GLX_DeleteContext( contextDescriptor );
#endif
}

// ------------------------------------------------------------------------------------ //
// Сменить буферы
// ------------------------------------------------------------------------------------ //
void le::RenderContext::SwapBuffers()
{
	if ( !isCreated )	return;

#if defined( PLATFORM_WINDOWS )
	WinGL_SwapBuffers( contextDescriptor );
#elif defined( PLATFORM_LINUX )
    GLX_SwapBuffers( contextDescriptor );
#endif
}

// ------------------------------------------------------------------------------------ //
// Включить вертикальную синхронизацию
// ------------------------------------------------------------------------------------ //
void le::RenderContext::SetVerticalSync( bool IsEnable )
{
	if ( !isCreated )	return;

#if defined( PLATFORM_WINDOWS )
	WinGL_SetVerticalSync( IsEnable );
#elif defined( PLATFORM_LINUX )
    GLX_SetVerticalSync( IsEnable );
#endif
}

//----------------------------------------------------------------------//
