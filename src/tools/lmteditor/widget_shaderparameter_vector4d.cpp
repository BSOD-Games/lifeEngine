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
#include "widget_shaderparameter_vector4d.h"
#include "ui_widget_shaderparameter_vector4d.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Vector4D::Widget_ShaderParameter_Vector4D( ShaderParameterPtr ShaderParameter, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ShaderParameter_Vector4D() ),
	shaderParameter( ShaderParameter ),
	value( 0.f, 0.f, 0.f, 0.f )
{
	ui->setupUi( this );

	if ( !shaderParameter || !shaderParameter->IsDefined() )
	{
		SetVector4D( value );
		return;
	}
	else
		SetVector4D( shaderParameter->GetValueVector4D() );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Vector4D::~Widget_ShaderParameter_Vector4D()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Vector4D::SetVector4D( const le::Vector4D_t& Value )
{
	ui->doubleSpinBox_x->setValue( Value.x );
	ui->doubleSpinBox_y->setValue( Value.y );
	ui->doubleSpinBox_z->setValue( Value.z );
	ui->doubleSpinBox_w->setValue( Value.w );

	shaderParameter->SetValueVector4D( Value );
	value = Value;
}

// ------------------------------------------------------------------------------------ //
// Set X
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Vector4D::on_doubleSpinBox_x_valueChanged( double Value )
{
	value.x = Value;
	SetVector4D( value );
}

// ------------------------------------------------------------------------------------ //
// Set Y
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Vector4D::on_doubleSpinBox_y_valueChanged( double Value )
{
	value.y = Value;
	SetVector4D( value ); 
}

// ------------------------------------------------------------------------------------ //
// Set Z
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Vector4D::on_doubleSpinBox_z_valueChanged( double Value )
{
	value.z = Value;
	SetVector4D( value );
}

// ------------------------------------------------------------------------------------ //
// Set W
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Vector4D::on_doubleSpinBox_w_valueChanged( double Value )
{
	value.w = Value;
	SetVector4D( value );
}
