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
#include <qevent.h>

#include "scene.h"
#include "gamedescriptor.h"
#include "model.h"

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
	void			on_actionImport_triggered();
	void			on_actionConvert_to_PHY_triggered();
	void			on_listWidget_materials_itemSelectionChanged();
	void			on_toolButton_pathMaterial_clicked();
	void			on_checkBox_wireframe_clicked();
	void			on_checkBox_ground_clicked();
	void			OnMouseMove( quint32 PositionX, quint32 PositionY );

private:
	void			UpdateCameraPosition();
	void			RemoveAllMaterials();
	void			closeEvent( QCloseEvent* Event );


	Ui::Window_Viewer*					ui;
	le::ICamera*						camera;
	le::IDirectionalLight*				directionalLight;
	Model								mesh;
	QString								title;

};

//---------------------------------------------------------------------//

#endif // WINDOW_VIEWER_H
