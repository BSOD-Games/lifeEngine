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
#include "studiorender/imesh.h"

#include "scene.h"
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

	void			on_listWidget_parameters_itemClicked( QListWidgetItem* Item );
	void			on_listWidget_proxies_itemClicked( QListWidgetItem* Item );
	void			on_listWidget_proxiesParameters_itemClicked( QListWidgetItem* Item );

	void			OnResizeViewport( quint32 Width, quint32 Height );
	void			OnAddShaderParameter();
	void			OnAddProxy();
	void			OnAddProxyParameter();

private:
	Scene								scene;
	le::ICamera*						camera;
	le::IModel*							model;
	le::IPointLight*					pointLight;
	Ui::Window_Editor*					ui;

	le::ShaderDescriptor				selectedShaderDescriptor;
	le::MaterialProxyDescriptor			selectedMaterialProxyDescriptor;
};

//---------------------------------------------------------------------//

#endif // WINDOW_EDITOR_H
