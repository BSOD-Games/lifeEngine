//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WINDOW_VIEWER_H
#define WINDOW_VIEWER_H

#include <QMainWindow>

#include "scene.h"
#include "gamedescriptor.h"
#include "mesh.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Window_Viewer;
}

//---------------------------------------------------------------------//

class Window_Viewer : public QMainWindow
{
	Q_OBJECT

public:
	// Window_Viewer
	Window_Viewer( const GameDescriptor& GameDescriptor, QWidget* Parent = nullptr );
	~Window_Viewer();

private slots:
	void			OnResizeViewport( quint32 Width, quint32 Height );
	void			on_actionOpen_triggered();
	void			on_actionSave_triggered();
	void			on_actionSave_As_triggered();
	void			on_listWidget_materials_itemSelectionChanged();
	void			on_toolButton_pathMaterial_clicked();

private:
	void			UpdateCameraPosition();
	void			RemoveAllMaterials();


	Scene								scene;
	Ui::Window_Viewer*					ui;
	le::IModel*							model;
	le::ICamera*						camera;
	le::IDirectionalLight*				directionalLight;
	Mesh								mesh;
};

//---------------------------------------------------------------------//

#endif // WINDOW_VIEWER_H
