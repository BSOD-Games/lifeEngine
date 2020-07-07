//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <qdebug.h>

#include "engineapi.h"
#include "errors.h"
#include "window_viewer.h"
#include "ui_window_viewer.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Window_Viewer::Window_Viewer( const GameDescriptor& GameDescriptor, QWidget* Parent ) :
	QMainWindow( Parent ),
	ui( new Ui::Window_Viewer() )
{
	ui->setupUi( this );
	if ( !ui->widget_viewport->Initialize() )
		Error_Critical( "Failed initialize viewport" );

	connect( ui->widget_viewport, SIGNAL( ResizeViewport( quint32, quint32 ) ), this, SLOT( OnResizeViewport( quint32, quint32 ) ) );

	qDebug() << "Loading game";

	if ( !EngineAPI::GetInstance()->GetEngine()->LoadGame( GameDescriptor.path.toStdString().c_str() ) )
		Error_Critical( "Failed loading game" );

	qDebug() << "Loaded game";
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Window_Viewer::~Window_Viewer()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: resize viewport
// ------------------------------------------------------------------------------------ //
void Window_Viewer::OnResizeViewport( quint32 Width, quint32 Height )
{
}