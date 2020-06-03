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

	void			OnResizeViewport( quint32 Width, quint32 Height );
	void			OnAddShaderParameter();
	void			OnRemoveShaderParameter();
	void			OnAddProxy();
	void			OnAddProxyParameter();

private:
	Scene								scene;
	Material							material;
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
