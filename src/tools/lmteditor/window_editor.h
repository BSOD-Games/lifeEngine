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

#include "engine/icamera.h"

#include "scene.h"
#include "gamedescriptor.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Window_Editor;
}

//---------------------------------------------------------------------//

class Window_Editor : public QMainWindow
{
	Q_OBJECT

public:
	// Window_Editor
	Window_Editor( const GameDescriptor& GameDescriptor, QWidget* Parent = nullptr );
	~Window_Editor();

private:
	Scene						scene;
	le::ICamera*				camera;
	Ui::Window_Editor*			ui;
};

//---------------------------------------------------------------------//

#endif // WINDOW_EDITOR_H
