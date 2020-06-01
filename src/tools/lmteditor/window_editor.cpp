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
#include <qlistwidget.h>

#include "engine/shaderdescriptor.h"
#include "common/shaderparaminfo.h"
#include "engine/ifactory.h"
#include "engine/iengine.h"
#include "engine/icamera.h"
#include "engine/imaterialsystem.h"
#include "engine/ishaderfactory.h"
#include "engine/iconsolesystem.h"
#include "engine/imaterialproxyfactory.h"
#include "engine/materialproxyvarinfo.h"
#include "studiorender/imesh.h"
#include "studiorender/studiorenderviewport.h"

#include "errors.h"
#include "engineapi.h"
#include "window_editor.h"
#include "ui_window_editor.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Window_Editor::Window_Editor( const GameDescriptor& GameDescriptor, QWidget* Parent ) :
	QMainWindow( Parent ),
	ui( new Ui::Window_Editor() ),
	camera( nullptr ),
	model( nullptr ),
	pointLight( nullptr )
{
	ui->setupUi( this );
	if ( !ui->widget_preview->Initialize() )
		Error_Critical( "Failed initialize preview" );

	connect( ui->widget_preview, SIGNAL( ResizeViewport( quint32, quint32 ) ), this, SLOT( OnResizeViewport( quint32, quint32 ) ) );

	if ( !EngineAPI::GetInstance()->GetEngine()->LoadGame( GameDescriptor.path.toStdString().c_str() ) )
		Error_Critical( "Failed loading game" );

	// Creating camera for preview
	camera = ( le::ICamera* ) EngineAPI::GetInstance()->GetEngine()->GetFactory()->Create( CAMERA_INTERFACE_VERSION );
	if ( !camera )		Error_Critical( "Interface le::ICamera version[" CAMERA_INTERFACE_VERSION "] not found in core" );

	camera->IncrementReference();
	camera->InitProjection_Perspective( 75.f, ( float ) ui->widget_preview->width() / ui->widget_preview->height(), 0.1f, 5500.f );
	scene.SetCamera( camera );

	// Loading sphere for preview
	le::IMesh*			mesh = EngineAPI::GetInstance()->GetResourceSystem()->LoadMesh( "sphere", "models/sphere.mdl" );
	if ( !mesh )		Error_Critical( "Mesh [models/sphere.mdl] not found" );

	model = ( le::IModel* ) EngineAPI::GetInstance()->GetEngine()->GetFactory()->Create( MODEL_INTERFACE_VERSION );
	if ( !model )		Error_Critical( "Interface le::IModel version[" MODEL_INTERFACE_VERSION "] not found in core" );

	model->SetMesh( mesh );
	model->SetScale( le::Vector3D_t( 50, 50, 50 ) );
	model->SetPosition( le::Vector3D_t( 0, 0, -150 ) );
	scene.AddModel( model );

	// Creating light source
	pointLight = ( le::IPointLight* ) EngineAPI::GetInstance()->GetStudioRender()->GetFactory()->Create( POINTLIGHT_INTERFACE_VERSION );
	if ( !pointLight )		Error_Critical( "Interface le::IPointLight version[" POINTLIGHT_INTERFACE_VERSION "] not found in studiorender" );

	pointLight->SetPosition( le::Vector3D_t( 100, 0, 0 ) );
	pointLight->SetRadius( 350 );
	pointLight->SetIntensivity( 8500 );
	scene.AddLight( pointLight );

	// Getting all shaders list
	le::IShaderFactory*			shaderFactory = EngineAPI::GetInstance()->GetMaterialSystem()->GetShaderFactory();
	le::ShaderDescriptor*		shaderDescriptors = shaderFactory->GetShaders();
	quint32						countShaders = shaderFactory->GetCountShaders();
	if ( countShaders == 0 ) return;

	selectedShaderDescriptor = shaderDescriptors[ 0 ];
	for ( quint32 index = 0; index < countShaders; ++index )
		ui->comboBox_shader->addItem( shaderDescriptors[ index ].name );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Window_Editor::~Window_Editor()
{
	scene.Clear();

	if ( camera )
	{
		if ( camera->GetCountReferences() <= 1 )
			camera->Release();
		else
			camera->DecrementReference();
	}

	if ( model )
	{
		if ( model->GetCountReferences() <= 1 )
			model->Release();
		else
			model->DecrementReference();
	}

	if ( pointLight )
	{
		if ( pointLight->GetCountReferences() <= 1 )
			pointLight->Release();
		else
			pointLight->DecrementReference();
	}

	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: call context menu in list shader parameters
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_listWidget_parameters_customContextMenuRequested( QPoint Point )
{
	QMenu							contextMenu( this );
	QMenu							menu_add( "Add", this );
	QAction							action_delete( "Delete", this );
	QList< QAction* >				action_addShaders;

	if ( selectedShaderDescriptor.countParameters == 0 )	menu_add.setEnabled( false );
	else
		for ( quint32 index = 0; index < selectedShaderDescriptor.countParameters; ++index )
		{
			QAction*			shaderParameter = new QAction( selectedShaderDescriptor.parametersInfo[ index ].name, this );
			action_addShaders.append( shaderParameter );
			menu_add.addAction( shaderParameter );
			connect( shaderParameter, SIGNAL( triggered() ), this, SLOT( OnAddShaderParameter() ) );
		}

	if ( !ui->listWidget_parameters->itemAt( Point ) )		action_delete.setEnabled( false );
	
	contextMenu.addMenu( &menu_add );
	contextMenu.addAction( &action_delete );
	contextMenu.exec( QCursor::pos() );

	for ( auto it = action_addShaders.begin(), itEnd = action_addShaders.end(); it != itEnd; ++it )
		delete *it;
}

// ------------------------------------------------------------------------------------ //
// Event: call context menu in list proxies parameters
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_listWidget_proxiesParameters_customContextMenuRequested( QPoint Point )
{
	QMenu							contextMenu( this );
	QMenu							menu_add( "Add", this );
	QAction							action_delete( "Delete", this );
	QList< QAction* >				action_addProxyParameters;

	if ( selectedMaterialProxyDescriptor.countParameters == 0 )		menu_add.setEnabled( false );
	else
		for ( quint32 index = 0; index < selectedMaterialProxyDescriptor.countParameters; ++index )
		{
			QAction*			proxyParameter = new QAction( selectedMaterialProxyDescriptor.parametersInfo[ index ].name, this );
			action_addProxyParameters.append( proxyParameter );
			menu_add.addAction( proxyParameter );
			connect( proxyParameter, SIGNAL( triggered() ), this, SLOT( OnAddProxyParameter() ) );
		}

	if ( !ui->listWidget_proxiesParameters->itemAt( Point ) )		action_delete.setEnabled( false );

	contextMenu.addMenu( &menu_add );
	contextMenu.addAction( &action_delete );
	contextMenu.exec( QCursor::pos() );

	for ( auto it = action_addProxyParameters.begin(), itEnd = action_addProxyParameters.end(); it != itEnd; ++it )
		delete *it;
}

// ------------------------------------------------------------------------------------ //
// Event: add shader parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::OnAddShaderParameter()
{
	if ( !selectedShaderDescriptor.name ) return;

	QString			selectedShaderParameter = static_cast< QAction* >( sender() )->text();
	ui->listWidget_parameters->addItem( selectedShaderParameter );
}

// ------------------------------------------------------------------------------------ //
// Event: add proxy parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::OnAddProxyParameter()
{
	if ( !selectedMaterialProxyDescriptor.name ) return;

	QString			selectedProxyParameter = static_cast< QAction* >( sender() )->text();
	ui->listWidget_proxiesParameters->addItem( selectedProxyParameter );
}

// ------------------------------------------------------------------------------------ //
// Event: add proxy
// ------------------------------------------------------------------------------------ //
void Window_Editor::OnAddProxy()
{
	QString			selectedProxy = static_cast< QAction* >( sender() )->text();
	ui->listWidget_proxies->addItem( selectedProxy );
}

// ------------------------------------------------------------------------------------ //
// Event: resize viewport
// ------------------------------------------------------------------------------------ //
void Window_Editor::OnResizeViewport( quint32 Width, quint32 Height )
{
	if ( !camera ) return;
	camera->InitProjection_Perspective( 75.f, ( float ) Width / ( float ) Height, 0.1f, 5500.f );
}

// ------------------------------------------------------------------------------------ //
// Event: call context menu in list proxies
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_listWidget_proxies_customContextMenuRequested( QPoint Point )
{
	QMenu							contextMenu( this );
	QMenu							menu_add( "Add", this );
	QAction							action_delete( "Delete", this );
	QList< QAction* >				action_addProxies;

	le::IMaterialProxyFactory*			materialProxyFactory = EngineAPI::GetInstance()->GetMaterialSystem()->GetMaterialProxyFactory();
	le::MaterialProxyDescriptor*		materialProxyDescriptors = materialProxyFactory->GetMaterialProxes();
	if ( materialProxyFactory->GetCountMaterialProxes() == 0 )		menu_add.setEnabled( false );
	else
		for ( quint32 index = 0, count = materialProxyFactory->GetCountMaterialProxes(); index < count; ++index )
		{
			QAction*			proxy = new QAction( materialProxyDescriptors[ index ].name, this );
			action_addProxies.append( proxy );
			menu_add.addAction( proxy );
			connect( proxy, SIGNAL( triggered() ), this, SLOT( OnAddProxy() ) );
		}

	if ( !ui->listWidget_proxies->itemAt( Point ) )		action_delete.setEnabled( false );

	contextMenu.addMenu( &menu_add );
	contextMenu.addAction( &action_delete );
	contextMenu.exec( QCursor::pos() );

	for ( auto it = action_addProxies.begin(), itEnd = action_addProxies.end(); it != itEnd; ++it )
		delete *it;
}

// ------------------------------------------------------------------------------------ //
// Event: change selected shader
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_comboBox_shader_currentIndexChanged( int Value )
{
	le::IShaderFactory*			shaderFactory = EngineAPI::GetInstance()->GetMaterialSystem()->GetShaderFactory();
	le::ShaderDescriptor		shaderDescriptor = shaderFactory->GetShader( Value );
	if ( !shaderDescriptor.name ) return;

	selectedShaderDescriptor = shaderDescriptor;
	ui->listWidget_parameters->clear();
}

// ------------------------------------------------------------------------------------ //
// Event: selected shader parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_listWidget_parameters_itemClicked( QListWidgetItem* Item )
{}

// ------------------------------------------------------------------------------------ //
// Event: selected proxy
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_listWidget_proxies_itemClicked( QListWidgetItem* Item )
{}

// ------------------------------------------------------------------------------------ //
// Event: selected proxy parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_listWidget_proxiesParameters_itemClicked( QListWidgetItem* Item )
{}