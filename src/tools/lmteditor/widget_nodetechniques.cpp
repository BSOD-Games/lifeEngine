//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "widget_nodetechniques.h"
#include "ui_widget_nodetechniques.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_NodeTechniques::Widget_NodeTechniques( QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_NodeTechniques() )
{
	ui->setupUi( this );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_NodeTechniques::~Widget_NodeTechniques()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: count techniques changed
// ------------------------------------------------------------------------------------ //
void Widget_NodeTechniques::on_spinBox_count_valueChanged( int Value )
{
	CountTechniquesChanged( Value );
}

// ------------------------------------------------------------------------------------ //
// Get count techniques
// ------------------------------------------------------------------------------------ //
quint32 Widget_NodeTechniques::GetCountTechniques() const
{
	return ui->spinBox_count->value();
}
