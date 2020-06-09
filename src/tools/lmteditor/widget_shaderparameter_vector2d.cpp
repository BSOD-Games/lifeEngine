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
#include "widget_shaderparameter_vector2d.h"
#include "ui_widget_shaderparameter_vector2d.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Vector2D::Widget_ShaderParameter_Vector2D( ShaderParameterPtr ShaderParameter, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ShaderParameter_Vector2D() ),
	shaderParameter( ShaderParameter ),
	value( 0.f, 0.f )
{
	ui->setupUi( this );

	if ( !shaderParameter || !shaderParameter->IsDefined() )
	{
		SetVector2D( value );
		return;
	}
	else
		SetVector2D( shaderParameter->GetValueVector2D() );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Vector2D::~Widget_ShaderParameter_Vector2D()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Vector2D::SetVector2D( const le::Vector2D_t& Value )
{
	ui->doubleSpinBox_x->setValue( Value.x );
	ui->doubleSpinBox_y->setValue( Value.y );

	shaderParameter->SetValueVector2D( Value );
	value = Value;
}

// ------------------------------------------------------------------------------------ //
// Set X
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Vector2D::on_doubleSpinBox_x_valueChanged( double Value )
{
	value.x = Value;
	SetVector2D( value );
}

// ------------------------------------------------------------------------------------ //
// Set Y
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Vector2D::on_doubleSpinBox_y_valueChanged( double Value )
{
	value.y = Value;
	SetVector2D( value ); 
}