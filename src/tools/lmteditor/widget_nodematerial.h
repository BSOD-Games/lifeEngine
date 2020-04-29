//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_NODEMATERIAL_H
#define WIDGET_NODEMATERIAL_H

#include <qwidget.h>

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_NodeMaterial;
}

//---------------------------------------------------------------------//

class Widget_NodeMaterial : public QWidget
{
	Q_OBJECT

public:
	// Widget_NodeMaterial
	Widget_NodeMaterial( QWidget* Parent = nullptr );
	~Widget_NodeMaterial();

private:
	Ui::Widget_NodeMaterial*			ui;
};

//---------------------------------------------------------------------//

#endif // !WIDGET_NODEMATERIAL_H

