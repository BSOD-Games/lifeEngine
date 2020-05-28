//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef WIDGET_NODEPROXY_H
#define WIDGET_NODEPROXY_H

#include <qwidget.h>

//---------------------------------------------------------------------//

namespace Ui
{
	class Widget_NodeProxy;
}

//---------------------------------------------------------------------//

class Widget_NodeProxy : public QWidget
{
	Q_OBJECT

public:
	// Widget_NodeProxy
	Widget_NodeProxy( QWidget* Parent = nullptr );
	~Widget_NodeProxy();

	quint32			GetCountValues() const;
	QString			GetProxyName() const;

signals:
	void			CountValuesChanged( quint32 Value );
	void			ProxyChanged( QString ProxyName );

private slots:
	void			on_spinBox_countValues_valueChanged( int Value );
	void			on_comboBox_type_currentIndexChanged( int Index );

private:
	Ui::Widget_NodeProxy*			ui;
};

//---------------------------------------------------------------------//

#endif // !WIDGET_NODEPROXY_H

