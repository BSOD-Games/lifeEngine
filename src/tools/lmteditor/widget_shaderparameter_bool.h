//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_SHADERPARAMETER_BOOL_H
#define WIDGET_SHADERPARAMETER_BOOL_H

#include <QWidget>
#include <QTimer>

#include "engine/ishaderparameter.h"
#include "shaderparameter.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_ShaderParameter_Bool;
}

//---------------------------------------------------------------------//

class Widget_ShaderParameter_Bool : public QWidget
{
	Q_OBJECT

public:	
	// Widget_ShaderParameter_Bool
	Widget_ShaderParameter_Bool( ShaderParameterPtr ShaderParameter, QWidget* Parent = nullptr );
	~Widget_ShaderParameter_Bool();

private slots:
	void			on_spinBox_value_stateChanged( int State );

private:
	void			SetBool( bool Value );

	ShaderParameterPtr							shaderParameter;
	bool										value;
	Ui::Widget_ShaderParameter_Bool*			ui;
};

//---------------------------------------------------------------------//

#endif // Widget_ShaderParameter_Bool
