//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/ifactory.h"
#include "engine/icamera.h"

#include "engineapi.h"
#include "window_editor.h"
#include "ui_window_editor.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Window_Editor::Window_Editor( QWidget* Parent ) :
	QMainWindow( Parent ),
	ui( new Ui::Window_Editor() ),
	camera( nullptr )
{
	ui->setupUi( this );
	ui->widget_viewport->Initialize();

	camera = ( le::ICamera* ) EngineAPI::GetInstance()->GetEngine()->GetFactory()->Create( CAMERA_INTERFACE_VERSION );
	LIFEENGINE_ASSERT( camera );

	camera->IncrementReference();
	camera->InitProjection_Perspective( 75.f, ( float ) ui->widget_viewport->width() / ui->widget_viewport->height(), 0.1f, 1500.f );
	scene.SetCamera( camera );

	// TODO:
	// 1. Add loading sphere mesh and render
	// 2. Add filling comboboxes: Techniques and Passes
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Window_Editor::~Window_Editor()
{
	if ( camera )
	{
		if ( camera->GetCountReferences() <= 1 )
			camera->Release();
		else
			camera->DecrementReference();
	}

	delete ui;
}
