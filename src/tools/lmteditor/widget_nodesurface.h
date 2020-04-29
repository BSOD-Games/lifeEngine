//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_NODESURFACE_H
#define WIDGET_NODESURFACE_H

#include <qwidget.h>

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_NodeSurface;
}

//---------------------------------------------------------------------//

class Widget_NodeSurface : public QWidget
{
	Q_OBJECT

public:
	// Widget_NodeSurface
	Widget_NodeSurface( QWidget* Parent = nullptr );
	~Widget_NodeSurface();

private:
	Ui::Widget_NodeSurface*			ui;
};

//---------------------------------------------------------------------//

#endif // !WIDGET_NODESURFACE_H

