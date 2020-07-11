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
#include <qwidget.h>
#include <qfiledialog.h>
#include <string>

#include "engine/iengine.h"
#include "engine/iresourcesystem.h"
#include "engine/ifactory.h"
#include "engineapi.h"
#include "errors.h"
#include "window_viewer.h"
#include "widget_viewport.h"
#include "ui_window_viewer.h"
#include "studiorender/imesh.h"
#include "engine/imodel.h"
#include "common/meshsurface.h"
#include "common/meshdescriptor.h"
#include "studiorender/studiovertexelement.h"
#include "engine/consolesystem.h"
#include "mesh.h"

#include "common/gameinfo.h"

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

	model = ( le::IModel* ) EngineAPI::GetInstance()->GetEngine()->GetFactory()->Create( MODEL_INTERFACE_VERSION );
	if ( !model )	Error_Critical( "Interface le::IModel Version [" MODEL_INTERFACE_VERSION "] not found in core" );
	//scene.AddModel(model);

	qDebug() << "Loaded model";

	directionalLight = (le::IDirectionalLight*) EngineAPI::GetInstance()->GetStudioRender()->GetFactory()->Create(DIRECTIONALLIGHT_INTERFACE_VERSION);
	if (!directionalLight)		Error_Critical("Interface le::IDirectionalLight version[" DIRECTIONALLIGHT_INTERFACE_VERSION "] not found in studiorender");

	directionalLight->SetDirection(le::Vector3D_t(0.f, 0.5f, 0.5f));
	directionalLight->SetIntensivity(0.5f);
	scene.AddLight(directionalLight);

	qDebug() << "Created directional light";

	camera = (le::ICamera*) EngineAPI::GetInstance()->GetEngine()->GetFactory()->Create( CAMERA_INTERFACE_VERSION );
	if ( !camera )    Error_Critical( "Interface le::ICamera version [" CAMERA_INTERFACE_VERSION "] don`t found in core" );

	camera->IncrementReference();
	camera->InitProjection_Perspective( 75.f, ( float )ui->widget_viewport->width() / ui->widget_viewport->height(), 0.1f, 5500.f );
	camera->SetPosition(le::Vector3D_t( 0.f, 0.f, 150.f ) );
	scene.SetCamera( camera );

	qDebug() << "Loaded camera";

	ui->toolButton_pathMaterial->setDisabled(true);

//	EngineAPI::GetInstance()->GetConsoleSystem()->Exec("r_showgbuffer 1");
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Window_Viewer::~Window_Viewer()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: open new model
// ------------------------------------------------------------------------------------ //
void Window_Viewer::on_actionOpen_triggered()
{
	QString path = QFileDialog::getOpenFileName(this, "Choose model file", EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir, "Model files (*.mdl)");
	if( path.isEmpty() ) return;
	
	mesh.Load( path );
	model->SetMesh( mesh.GetMesh() );
	std::vector<QString> paths = mesh.GetPaths();
	if( paths.empty() ) return;
	std::string fileName = "";
	for (le::UInt32_t index = 0, count = paths.size(); index < count; ++index)
	{
		std::size_t found = paths[index].toStdString().find_last_of("/\\");
		fileName = paths[index].toStdString().substr(found + 1);
		ui->listWidget_materials->addItem((QString)fileName.c_str());
	}

	scene.AddModel( model );
}

void Window_Viewer::on_listWidget_materials_itemSelectionChanged()
{
	if (ui->listWidget_materials->currentRow() == -1)
		return;

	std::vector<QString> paths = mesh.GetPaths();
	ui->lineEdit_pathMaterial->setText(paths[ui->listWidget_materials->currentRow()]);
	ui->toolButton_pathMaterial->setDisabled(false);
}

// ------------------------------------------------------------------------------------ //
// Event:load material
// ------------------------------------------------------------------------------------ //
void Window_Viewer::on_toolButton_pathMaterial_clicked()
{
	QDir dir = EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir;
	QString path = dir.relativeFilePath( QFileDialog::getOpenFileName(this, "Choose material file", EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir, "*.lmt"));
	if (path.isEmpty()) return;
	
	qDebug() << "Material path: " << path;

	mesh.LoadMaterial(path, ui->listWidget_materials->currentRow());

	ui->listWidget_materials->clear();
	std::vector<QString> paths = mesh.GetPaths();
	std::string fileName = "";
	for (le::UInt32_t index = 0, count = paths.size(); index < count; ++index)
	{
		std::size_t found = paths[index].toStdString().find_last_of("/\\");
		fileName = paths[index].toStdString().substr(found + 1);
		ui->listWidget_materials->addItem((QString)fileName.c_str());
	}

}

// ------------------------------------------------------------------------------------ //
// Event: resize viewport
// ------------------------------------------------------------------------------------ //
void Window_Viewer::OnResizeViewport( quint32 Width, quint32 Height )
{
	if (!camera) return;
		camera->InitProjection_Perspective(75.f, (float)Width / (float)Height, 0.1f, 5500.f);
}