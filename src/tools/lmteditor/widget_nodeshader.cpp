//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "widget_nodeshader.h"
#include "ui_widget_nodeshader.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_NodeShader::Widget_NodeShader( QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_NodeShader() )
{
	ui->setupUi( this );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_NodeShader::~Widget_NodeShader()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: count parameters changed
// ------------------------------------------------------------------------------------ //
void Widget_NodeShader::on_spinBox_countParameters_valueChanged( int Value )
{
	CountParametersChanged( Value );
}

// ------------------------------------------------------------------------------------ //
// Get count parameters
// ------------------------------------------------------------------------------------ //
quint32 Widget_NodeShader::GetCountParameters() const
{
	return ui->spinBox_countParameters->value();
}