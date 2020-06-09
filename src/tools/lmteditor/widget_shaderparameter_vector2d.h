//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_SHADERPARAMETER_VECTOR2D_H
#define WIDGET_SHADERPARAMETER_VECTOR2D_H

#include <QWidget>
#include <QTimer>

#include "engine/ishaderparameter.h"
#include "shaderparameter.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_ShaderParameter_Vector2D;
}

//---------------------------------------------------------------------//

class Widget_ShaderParameter_Vector2D : public QWidget
{
	Q_OBJECT

public:	
	// Widget_ShaderParameter_Vector2D
	Widget_ShaderParameter_Vector2D( ShaderParameterPtr ShaderParameter, QWidget* Parent = nullptr );
	~Widget_ShaderParameter_Vector2D();

private slots:
	void			on_doubleSpinBox_x_valueChanged( double Value );
	void			on_doubleSpinBox_y_valueChanged( double Value );

private:
	void			SetVector2D( const le::Vector2D_t& Value );

	ShaderParameterPtr									shaderParameter;
	le::Vector2D_t										value;
	Ui::Widget_ShaderParameter_Vector2D*				ui;
};

//---------------------------------------------------------------------//

#endif // WIDGET_SHADERPARAMETER_VECTOR2D_H
