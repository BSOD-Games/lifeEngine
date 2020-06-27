//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_PROXYPARAMETER_VECTOR3D_H
#define WIDGET_PROXYPARAMETER_VECTOR3D_H

#include <QWidget>

#include "materialproxyparameter.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_ProxyParameter_Vector3D;
}

//---------------------------------------------------------------------//

class Widget_ProxyParameter_Vector3D : public QWidget
{
	Q_OBJECT

public:	
	// Widget_ProxyParameter_Vector3D
	Widget_ProxyParameter_Vector3D( MaterialProxyParameterPtr MaterialProxyParameter, QWidget* Parent = nullptr );
	~Widget_ProxyParameter_Vector3D();

private slots:
	void			on_doubleSpinBox_x_valueChanged( double Value );
	void			on_doubleSpinBox_y_valueChanged( double Value );
	void			on_doubleSpinBox_z_valueChanged( double Value );

private:
	void			SetVector3D( const le::Vector3D_t& Value );

	MaterialProxyParameterPtr						proxyParameter;
	Ui::Widget_ProxyParameter_Vector3D*				ui;
};

//---------------------------------------------------------------------//

#endif // WIDGET_PROXYPARAMETER_VECTOR3D_H
