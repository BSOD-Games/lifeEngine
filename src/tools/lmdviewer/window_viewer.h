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

private:
	Scene								scene;
	Ui::Window_Viewer*					ui;
};

//---------------------------------------------------------------------//

#endif // WINDOW_VIEWER_H
