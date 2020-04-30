//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_NODEPASS_H
#define WIDGET_NODEPASS_H

#include <qwidget.h>

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_NodePass;
}

//---------------------------------------------------------------------//

class Widget_NodePass : public QWidget
{
	Q_OBJECT

public:
	// Widget_NodePass
	Widget_NodePass( QWidget* Parent = nullptr );
	~Widget_NodePass();

private:
	Ui::Widget_NodePass*			ui;
};

//---------------------------------------------------------------------//

#endif // !WIDGET_NODEPASS_H

