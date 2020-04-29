//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "widget_nodematerial.h"
#include "ui_widget_nodematerial.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_NodeMaterial::Widget_NodeMaterial( QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_NodeMaterial() )
{
	ui->setupUi( this );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_NodeMaterial::~Widget_NodeMaterial()
{
	delete ui;
}
