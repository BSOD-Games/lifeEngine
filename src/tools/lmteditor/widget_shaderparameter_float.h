//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_SHADERPARAMETER_FLOAT_H
#define WIDGET_SHADERPARAMETER_FLOAT_H

#include <QWidget>
#include <QTimer>

#include "engine/ishaderparameter.h"
#include "shaderparameter.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_ShaderParameter_Float;
}

//---------------------------------------------------------------------//

class Widget_ShaderParameter_Float : public QWidget
{
	Q_OBJECT

public:	
	// Widget_ShaderParameter_Float
	Widget_ShaderParameter_Float( ShaderParameterPtr ShaderParameter, QWidget* Parent = nullptr );
	~Widget_ShaderParameter_Float();

private slots:
	void			on_doubleSpinBox_value_valueChanged( double Value );

private:
	void			SetFloat( float Value );

	ShaderParameterPtr							shaderParameter;
	float										value;
	Ui::Widget_ShaderParameter_Float*			ui;
};

//---------------------------------------------------------------------//

#endif // WIDGET_SHADERPARAMETER_FLOAT_H
