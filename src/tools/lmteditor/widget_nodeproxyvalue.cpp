//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "widget_nodeproxyvalue.h"
#include "ui_widget_nodeproxyvalue.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_NodeProxyValue::Widget_NodeProxyValue( QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_NodeProxyValue() )
{
	ui->setupUi( this );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_NodeProxyValue::~Widget_NodeProxyValue()
{
	delete ui;
}
