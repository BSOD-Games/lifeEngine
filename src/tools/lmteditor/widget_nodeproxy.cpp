//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "widget_nodeproxy.h"
#include "ui_widget_nodeproxy.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_NodeProxy::Widget_NodeProxy( QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_NodeProxy() )
{
	ui->setupUi( this );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_NodeProxy::~Widget_NodeProxy()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: count techniques changed
// ------------------------------------------------------------------------------------ //
void Widget_NodeProxy::on_spinBox_countValues_valueChanged( int Value )
{
	CountValuesChanged( Value );
}

// ------------------------------------------------------------------------------------ //
// Get count techniques
// ------------------------------------------------------------------------------------ //
quint32 Widget_NodeProxy::GetCountValues() const
{
	return ui->spinBox_countValues->value();
}
