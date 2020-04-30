//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "widget_nodeshaderparameter.h"
#include "ui_widget_nodeshaderparameter.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_NodeShaderParameter::Widget_NodeShaderParameter( QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_NodeShaderParameter() )
{
	ui->setupUi( this );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_NodeShaderParameter::~Widget_NodeShaderParameter()
{
	delete ui;
}
