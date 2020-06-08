//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_SHADERPARAMETER_COLOR_H
#define WIDGET_SHADERPARAMETER_COLOR_H

#include <QWidget>
#include <QTimer>

#include "engine/ishaderparameter.h"
#include "shaderparameter.h"

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_ShaderParameter_Color;
}

//---------------------------------------------------------------------//

class Widget_ShaderParameter_Color : public QWidget
{
	Q_OBJECT

public:	
	// Widget_ShaderParameter_Color
	Widget_ShaderParameter_Color( ShaderParameterPtr ShaderParameter, QWidget* Parent = nullptr );
	~Widget_ShaderParameter_Color();

private slots:
	void			on_toolButton_color_clicked();

private:
	void			SetShowColor( const QColor& Value );

	ShaderParameterPtr									shaderParameter;
	QColor												color;
	Ui::Widget_ShaderParameter_Color*					ui;
};

//---------------------------------------------------------------------//

#endif // WIDGET_SHADERPARAMETER_COLOR_H
