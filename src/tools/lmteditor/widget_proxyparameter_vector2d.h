//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_PROXYPARAMETER_VECTOR2D_H
#define WIDGET_PROXYPARAMETER_VECTOR2D_H

#include <QWidget>

#include "materialproxyparameter.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_ProxyParameter_Vector2D;
}

//---------------------------------------------------------------------//

class Widget_ProxyParameter_Vector2D : public QWidget
{
	Q_OBJECT

public:	
	// Widget_ProxyParameter_Vector2D
	Widget_ProxyParameter_Vector2D( MaterialProxyParameterPtr MaterialProxyParameter, QWidget* Parent = nullptr );
	~Widget_ProxyParameter_Vector2D();

private slots:
	void			on_doubleSpinBox_x_valueChanged( double Value );
	void			on_doubleSpinBox_y_valueChanged( double Value );

private:
	void			SetVector2D( const le::Vector2D_t& Value );

	MaterialProxyParameterPtr						proxyParameter;
	Ui::Widget_ProxyParameter_Vector2D*				ui;
};

//---------------------------------------------------------------------//

#endif // Widget_ProxyParameter_Vector2D
