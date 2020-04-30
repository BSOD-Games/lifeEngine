//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_NODESHADER_H
#define WIDGET_NODESHADER_H

#include <qwidget.h>

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_NodeShader;
}

//---------------------------------------------------------------------//

class Widget_NodeShader : public QWidget
{
	Q_OBJECT

public:
	// Widget_NodeShader
	Widget_NodeShader( QWidget* Parent = nullptr );
	~Widget_NodeShader();

	quint32					GetCountParameters() const;

signals:
	void					CountParametersChanged( quint32 Value );

private slots:
	void					on_spinBox_countParameters_valueChanged( int Value );

private:
	Ui::Widget_NodeShader*			ui;
};

//---------------------------------------------------------------------//

#endif // !WIDGET_NODESHADER_H

