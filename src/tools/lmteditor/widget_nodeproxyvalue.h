//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_NODEPROXYVALUE_H
#define WIDGET_NODEPROXYVALUE_H

#include <qwidget.h>

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_NodeProxyValue;
}

//---------------------------------------------------------------------//

class Widget_NodeProxyValue : public QWidget
{
	Q_OBJECT

public:
	// Widget_NodeProxyValue
	Widget_NodeProxyValue( QWidget* Parent = nullptr );
	~Widget_NodeProxyValue();

	void			Clear();

	void			SetProxy( QString ProxyName );

private:
	Ui::Widget_NodeProxyValue*			ui;
};

//---------------------------------------------------------------------//

#endif // !WIDGET_NODEPROXYVALUE_H

