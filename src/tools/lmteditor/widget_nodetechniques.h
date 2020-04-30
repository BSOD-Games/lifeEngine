//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_NODETECHNIQUES_H
#define WIDGET_NODETECHNIQUES_H

#include <qwidget.h>

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_NodeTechniques;
}

//---------------------------------------------------------------------//

class Widget_NodeTechniques : public QWidget
{
	Q_OBJECT

public:
	// Widget_NodeTechniques
	Widget_NodeTechniques( QWidget* Parent = nullptr );
	~Widget_NodeTechniques();

	quint32			GetCountTechniques() const;

signals:
	void			CountTechniquesChanged( quint32 Value );

private slots:
	void			on_spinBox_count_valueChanged( int Value );

private:
	Ui::Widget_NodeTechniques*			ui;
};

//---------------------------------------------------------------------//

#endif // !WIDGET_NODETECHNIQUES_H

