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
#include "widget_shaderparameter_vector3d.h"
#include "ui_widget_shaderparameter_vector3d.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Vector3D::Widget_ShaderParameter_Vector3D( ShaderParameterPtr ShaderParameter, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ShaderParameter_Vector3D() ),
	shaderParameter( ShaderParameter ),
	value( 0.f, 0.f, 0.f )
{
	ui->setupUi( this );

	if ( !shaderParameter || !shaderParameter->IsDefined() )
	{
		SetVector3D( value );
		return;
	}
	else
		SetVector3D( shaderParameter->GetValueVector3D() );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Vector3D::~Widget_ShaderParameter_Vector3D()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Vector3D::SetVector3D( const le::Vector3D_t& Value )
{
	ui->doubleSpinBox_x->setValue( Value.x );
	ui->doubleSpinBox_y->setValue( Value.y );
	ui->doubleSpinBox_z->setValue( Value.z );

	shaderParameter->SetValueVector3D( Value );
	value = Value;
}

// ------------------------------------------------------------------------------------ //
// Set X
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Vector3D::on_doubleSpinBox_x_valueChanged( double Value )
{
	value.x = Value;
	SetVector3D( value );
}

// ------------------------------------------------------------------------------------ //
// Set Y
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Vector3D::on_doubleSpinBox_y_valueChanged( double Value )
{
	value.y = Value;
	SetVector3D( value ); 
}

// ------------------------------------------------------------------------------------ //
// Set Z
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Vector3D::on_doubleSpinBox_z_valueChanged( double Value )
{
	value.z = Value;
	SetVector3D( value );
}