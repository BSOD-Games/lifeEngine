//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_PROXYPARAMETER_BOOL_H
#define WIDGET_PROXYPARAMETER_BOOL_H

#include <QWidget>

#include "materialproxyparameter.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_ProxyParameter_Bool;
}

//---------------------------------------------------------------------//

class Widget_ProxyParameter_Bool : public QWidget
{
	Q_OBJECT

public:	
	// Widget_ProxyParameter_Bool
	Widget_ProxyParameter_Bool( MaterialProxyParameterPtr MaterialProxyParameter, QWidget* Parent = nullptr );
	~Widget_ProxyParameter_Bool();

private slots:
	void			on_spinBox_value_stateChanged( int State );

private:
	void			SetBool( bool Value );

	MaterialProxyParameterPtr				proxyParameter;
	Ui::Widget_ProxyParameter_Bool*			ui;
};

//---------------------------------------------------------------------//

#endif // WIDGET_PROXYPARAMETER_BOOL_H
