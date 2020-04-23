//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <QResizeEvent>

#include "engine/lifeengine.h"
#include "studiorender/studiorenderviewport.h"

#include "engineapi.h"
#include "scene.h"
#include "widget_viewport.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_Viewport::Widget_Viewport( QWidget* Parent) :
	QWidget( Parent ),
	isInitialize( false ),
	studioRender( nullptr )
{
	setAttribute( Qt::WA_PaintOnScreen );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_Viewport::~Widget_Viewport()
{
	isInitialize = false;
	disconnect( &timerUpdate, &QTimer::timeout, this, &Widget_Viewport::Render );
	timerUpdate.stop();
}

// ------------------------------------------------------------------------------------ //
// Initialize render
// ------------------------------------------------------------------------------------ //
bool Widget_Viewport::Initialize()
{
	le::WindowHandle_t				windowHandle = nullptr;
	studioRender = ( le::IStudioRenderInternal* ) EngineAPI::GetInstance()->GetEngine()->GetStudioRender();
	if ( !studioRender ) return false;

#if defined( PLATFORM_WINDOWS )
		windowHandle = ( le::WindowHandle_t ) winId();
#elif defined( PLATFORM_LINUX )
		WId                 winid = winId();
		windowHandle = &winid;
#else
#	error	Not supported platform
#endif

	isInitialize = studioRender->CreateContext( windowHandle, width(), height() );
	if ( !isInitialize ) return false;

	Scene::GetInstance()->SetStudioRender( studioRender );
	connect( &timerUpdate, &QTimer::timeout, this, &Widget_Viewport::Render );
	timerUpdate.start( 0 );
	return isInitialize;
}

// ------------------------------------------------------------------------------------ //
// Render frame
// ------------------------------------------------------------------------------------ //
void Widget_Viewport::Render()
{
	if ( !studioRender )
		return;

	studioRender->Begin();
	Scene::GetInstance()->Render();
	studioRender->End();
	studioRender->Present();
}

// ------------------------------------------------------------------------------------ //
// Get paint engine
// ------------------------------------------------------------------------------------ //
QPaintEngine* Widget_Viewport::paintEngine() const
{
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Paint event
// ------------------------------------------------------------------------------------ //
void Widget_Viewport::paintEvent( QPaintEvent* Event )
{}

// ------------------------------------------------------------------------------------ //
// Resize event
// ------------------------------------------------------------------------------------ //
void Widget_Viewport::resizeEvent( QResizeEvent* Event )
{
	if ( studioRender )
		studioRender->SetViewport( { 0, 0, ( le::UInt32_t ) width(), ( le::UInt32_t ) height() } );

	QWidget::resizeEvent( Event );
}

// ------------------------------------------------------------------------------------ //
// Show event
// ------------------------------------------------------------------------------------ //
void Widget_Viewport::showEvent( QShowEvent* Event )
{
	QWidget::showEvent( Event );
}
