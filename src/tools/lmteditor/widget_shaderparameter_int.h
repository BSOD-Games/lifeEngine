//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_SHADERPARAMETER_INT_H
#define WIDGET_SHADERPARAMETER_INT_H

#include <QWidget>
#include <QTimer>

#include "engine/ishaderparameter.h"
#include "shaderparameter.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_ShaderParameter_Int;
}

//---------------------------------------------------------------------//

class Widget_ShaderParameter_Int : public QWidget
{
	Q_OBJECT

public:	
	// Widget_ShaderParameter_Int
	Widget_ShaderParameter_Int( ShaderParameterPtr ShaderParameter, QWidget* Parent = nullptr );
	~Widget_ShaderParameter_Int();

private slots:
	void			on_spinBox_value_valueChanged( int Value );

private:
	void			SetInt( int Value );

	ShaderParameterPtr							shaderParameter;
	int											value;
	Ui::Widget_ShaderParameter_Int*				ui;
};

//---------------------------------------------------------------------//

#endif // WIDGET_SHADERPARAMETER_INT_H
