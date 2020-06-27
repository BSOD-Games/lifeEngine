//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_PROXYPARAMETER_INT_H
#define WIDGET_PROXYPARAMETER_INT_H

#include <QWidget>

#include "materialproxyparameter.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_ProxyParameter_Int;
}

//---------------------------------------------------------------------//

class Widget_ProxyParameter_Int : public QWidget
{
	Q_OBJECT

public:	
	// Widget_ProxyParameter_Int
	Widget_ProxyParameter_Int( MaterialProxyParameterPtr MaterialProxyParameter, QWidget* Parent = nullptr );
	~Widget_ProxyParameter_Int();

private slots:
	void			on_spinBox_value_valueChanged( int Value );

private:
	void			SetInt( int Value );

	MaterialProxyParameterPtr					proxyParameter;
	Ui::Widget_ProxyParameter_Int*				ui;
};

//---------------------------------------------------------------------//

#endif // WIDGET_PROXYPARAMETER_INT_H
