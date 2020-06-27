//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_PROXYPARAMETER_FLOAT_H
#define WIDGET_PROXYPARAMETER_FLOAT_H

#include <QWidget>

#include "materialproxyparameter.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_ProxyParameter_Float;
}

//---------------------------------------------------------------------//

class Widget_ProxyParameter_Float : public QWidget
{
	Q_OBJECT

public:	
	// Widget_ProxyParameter_Float
	Widget_ProxyParameter_Float( MaterialProxyParameterPtr MaterialProxyParameter, QWidget* Parent = nullptr );
	~Widget_ProxyParameter_Float();

private slots:
	void			on_doubleSpinBox_value_valueChanged( double Value );

private:
	void			SetFloat( float Value );

	MaterialProxyParameterPtr					proxyParameter;
	Ui::Widget_ProxyParameter_Float*			ui;
};

//---------------------------------------------------------------------//

#endif // WIDGET_PROXYPARAMETER_FLOAT_H
