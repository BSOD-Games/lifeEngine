//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>

#include "common/configurations.h"
#include "engine/lifeengine.h"
#include "engine/iengine.h"
#include "engine/iwindow.h"
#include "engine/iconsolesystem.h"
#include "settingscontext.h"
#include "common/shaderdescriptor.h"

#include "global.h"
#include "studiorender.h"
#include "gpuprogram.h"

LIFEENGINE_STUDIORENDER_API( le::StudioRender );

// ------------------------------------------------------------------------------------ //
// Изменить размер вьюпорта
// ------------------------------------------------------------------------------------ //
void le::StudioRender::ResizeViewport( UInt32_t X, UInt32_t Y, UInt32_t Width, UInt32_t Height )
{
	if ( !renderContext.IsCreated() ) return;
	glViewport( X, Y, Width, Height );
}

// ------------------------------------------------------------------------------------ //
// Подключиться к движку
// ------------------------------------------------------------------------------------ //
bool le::StudioRender::Connect( IFactory* Factory )
{
	try
	{
		if ( !Factory )				throw std::exception( "studiorender requires factory engine" );

		g_engine = ( IEngine* ) Factory->Create( ENGINE_INTERFACE_VERSION );
		if ( !g_engine )			throw std::exception( "studiorender requires engine api" );

		g_consoleSystem = ( IConsoleSystem* ) Factory->Create( CONSOLE_SYSTEM_INTERFACE_VERSION );
		if ( !g_consoleSystem )		throw std::exception( "studiorender requires consoleSystem" );

		g_engineFactory = Factory;
	}
	catch ( std::exception& Exception )
	{
		if ( g_consoleSystem )		g_consoleSystem->PrintError( Exception.what() );
		else						printf( Exception.what() );

		Disconnect();
		return false;
	}
	
	isConnect = true;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Отключится от движка
// ------------------------------------------------------------------------------------ //
void le::StudioRender::Disconnect()
{
	g_engine = nullptr;
	g_consoleSystem = nullptr;
	g_engineFactory = nullptr;

	isConnect = false;
}

// ------------------------------------------------------------------------------------ //
// Прекратить работу рендера
// ------------------------------------------------------------------------------------ //
void le::StudioRender::Shutdown()
{
	renderContext.Destroy();
	isInitialize = false;
}

// ------------------------------------------------------------------------------------ //
// Инициализировать рендер
// ------------------------------------------------------------------------------------ //
bool le::StudioRender::Initialize()
{
	if ( !isConnect ) return false;
	if ( isInitialize ) return true;

	// Если в ядре окно не создано (указатель на IWindow nullptr) или
	// заголовок окна nullptr, то выбрасываем ошибку

	if ( !g_engine->GetWindow() || !g_engine->GetWindow()->GetHandle() )
	{
		g_consoleSystem->PrintError( "Window not open or not valid handle" );
		return false;
	}

	// Создаем контекст OpenGL

	Configurations				configurations = g_engine->GetConfigurations();
	SettingsContext				settingsContext;
	settingsContext.redBits = 8;
	settingsContext.greenBits = 8;
	settingsContext.blueBits = 8;
	settingsContext.alphaBits = 8;
	settingsContext.depthBits = 24;
	settingsContext.stencilBits = 8;
	settingsContext.majorVersion = 3;
	settingsContext.minorVersion = 3;
	settingsContext.attributeFlags = SettingsContext::CA_CORE;

	if ( !renderContext.Create( g_engine->GetWindow()->GetHandle(), settingsContext ) )
	{
		g_consoleSystem->PrintError( "Failed created context" );
		return false;
	}

	renderContext.SetVerticalSync( configurations.isVerticalSinc );

	// Инициализируем OpenGL

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	return true;
}

// ------------------------------------------------------------------------------------ //
// Инициализировать рендер
// ------------------------------------------------------------------------------------ //
le::IFactory* le::StudioRender::GetFactory() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Отрисовать кадр
// ------------------------------------------------------------------------------------ //
void le::StudioRender::RenderFrame()
{
	if ( !renderContext.IsCreated() ) return;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	renderContext.SwapBuffers();
}

// ------------------------------------------------------------------------------------ //
// Включить вертикальную синхронизацию
// ------------------------------------------------------------------------------------ //
void le::StudioRender::SetVerticalSyncEnabled( bool IsEnabled )
{
	if ( !renderContext.IsCreated() ) return;
	renderContext.SetVerticalSync( IsEnabled );
}
