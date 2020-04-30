//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_NODEPROXIES_H
#define WIDGET_NODEPROXIES_H

#include <qwidget.h>

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_NodeProxies;
}

//---------------------------------------------------------------------//

class Widget_NodeProxies : public QWidget
{
	Q_OBJECT

public:
	// Widget_NodeProxies
	Widget_NodeProxies( QWidget* Parent = nullptr );
	~Widget_NodeProxies();

	quint32			GetCountProxies() const;

signals:
	void			CountProxiesChanged( quint32 Value );

private slots:
	void			on_spinBox_count_valueChanged( int Value );

private:
	Ui::Widget_NodeProxies*			ui;
};

//---------------------------------------------------------------------//

#endif // !WIDGET_NODEPROXIES_H

