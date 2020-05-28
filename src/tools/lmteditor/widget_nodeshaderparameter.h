//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_NODESHADERPARAMETER_H
#define WIDGET_NODESHADERPARAMETER_H

#include <qwidget.h>
#include <qstring.h>

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_NodeShaderParameter;
}

//---------------------------------------------------------------------//

class Widget_NodeShaderParameter : public QWidget
{
	Q_OBJECT

public:
	// Widget_NodeShaderParameter
	Widget_NodeShaderParameter( QWidget* Parent = nullptr );
	~Widget_NodeShaderParameter();

	void			Clear();

	void			SetShader( const QString& ShaderName );

private:
	Ui::Widget_NodeShaderParameter*			ui;
};

//---------------------------------------------------------------------//

#endif // !WIDGET_NODESHADERPARAMETER_H

