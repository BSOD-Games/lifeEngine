//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WINDOW_EDITOR_H
#define WINDOW_EDITOR_H

#include <QMainWindow>
#include <qstring.h>

#include "engine/shaderdescriptor.h"
#include "engine/materialproxydescriptor.h"
#include "engine/icamera.h"
#include "engine/imodel.h"
#include "engine/imaterial.h"
#include "studiorender/imesh.h"

#include "scene.h"
#include "material.h"
#include "gamedescriptor.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Window_Editor;
}

class QListWidgetItem;
class QFileInfo;

//---------------------------------------------------------------------//

class Window_Editor : public QMainWindow
{
	Q_OBJECT

public:
	// Window_Editor
	Window_Editor( const GameDescriptor& GameDescriptor, QWidget* Parent = nullptr );
	~Window_Editor();

private slots:
	void			on_comboBox_shader_currentIndexChanged( int Value );
	void			on_listWidget_parameters_customContextMenuRequested( QPoint Point );
	void			on_listWidget_proxies_customContextMenuRequested( QPoint Point );
	void			on_listWidget_proxiesParameters_customContextMenuRequested( QPoint Point );
	void			on_checkBox_depthTest_stateChanged( int State );
	void			on_checkBox_depthWrite_stateChanged( int State );
	void			on_checkBox_blend_stateChanged( int State );
	void			on_checkBox_cullface_stateChanged( int State );
	void			on_comboBox_cullfaceType_currentIndexChanged( int Value );

	void			on_listWidget_parameters_currentRowChanged( int Row );
	void			on_listWidget_proxies_currentRowChanged( int Row );
	void			on_lineEdit_surface_textChanged( QString Value );

	void			on_actionNew_file_triggered();
	void			on_actionOpen_file_triggered();
	void			on_actionSave_file_triggered();
	void			on_actionSave_file_as_triggered();
	void			on_actionClose_file_triggered();
	void			on_actionAbout_Qt_triggered();
	void			on_actionAbout_triggered();

	void			OnResizeViewport( quint32 Width, quint32 Height );
	void			OnAddShaderParameter();
	void			OnRemoveShaderParameter();
	void			OnAddProxy();
	void			OnRemoveProxy();
	void			OnAddProxyParameter();
	void			OnRemoveProxyParameter();

private:

	//---------------------------------------------------------------------//

	struct FileInfo
	{
		FileInfo();
		void		Clear();

		bool		isSavedFile;
		QString		path;
		QString		name;

		FileInfo&	operator=( const QFileInfo& FileInfo );
	};

	//---------------------------------------------------------------------//

	void			Clear();
	void			RequestSave();
	void			UpdateWindowTitle();	
	void			AddShaderParameter( const QString& Name, le::SHADER_PARAMETER_TYPE Type );
	void			RemoveShaderParameter( quint32 Index );
	void			RemoveAllShaderParameters();
	void			AddProxy( const QString& Name );
	void			RemoveProxy( quint32 Index );
	void			RemoveAllProxes();
	void			AddProxyParameter( const QString& Name, le::MATERIAL_PROXY_VAR_TYPE Type );
	void			RemoveProxyParameter( quint32 Index );
	void			RemoveAllProxyParameters();
	void			ShowWidgetShaderParameter( ShaderParameterPtr ShaderParameter, le::SHADER_PARAMETER_TYPE Type );
	void			HideWidgetShaderParameter();
	void			OnEditMaterial();

	Scene								scene;
	Material							material;
	FileInfo							fileInfo;
	MaterialProxyPtr					currentMaterialProxy;
	QWidget*							widget_shaderParameter;

	le::ICamera*						camera;
	le::IModel*							model;
	le::IPointLight*					pointLight;
	Ui::Window_Editor*					ui;

	le::ShaderDescriptor				selectedShaderDescriptor;
	le::MaterialProxyDescriptor			selectedMaterialProxyDescriptor;
};

//---------------------------------------------------------------------//

#endif // WINDOW_EDITOR_H
