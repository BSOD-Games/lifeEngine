//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "widget_nodeproxies.h"
#include "ui_widget_nodeproxies.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_NodeProxies::Widget_NodeProxies( QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_NodeProxies() )
{
	ui->setupUi( this );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_NodeProxies::~Widget_NodeProxies()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: count techniques changed
// ------------------------------------------------------------------------------------ //
void Widget_NodeProxies::on_spinBox_count_valueChanged( int Value )
{
	CountProxiesChanged( Value );
}

// ------------------------------------------------------------------------------------ //
// Get count techniques
// ------------------------------------------------------------------------------------ //
quint32 Widget_NodeProxies::GetCountProxies() const
{
	return ui->spinBox_count->value();
}
