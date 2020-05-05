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
#include "studiorender/imesh.h"

#include "engineapi.h"
#include "window_editor.h"
#include "ui_window_editor.h"
#include <nodes/NodeData>
#include <nodes/FlowScene>
#include <nodes/FlowView>
#include <nodes/ConnectionStyle>
#include <nodes/TypeConverter>

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
using QtNodes::FlowScene;

Window_Editor::Window_Editor( QWidget* Parent ) :
	QMainWindow( Parent ),
	ui( new Ui::Window_Editor() ),
	camera( nullptr )
{
	
	ui->setupUi( this );
	ui->widget_preview->Initialize();

	camera = ( le::ICamera* ) EngineAPI::GetInstance()->GetEngine()->GetFactory()->Create( CAMERA_INTERFACE_VERSION );
	LIFEENGINE_ASSERT( camera );

	camera->IncrementReference();
	camera->InitProjection_Perspective( 75.f, ( float ) ui->widget_preview->width() / ui->widget_preview->height(), 0.1f, 1500.f );
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