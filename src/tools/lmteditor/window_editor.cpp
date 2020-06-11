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
#include <qmessagebox.h>
#include <qfiledialog.h>

#include "engine/shaderdescriptor.h"
#include "common/shaderparaminfo.h"
#include "common/gameinfo.h"
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

#include "lmteditor.h"
#include "errors.h"
#include "engineapi.h"
#include "window_editor.h"
#include "widget_shaderparameter_texture.h"
#include "widget_shaderparameter_color.h"
#include "widget_shaderparameter_vector4d.h"
#include "widget_shaderparameter_vector3d.h"
#include "widget_shaderparameter_vector2d.h"
#include "widget_shaderparameter_int.h"
#include "widget_shaderparameter_float.h"
#include "widget_shaderparameter_bool.h"
#include "ui_window_editor.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Window_Editor::Window_Editor( const GameDescriptor& GameDescriptor, QWidget* Parent ) :
	QMainWindow( Parent ),
	ui( new Ui::Window_Editor() ),
	camera( nullptr ),
	model( nullptr ),
	pointLight( nullptr ),
	widget_shaderParameter( nullptr )
{
	ui->setupUi( this );
	if ( !ui->widget_preview->Initialize() )
		Error_Critical( "Failed initialize preview" );

	connect( ui->widget_preview, SIGNAL( ResizeViewport( quint32, quint32 ) ), this, SLOT( OnResizeViewport( quint32, quint32 ) ) );

	if ( !EngineAPI::GetInstance()->GetEngine()->LoadGame( GameDescriptor.path.toStdString().c_str() ) )
		Error_Critical( "Failed loading game" );

	// Getting all shaders list
	le::IShaderFactory*			shaderFactory = EngineAPI::GetInstance()->GetMaterialSystem()->GetShaderFactory();
	le::ShaderDescriptor*		shaderDescriptors = shaderFactory->GetShaders();
	quint32						countShaders = shaderFactory->GetCountShaders();
	if ( countShaders == 0 ) return;

	selectedShaderDescriptor = shaderDescriptors[ 0 ];
	for ( quint32 index = 0; index < countShaders; ++index )
		ui->comboBox_shader->addItem( shaderDescriptors[ index ].name );

	// Creating camera for preview
	camera = ( le::ICamera* ) EngineAPI::GetInstance()->GetEngine()->GetFactory()->Create( CAMERA_INTERFACE_VERSION );
	if ( !camera )		Error_Critical( "Interface le::ICamera version[" CAMERA_INTERFACE_VERSION "] not found in core" );

	camera->IncrementReference();
	camera->InitProjection_Perspective( 75.f, ( float ) ui->widget_preview->width() / ui->widget_preview->height(), 0.1f, 5500.f );
	scene.SetCamera( camera );

	// Loading sphere for preview
	le::IMesh*			mesh = EngineAPI::GetInstance()->GetResourceSystem()->LoadMesh( "sphere", "models/sphere.mdl" );
	if ( !mesh )		Error_Critical( "Mesh [models/sphere.mdl] not found" );

	std::vector< le::IMaterial* >		updateMaterials( mesh->GetCountMaterials() );
	std::fill( updateMaterials.begin(), updateMaterials.end(), material.GetHandle() );
	mesh->Update( updateMaterials.data(), updateMaterials.size(), 0 );

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

	if ( widget_shaderParameter )
		delete widget_shaderParameter;

	delete ui;
	EngineAPI::GetInstance()->GetResourceSystem()->UnloadAll();
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

			if ( material.HasParameter( selectedShaderDescriptor.parametersInfo[ index ].name ) )
				shaderParameter->setEnabled( false );
		}

	if ( !ui->listWidget_parameters->itemAt( Point ) )		action_delete.setEnabled( false );
	
	connect( &action_delete, SIGNAL( triggered() ), this, SLOT( OnRemoveShaderParameter() ) );

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
// Event: depth test enabled/disabled
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_checkBox_depthTest_stateChanged( int State )
{
	material.EnableDepthTest( ui->checkBox_depthTest->isChecked() );
}

// ------------------------------------------------------------------------------------ //
// Event: depth write enabled/disabled
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_checkBox_depthWrite_stateChanged( int State )
{
	material.EnableDepthWrite( ui->checkBox_depthWrite->isChecked() );
}

// ------------------------------------------------------------------------------------ //
// Event: blend enabled/disabled
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_checkBox_blend_stateChanged( int State )
{
	material.EnableBlend( ui->checkBox_blend->isChecked() );
}

// ------------------------------------------------------------------------------------ //
// Event: change cullface type
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_comboBox_cullfaceType_currentIndexChanged( int Value )
{
	QString					currentText = ui->comboBox_cullfaceType->currentText();	
	le::CULLFACE_TYPE		cullfaceType = le::CT_BACK;
	
	if ( currentText == "Back" )			cullfaceType = le::CT_BACK;
	else if ( currentText == "Front" )		cullfaceType = le::CT_FRONT;

	material.SetCullFaceType( cullfaceType );
}

// ------------------------------------------------------------------------------------ //
// Event: cullface enabled/disabled
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_checkBox_cullface_stateChanged( int State )
{
	material.EnableCullFace( ui->checkBox_cullface->isChecked() );
}

// ------------------------------------------------------------------------------------ //
// Event: add shader parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::OnAddShaderParameter()
{
	QString							selectedShaderParameter = static_cast< QAction* >( sender() )->text();
	le::SHADER_PARAMETER_TYPE		shaderParameterType;

	for ( quint32 index = 0; index < selectedShaderDescriptor.countParameters; ++index )
		if ( selectedShaderParameter == selectedShaderDescriptor.parametersInfo[ index ].name )
		{
			shaderParameterType = selectedShaderDescriptor.parametersInfo[ index ].type;
			break;
		}

	material.AddParameter( selectedShaderParameter, shaderParameterType );
	ui->listWidget_parameters->addItem( selectedShaderParameter );
	ui->listWidget_parameters->setCurrentRow( ui->listWidget_parameters->model()->rowCount() - 1 );
}

// ------------------------------------------------------------------------------------ //
// Event: remove shader parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::OnRemoveShaderParameter()
{
	material.RemoveParameter( ui->listWidget_parameters->currentRow() );
	ui->listWidget_parameters->model()->removeRow( ui->listWidget_parameters->currentRow() );

	if ( widget_shaderParameter )
	{
		delete widget_shaderParameter;
		widget_shaderParameter = nullptr;
	}
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

	material.SetShader( shaderDescriptor.name );
	selectedShaderDescriptor = shaderDescriptor;
	ui->listWidget_parameters->setCurrentRow( -1 );

	// Remove parameters if not exist in new shader
	for ( quint32 index = 0, count = material.GetCountParameters(); index < count; ++index )
	{
		bool							isFind = false;
		ShaderParameterPtr				shaderParameter = material.GetParameter( index );

		for ( quint32 newIndex = 0; newIndex < selectedShaderDescriptor.countParameters; ++newIndex )
			if ( shaderParameter->GetName() == selectedShaderDescriptor.parametersInfo[ newIndex ].name )
			{
				isFind = true;
				break;
			}

		if ( isFind ) continue;
		material.RemoveParameter( index );
		ui->listWidget_parameters->model()->removeRow( index );
		count = material.GetCountParameters();
		--index;
	}

	// Remove selected editor shader parameter
	if ( widget_shaderParameter )
	{
		delete widget_shaderParameter;
		widget_shaderParameter = nullptr;
	}
}

// ------------------------------------------------------------------------------------ //
// Event: selected shader parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_listWidget_parameters_currentRowChanged( int Row )
{
	ShaderParameterPtr				shaderParameter = material.GetParameter( Row );
	if ( !shaderParameter ) return;
	
	if ( widget_shaderParameter )
	{
		delete widget_shaderParameter;
		widget_shaderParameter = nullptr;
	}

	le::SHADER_PARAMETER_TYPE		shaderParameterType;
	for ( quint32 index = 0; index < selectedShaderDescriptor.countParameters; ++index )
		if ( shaderParameter->GetName() == selectedShaderDescriptor.parametersInfo[ index ].name )
		{
			shaderParameterType = selectedShaderDescriptor.parametersInfo[ index ].type;
			break;
		}

	switch ( shaderParameterType )
	{
	case le::SPT_TEXTURE:		widget_shaderParameter = new Widget_ShaderParameter_Texture( shaderParameter );		break;
	case le::SPT_INT:			widget_shaderParameter = new Widget_ShaderParameter_Int( shaderParameter );			break;
	case le::SPT_FLOAT:			widget_shaderParameter = new Widget_ShaderParameter_Float( shaderParameter );		break;
	case le::SPT_SHADER_FLAG:	widget_shaderParameter = new Widget_ShaderParameter_Bool( shaderParameter );		break;
	case le::SPT_VECTOR_4D:		widget_shaderParameter = new Widget_ShaderParameter_Vector4D( shaderParameter );	break;
	case le::SPT_VECTOR_3D:		widget_shaderParameter = new Widget_ShaderParameter_Vector3D( shaderParameter );	break;
	case le::SPT_VECTOR_2D:		widget_shaderParameter = new Widget_ShaderParameter_Vector2D( shaderParameter );	break;
	case le::SPT_COLOR:			widget_shaderParameter = new Widget_ShaderParameter_Color( shaderParameter );		break;
	}

	if ( widget_shaderParameter )
		ui->verticalLayout_2->addWidget( widget_shaderParameter );
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on "New file"
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_actionNew_file_triggered()
{}

// ------------------------------------------------------------------------------------ //
// Event: clicked on "Open file"
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_actionOpen_file_triggered()
{
	QString			path = QFileDialog::getOpenFileName( this, "Open material", EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir, "lifeEngine material (*.lmt)" );
	if ( path.isEmpty() ) return;

	if ( !material.Load( path ) )
	{
		QString			message = "Failed open material [" + path + "]";
		Error_Info( message.toLocal8Bit().data() );
	}
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on "Save file"
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_actionSave_file_triggered()
{
	QString			path = QFileDialog::getSaveFileName( this, "Save material", EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir, "lifeEngine material (*.lmt)" );
	if ( path.isEmpty() ) return;

	if ( !material.Save( path ) )
	{
		QString			message = "Failed save material [" + path + "]";
		Error_Info( message.toLocal8Bit().data() );
	}
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on "Save file as"
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_actionSave_file_as_triggered()
{
	QString			path = QFileDialog::getSaveFileName( this, "Save as material", EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir, "lifeEngine material (*.lmt)" );
	if ( path.isEmpty() ) return;

	if ( !material.Save( path ) )
	{
		QString			message = "Failed save as material [" + path + "]";
		Error_Info( message.toLocal8Bit().data() );
	}
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on "Close file"
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_actionClose_file_triggered()
{
	close();
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on "About Qt"
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_actionAbout_Qt_triggered()
{
	QMessageBox::aboutQt( this, "About Qt" );
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on "About"
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_actionAbout_triggered()
{
	QString				content;
	content = "LMTEditor " + QString::number( LMTEDITOR_VERSION_MAJOR ) + "." + QString::number( LMTEDITOR_VERSION_MINOR ) + "." + QString::number( LMTEDITOR_VERSION_PATCH ) + " (build " + QString::number( Application_ComputeBuildNumber( GOLD_DATE ) ) + ")<br>" +
		"lifeEngine " + LIFEENGINE_VERSION + "<br>" +
		"<br>" +
		"<b>Author:</b> Egor Pogulyaka [zombiHello]<br>" +	
		"<b>Description:</b> This editor is designed to create materials for lifeEngine";
	QMessageBox::about( this, "About LMTEditor", content );
}
