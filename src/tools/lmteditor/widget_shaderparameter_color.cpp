//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <qcolordialog.h>
#include <qmessagebox.h>

#include "common/gameinfo.h"
#include "engine/iengine.h"
#include "engine/iresourcesystem.h"

#include "engineapi.h"
#include "errors.h"
#include "shaderparameter.h"
#include "widget_shaderparameter_color.h"
#include "ui_widget_shaderparameter_color.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Color::Widget_ShaderParameter_Color( ShaderParameterPtr ShaderParameter, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ShaderParameter_Color() ),
	shaderParameter( ShaderParameter ),
	color( QColor( 255, 255, 255, 255 ) )
{
	ui->setupUi( this );

	if ( !shaderParameter || !shaderParameter->IsDefined() )
	{
		SetShowColor( color );
		return;
	}
	else
		SetShowColor( shaderParameter->GetValueColor() );
}

// ------------------------------------------------------------------------------------ //
// Set show color
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Color::SetShowColor( const QColor& Value )
{
	int		color_r = Value.red();
	int		color_g = Value.green();
	int		color_b = Value.blue();
	int		color_a = Value.alpha();

	ui->label_color->setStyleSheet( QString( "background-color: rgba(" ) + QString::number( color_r ) + ", " +
							  QString::number( color_g ) + ", " + QString::number( color_b ) + ", " +
							  QString::number( color_a ) + ")" );
	
	shaderParameter->SetValueColor( Value );
	color = Value;
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Color::~Widget_ShaderParameter_Color()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: pushed button "Select color"
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Color::on_toolButton_color_clicked()
{
	if ( !shaderParameter ) return;

	QString			parameterName = shaderParameter->GetName();
	QColor			color = QColorDialog::getColor( this->color, this, "Select color [" + parameterName + "]", QColorDialog::ColorDialogOption::ShowAlphaChannel );
	if ( !color.isValid() ) return;

	SetShowColor( color );
}