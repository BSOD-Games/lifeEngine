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
#include <qmessagebox.h>
#include <QCloseEvent>

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
#include "model.h"

#include "window_importsettings.h"
#include "window_convertphy.h"

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
	connect( ui->widget_viewport, SIGNAL( MouseMove( quint32, quint32 ) ), this, SLOT( OnMouseMove( quint32, quint32 ) ) );

	qDebug() << "Loading game";

	if ( !EngineAPI::GetInstance()->GetEngine()->LoadGame( GameDescriptor.path.toStdString().c_str() ) )
		Error_Critical( "Failed loading game" );

	qDebug() << "Loaded game";

	qDebug() << "Start mesh initialize";

	qDebug() << "Mesh initialized";

	directionalLight = ( le::IDirectionalLight* ) EngineAPI::GetInstance()->GetStudioRender()->GetFactory()->Create( DIRECTIONALLIGHT_INTERFACE_VERSION );
	if ( !directionalLight )		Error_Critical( "Interface le::IDirectionalLight version[" DIRECTIONALLIGHT_INTERFACE_VERSION "] not found in studiorender" );

	directionalLight->SetDirection( le::Vector3D_t( 0.f, 0.5f, 0.5f ) );
	directionalLight->SetIntensivity( 0.5f );
	Scene::GetInstance()->AddLight( directionalLight );

	qDebug() << "Created directional light";

	camera = ( le::ICamera* ) EngineAPI::GetInstance()->GetEngine()->GetFactory()->Create( CAMERA_INTERFACE_VERSION );
	if ( !camera )    Error_Critical( "Interface le::ICamera version [" CAMERA_INTERFACE_VERSION "] don`t found in core" );

	camera->IncrementReference();
	camera->InitProjection_Perspective( 75.f, ( float ) ui->widget_viewport->width() / ui->widget_viewport->height(), 0.1f, 5500.f );
	camera->SetPosition( le::Vector3D_t( 0.f, 0.f, 150.f ) );
	Scene::GetInstance()->SetCamera( camera );

	qDebug() << "Loaded camera";

	ui->toolButton_pathMaterial->setDisabled( true );
	ui->actionSave->setDisabled( true );
	ui->actionSave_As->setDisabled( true );
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
	QString path = QFileDialog::getOpenFileName( this, "Choose model file",
		EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir, "Model file (*.mdl)" );
	if ( path.isEmpty() ) return;

	mesh.Load( path );

	RemoveAllMaterials();
	UpdateCameraPosition();

	std::vector<std::string>	paths = mesh.GetMaterialPaths();
	std::string					fileName = "";

	for ( le::UInt32_t index = 0, count = paths.size(); index < count; ++index )
	{
		std::size_t found = paths [ index ].find_last_of( "/\\" );
		fileName = paths [ index ].substr( found + 1 );
		ui->listWidget_materials->addItem( ( QString ) fileName.c_str() );
	}

	ui->toolButton_pathMaterial->setDisabled( true );
	ui->actionSave->setEnabled( true );
	ui->actionSave_As->setEnabled( true );
	ui->lineEdit_pathMaterial->setText( "" );
}

// ------------------------------------------------------------------------------------ //
// Event: save model
// ------------------------------------------------------------------------------------ //
void Window_Viewer::on_actionSave_triggered()
{
	QMessageBox::Button result =
		QMessageBox::information( this, "Info", "Do you want save this model?", QMessageBox::Button::Ok, QMessageBox::Button::Cancel );

	if ( result == QMessageBox::Button::Ok )
		mesh.Save();
}

// ------------------------------------------------------------------------------------ //
// Event: save model as
// ------------------------------------------------------------------------------------ //
void Window_Viewer::on_actionSave_As_triggered()
{
	QMessageBox::QMessageBox::Button result =
		QMessageBox::information( this, "Info", "Do you want save this model?", QMessageBox::Button::Ok, QMessageBox::Button::Cancel );

	if ( result == QMessageBox::Button::Ok )
		mesh.SaveAs( QFileDialog::getSaveFileName( this, "Save model as",
			EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir, "Model file (*.mdl)" ) );
}

// ------------------------------------------------------------------------------------ //
// Event: import model
// ------------------------------------------------------------------------------------ //
void Window_Viewer::on_actionImport_triggered()
{
	Window_ImportSettings		windowImport;

	windowImport.exec();
}

// ------------------------------------------------------------------------------------ //
// Event: convert to phy
// ------------------------------------------------------------------------------------ //
void Window_Viewer::on_actionConvert_to_PHY_triggered()
{
	Window_ConvertPHY		windowConvert;

	windowConvert.exec();
}

// ------------------------------------------------------------------------------------ //
// Event: select item on the material list
// ------------------------------------------------------------------------------------ //
void Window_Viewer::on_listWidget_materials_itemSelectionChanged()
{
	if ( ui->listWidget_materials->currentRow() == -1 )
		return;

	std::vector<std::string> paths = mesh.GetMaterialPaths();
	ui->lineEdit_pathMaterial->setText( paths [ ui->listWidget_materials->currentRow() ].c_str() );
	ui->toolButton_pathMaterial->setDisabled( false );
}

// ------------------------------------------------------------------------------------ //
// Event: load material
// ------------------------------------------------------------------------------------ //
void Window_Viewer::on_toolButton_pathMaterial_clicked()
{
	QDir dir = EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir;
	QString path = dir.relativeFilePath( QFileDialog::getOpenFileName( this, "Choose material file",
		EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir, "Material file (*.lmt)" ) );

	if ( path.isEmpty() ) return;

	ui->lineEdit_pathMaterial->setText( path );
	mesh.LoadMaterial( path, ui->listWidget_materials->currentRow() );

	ui->listWidget_materials->clear();

	//create names for material list
	std::vector<std::string> paths = mesh.GetMaterialPaths();
	std::string fileName = "";
	for ( le::UInt32_t index = 0, count = paths.size(); index < count; ++index )
	{
		std::size_t found = paths [ index ].find_last_of( "/\\" );
		fileName = paths [ index ].substr( found + 1 );
		ui->listWidget_materials->addItem( ( QString ) fileName.c_str() );
	}
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on check box wireframe
// ------------------------------------------------------------------------------------ //
void Window_Viewer::on_checkBox_wireframe_clicked()
{
	if ( ui->checkBox_wireframe->checkState() == Qt::CheckState::Checked )
		EngineAPI::GetInstance()->GetConsoleSystem()->Exec( "r_wireframe 1" );
	else
		EngineAPI::GetInstance()->GetConsoleSystem()->Exec( "r_wireframe 0" );
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on check box ground
// ------------------------------------------------------------------------------------ //
void Window_Viewer::on_checkBox_ground_clicked()
{
	Scene::GetInstance()->EnableGround( ui->checkBox_ground->checkState() == Qt::CheckState::Checked );
}

// ------------------------------------------------------------------------------------ //
// Update camera position 
// ------------------------------------------------------------------------------------ //
void Window_Viewer::UpdateCameraPosition()
{
	le::Vector3D_t maxXYZ = mesh.GetMesh()->GetMax();

	float max = glm::max( maxXYZ.x, glm::max( maxXYZ.y, maxXYZ.z ) );

	camera->SetPosition( le::Vector3D_t( 0, maxXYZ.y / 2.f, max + 150.f ) );
}

// ------------------------------------------------------------------------------------ //
// Delete materials before load model
// ------------------------------------------------------------------------------------ //
void Window_Viewer::RemoveAllMaterials()
{
	ui->listWidget_materials->clear();
	ui->listWidget_materials->clearFocus();
}

// ------------------------------------------------------------------------------------ //
// Event: resize viewport
// ------------------------------------------------------------------------------------ //
void Window_Viewer::OnResizeViewport( quint32 Width, quint32 Height )
{
	if ( !camera ) return;
	camera->InitProjection_Perspective( 75.f, ( float ) Width / ( float ) Height, 0.1f, 5500.f );
}

void Window_Viewer::OnMouseMove( quint32 PositionX, quint32 PositionY )
{
	if ( !camera ) return;


	mesh.RotateByMouse( le::Vector2D_t( PositionX, PositionY ), 0.15f );
}