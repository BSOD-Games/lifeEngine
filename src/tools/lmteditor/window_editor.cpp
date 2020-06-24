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
	widget_shaderParameter( nullptr ),
	currentMaterialProxy( nullptr )
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

	if ( model )
	{
		if ( model->GetCountReferences() <= 1 )
			model->Release();
		else
			model->DecrementReference();
	}

	EngineAPI::GetInstance()->GetResourceSystem()->UnloadAll();

	if ( camera )
	{
		if ( camera->GetCountReferences() <= 1 )
			camera->Release();
		else
			camera->DecrementReference();
	}

	if ( pointLight )
	{
		if ( pointLight->GetCountReferences() <= 1 )
			pointLight->Release();
		else
			pointLight->DecrementReference();
	}

	HideWidgetShaderParameter();
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

	if ( selectedMaterialProxyDescriptor.countParameters == 0 || !currentMaterialProxy )		
		menu_add.setEnabled( false );
	else
		for ( quint32 index = 0; index < selectedMaterialProxyDescriptor.countParameters; ++index )
		{
			QAction*			proxyParameter = new QAction( selectedMaterialProxyDescriptor.parametersInfo[ index ].name, this );
			action_addProxyParameters.append( proxyParameter );
			menu_add.addAction( proxyParameter );
			connect( proxyParameter, SIGNAL( triggered() ), this, SLOT( OnAddProxyParameter() ) );

			if ( currentMaterialProxy->HasParameter( selectedMaterialProxyDescriptor.parametersInfo[ index ].name ) )
				proxyParameter->setEnabled( false );
		}

	if ( !ui->listWidget_proxiesParameters->itemAt( Point ) )		action_delete.setEnabled( false );

	connect( &action_delete, SIGNAL( triggered() ), this, SLOT( OnRemoveProxyParameter() ) );

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
	OnEditMaterial();
}

// ------------------------------------------------------------------------------------ //
// Event: depth write enabled/disabled
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_checkBox_depthWrite_stateChanged( int State )
{
	material.EnableDepthWrite( ui->checkBox_depthWrite->isChecked() );
	OnEditMaterial();
}

// ------------------------------------------------------------------------------------ //
// Event: blend enabled/disabled
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_checkBox_blend_stateChanged( int State )
{
	material.EnableBlend( ui->checkBox_blend->isChecked() );
	OnEditMaterial();
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
	OnEditMaterial();
}

// ------------------------------------------------------------------------------------ //
// Event: cullface enabled/disabled
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_checkBox_cullface_stateChanged( int State )
{
	material.EnableCullFace( ui->checkBox_cullface->isChecked() );
	OnEditMaterial();
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

	AddShaderParameter( selectedShaderParameter, shaderParameterType );
}

// ------------------------------------------------------------------------------------ //
// Event: remove shader parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::OnRemoveShaderParameter()
{
	RemoveShaderParameter( ui->listWidget_parameters->currentRow() );
	HideWidgetShaderParameter();
}

// ------------------------------------------------------------------------------------ //
// Event: remove proxy
// ------------------------------------------------------------------------------------ //
void Window_Editor::OnRemoveProxy()
{
	RemoveProxy( ui->listWidget_proxies->currentRow() );
}

// ------------------------------------------------------------------------------------ //
// Event: add proxy parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::OnAddProxyParameter()
{
	if ( !selectedMaterialProxyDescriptor.name ) return;

	QString							selectedProxyParameter = static_cast< QAction* >( sender() )->text();
	le::MATERIAL_PROXY_VAR_TYPE		proxyParameterType;

	for ( quint32 index = 0; index < selectedMaterialProxyDescriptor.countParameters; ++index )
		if ( selectedProxyParameter == selectedMaterialProxyDescriptor.parametersInfo[ index ].name )
		{
			proxyParameterType = selectedMaterialProxyDescriptor.parametersInfo[ index ].type;
			break;
		}

	AddProxyParameter( selectedProxyParameter, proxyParameterType );
}

// ------------------------------------------------------------------------------------ //
// Event: remove proxy parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::OnRemoveProxyParameter()
{
	RemoveProxyParameter( ui->listWidget_proxiesParameters->currentRow() );
}

// ------------------------------------------------------------------------------------ //
// Event: add proxy
// ------------------------------------------------------------------------------------ //
void Window_Editor::OnAddProxy()
{
	QString			selectedProxy = static_cast< QAction* >( sender() )->text();
	AddProxy( selectedProxy );
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

	connect( &action_delete, SIGNAL( triggered() ), this, SLOT( OnRemoveProxy() ) );

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

	OnEditMaterial();
	HideWidgetShaderParameter();
}

// ------------------------------------------------------------------------------------ //
// Event: selected shader parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_listWidget_parameters_currentRowChanged( int Row )
{
	ShaderParameterPtr				shaderParameter = material.GetParameter( Row );
	if ( !shaderParameter ) return;
	
	HideWidgetShaderParameter();

	le::SHADER_PARAMETER_TYPE		shaderParameterType;
	for ( quint32 index = 0; index < selectedShaderDescriptor.countParameters; ++index )
		if ( shaderParameter->GetName() == selectedShaderDescriptor.parametersInfo[ index ].name )
		{
			shaderParameterType = selectedShaderDescriptor.parametersInfo[ index ].type;
			break;
		}

	ShowWidgetShaderParameter( shaderParameter, shaderParameterType );
}

// ------------------------------------------------------------------------------------ //
// Event: selected proxy
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_listWidget_proxies_currentRowChanged( int Row )
{
	// Get current material proxy
	currentMaterialProxy = material.GetProxy( Row );
	if ( !currentMaterialProxy ) return;

	// Get current descriptor for material proxy
	le::IMaterialProxyFactory*			proxyFactory = EngineAPI::GetInstance()->GetMaterialSystem()->GetMaterialProxyFactory();
	le::MaterialProxyDescriptor*		proxyDescriptors = proxyFactory->GetMaterialProxes();
	quint32								countProxyDescriptors = proxyFactory->GetCountMaterialProxes();

	for ( quint32 index = 0; index < countProxyDescriptors; ++index )
		if ( currentMaterialProxy->GetName() == proxyDescriptors[ index ].name )
		{
			selectedMaterialProxyDescriptor = proxyDescriptors[ index ];
			break;
		}

	// Show all parameters in material proxy
	auto&			proxyParameters = currentMaterialProxy->GetParameters();
	ui->listWidget_proxiesParameters->clear();

	for ( quint32 index = 0, count = proxyParameters.size(); index < count; ++index )
		ui->listWidget_proxiesParameters->addItem( proxyParameters[ index ]->GetName() );
}

// ------------------------------------------------------------------------------------ //
// Event: change surface name
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_lineEdit_surface_textChanged( QString Value )
{
	material.SetSurface( Value );
	OnEditMaterial();
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void Window_Editor::Clear()
{
	material.Clear();
	EngineAPI::GetInstance()->GetResourceSystem()->UnloadAll();

	ui->comboBox_shader->setCurrentIndex( 0 );
	material.SetShader( ui->comboBox_shader->currentText() );	

	ui->lineEdit_surface->setText( "" );
	ui->checkBox_blend->setChecked( false );
	ui->checkBox_cullface->setChecked( true );
	ui->checkBox_depthTest->setChecked( true );
	ui->checkBox_depthWrite->setChecked( true );
	ui->comboBox_cullfaceType->setCurrentIndex( 0 );

	RemoveAllShaderParameters();
	RemoveAllProxes();
	RemoveAllProxyParameters();

	fileInfo.Clear();
	HideWidgetShaderParameter();
	UpdateWindowTitle();
}

// ------------------------------------------------------------------------------------ //
// Add shader parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::AddShaderParameter( const QString& Name, le::SHADER_PARAMETER_TYPE Type )
{
	material.AddParameter( Name, Type );
	ui->listWidget_parameters->addItem( Name );
	ui->listWidget_parameters->setCurrentRow( ui->listWidget_parameters->model()->rowCount() - 1 );
	OnEditMaterial();
}

// ------------------------------------------------------------------------------------ //
// Remove shader parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::RemoveShaderParameter( quint32 Index )
{
	material.RemoveParameter( Index );
	ui->listWidget_parameters->model()->removeRow( Index );
	ui->listWidget_parameters->clearSelection();
	ui->listWidget_parameters->setCurrentRow( -1 );
	OnEditMaterial();
}

// ------------------------------------------------------------------------------------ //
// Remove all shader parameters
// ------------------------------------------------------------------------------------ //
void Window_Editor::RemoveAllShaderParameters()
{
	for ( quint32 index = 0, count = ui->listWidget_parameters->count(); index < count; ++index )
		RemoveShaderParameter( 0 );
}

// ------------------------------------------------------------------------------------ //
// Show widget shader parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::ShowWidgetShaderParameter( ShaderParameterPtr ShaderParameter, le::SHADER_PARAMETER_TYPE Type )
{
	switch ( Type )
	{
	case le::SPT_TEXTURE:		widget_shaderParameter = new Widget_ShaderParameter_Texture( ShaderParameter );		break;
	case le::SPT_INT:			widget_shaderParameter = new Widget_ShaderParameter_Int( ShaderParameter );			break;
	case le::SPT_FLOAT:			widget_shaderParameter = new Widget_ShaderParameter_Float( ShaderParameter );		break;
	case le::SPT_SHADER_FLAG:	widget_shaderParameter = new Widget_ShaderParameter_Bool( ShaderParameter );		break;
	case le::SPT_VECTOR_4D:		widget_shaderParameter = new Widget_ShaderParameter_Vector4D( ShaderParameter );	break;
	case le::SPT_VECTOR_3D:		widget_shaderParameter = new Widget_ShaderParameter_Vector3D( ShaderParameter );	break;
	case le::SPT_VECTOR_2D:		widget_shaderParameter = new Widget_ShaderParameter_Vector2D( ShaderParameter );	break;
	case le::SPT_COLOR:			widget_shaderParameter = new Widget_ShaderParameter_Color( ShaderParameter );		break;
	}

	if ( widget_shaderParameter )		ui->verticalLayout_2->addWidget( widget_shaderParameter );
}

// ------------------------------------------------------------------------------------ //
// Hide widget shader parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::HideWidgetShaderParameter()
{
	if ( !widget_shaderParameter ) return;

	delete widget_shaderParameter;
	widget_shaderParameter = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on "New file"
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_actionNew_file_triggered()
{
	// Save file if not saved
	RequestSave();

	// Clear material
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on "Open file"
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_actionOpen_file_triggered()
{
	// Save file if not saved
	RequestSave();

	// Select file for load
	QString			path = QFileDialog::getOpenFileName( this, "Open material", EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir, "lifeEngine material (*.lmt)" );
	if ( path.isEmpty() ) return;

	// If we selecting file for load -> clear current material
	Clear();

	// Load material
	if ( !material.Load( path ) )
	{
		QString			message = "Failed open material [" + path + "]";
		Error_Info( message.toLocal8Bit().data() );
		return;
	}

	// Getting shader in material
	std::string			nameShader = material.GetShader();
	bool				isFindShader = false;

	for ( quint32 index = 0, count = ui->comboBox_shader->count(); index < count; ++index )
		if ( ui->comboBox_shader->itemText( index ) == nameShader.c_str() )
		{
			isFindShader = true;
			ui->comboBox_shader->setCurrentIndex( index );
			break;
		}

	// If shader not found in combobox for shaders -> error
	if ( !isFindShader )
	{
		QString			message = ( "Shader [" + nameShader + "] not found in material system" ).c_str();
		Error_Info( message.toLocal8Bit().data() );
		return;
	}

	// Setting general parameters material
	ui->lineEdit_surface->setText( material.GetSurface() );
	ui->checkBox_blend->setChecked( material.IsEnabledBlend() );
	ui->checkBox_cullface->setChecked( material.IsEnabledCullFace() );
	ui->checkBox_depthTest->setChecked( material.IsEnabledDepthTest() );
	ui->checkBox_depthWrite->setChecked( material.IsEnabledDepthWrite() );

	switch ( material.GetCullFaceType() )
	{
	case le::CT_BACK:			ui->comboBox_cullfaceType->setCurrentIndex( 0 );	break;
	case le::CT_FRONT:			ui->comboBox_cullfaceType->setCurrentIndex( 1 );	break;
	}

	// Show name parameters in material
	auto&		parameters = material.GetParameters();
	for ( quint32 index = 0, count = parameters.size(); index < count; ++index )
		ui->listWidget_parameters->addItem( parameters[ index ]->GetName() );
	
	// Show name proxes in material
	auto&		proxes = material.GetProxes();
	for ( quint32 index = 0, count = proxes.size(); index < count; ++index )
		ui->listWidget_proxies->addItem( proxes[ index ]->GetName() );

	fileInfo = QFileInfo( path );
	UpdateWindowTitle();
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on "Save file"
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_actionSave_file_triggered()
{
	if ( fileInfo.path.isEmpty() )
	{
		on_actionSave_file_as_triggered();
		return;
	}

	if ( !material.Save( fileInfo.path ) )
	{
		QString			message = "Failed save material [" + fileInfo.path + "]";
		Error_Info( message.toLocal8Bit().data() );
		return;
	}

	fileInfo.isSavedFile = true;
	UpdateWindowTitle();
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
		return;
	}

	fileInfo = QFileInfo( path );
	UpdateWindowTitle();
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on "Close file"
// ------------------------------------------------------------------------------------ //
void Window_Editor::on_actionClose_file_triggered()
{
	// Save file if not saved
	RequestSave();

	// Clear material and close window
	Clear();
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

// ------------------------------------------------------------------------------------ //
// Update title window
// ------------------------------------------------------------------------------------ //
void Window_Editor::UpdateWindowTitle()
{
	if ( fileInfo.name.isEmpty() )
	{
		setWindowTitle( "LMTEditor" );
		return;
	}

	setWindowTitle( "LMTEditor [" + fileInfo.name  + "]" );
}

// ------------------------------------------------------------------------------------ //
// Constructor FileInfo
// ------------------------------------------------------------------------------------ //
Window_Editor::FileInfo::FileInfo() :
	isSavedFile( true )
{}

// ------------------------------------------------------------------------------------ //
// Clear file info
// ------------------------------------------------------------------------------------ //
void Window_Editor::FileInfo::Clear()
{
	name = "";
	path = "";
	isSavedFile = true;
}

// ------------------------------------------------------------------------------------ //
// Operator = for FileInfo
// ------------------------------------------------------------------------------------ //
Window_Editor::FileInfo& Window_Editor::FileInfo::operator=( const QFileInfo& FileInfo )
{
	name = FileInfo.fileName();
	path = FileInfo.absoluteFilePath();
	isSavedFile = true;
	
	return *this;
}

// ------------------------------------------------------------------------------------ //
// Request save material
// ------------------------------------------------------------------------------------ //
void Window_Editor::RequestSave()
{
	if ( !fileInfo.isSavedFile )
	{
		QMessageBox::StandardButton			button = ( QMessageBox::StandardButton ) QMessageBox::warning( this, "Warning LMTEditor", "Material not saved. Save?", QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel );
		if ( button == QMessageBox::StandardButton::Ok )
			on_actionSave_file_triggered();
	}
}

// ------------------------------------------------------------------------------------ //
// On edit material
// ------------------------------------------------------------------------------------ //
void Window_Editor::OnEditMaterial()
{
	fileInfo.isSavedFile = false;
}

// ------------------------------------------------------------------------------------ //
// Add proxy
// ------------------------------------------------------------------------------------ //
void Window_Editor::AddProxy( const QString& Name )
{
	material.AddProxy( Name );
	ui->listWidget_proxies->addItem( Name );
	OnEditMaterial();
}

// ------------------------------------------------------------------------------------ //
// Remove proxy
// ------------------------------------------------------------------------------------ //
void Window_Editor::RemoveProxy( quint32 Index )
{
	if ( currentMaterialProxy == material.GetProxy( Index ) )
	{
		RemoveAllProxyParameters();
		currentMaterialProxy = nullptr;
	}

	material.RemoveProxy( Index );
	ui->listWidget_proxies->model()->removeRow( Index );
	ui->listWidget_proxies->clearSelection();
	ui->listWidget_proxies->setCurrentRow( -1 );

	selectedMaterialProxyDescriptor = le::MaterialProxyDescriptor();	
	OnEditMaterial();
}

// ------------------------------------------------------------------------------------ //
// Remove all proxes
// ------------------------------------------------------------------------------------ //
void Window_Editor::RemoveAllProxes()
{
	for ( quint32 index = 0, count = ui->listWidget_proxies->count(); index < count; ++index )
		RemoveProxy( 0 );
}

// ------------------------------------------------------------------------------------ //
// Add proxy parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::AddProxyParameter( const QString& Name, le::MATERIAL_PROXY_VAR_TYPE Type )
{
	if ( !currentMaterialProxy ) return;

	currentMaterialProxy->AddParameter( Name, Type );
	ui->listWidget_proxiesParameters->addItem( Name );
	OnEditMaterial();
}

// ------------------------------------------------------------------------------------ //
// Remove proxy parameter
// ------------------------------------------------------------------------------------ //
void Window_Editor::RemoveProxyParameter( quint32 Index )
{
	if ( !currentMaterialProxy ) return;

	currentMaterialProxy->RemoveParameter( Index );
	ui->listWidget_proxiesParameters->model()->removeRow( Index );
	ui->listWidget_proxiesParameters->clearSelection();
	ui->listWidget_proxiesParameters->setCurrentRow( -1 );
	OnEditMaterial();
}

// ------------------------------------------------------------------------------------ //
// Remove all proxy parameters
// ------------------------------------------------------------------------------------ //
void Window_Editor::RemoveAllProxyParameters()
{
	for ( quint32 index = 0, count = ui->listWidget_proxiesParameters->count(); index < count; ++index )
		RemoveProxyParameter( 0 );
}
