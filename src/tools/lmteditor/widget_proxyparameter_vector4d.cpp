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
#include "widget_proxyparameter_vector4d.h"
#include "ui_widget_proxyparameter_vector4d.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ProxyParameter_Vector4D::Widget_ProxyParameter_Vector4D( MaterialProxyParameterPtr MaterialProxyParameter, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ProxyParameter_Vector4D() ),
	proxyParameter( MaterialProxyParameter )
{
	ui->setupUi( this );

	if ( !proxyParameter )
	{
		Error_Critical( "Widget_ProxyParameter_Vector4D::Widget_ProxyParameter_Vector4D( MaterialProxyParameterPtr, QWidget* ) => MaterialProxyParameterPtr is null" );
		return;
	}

	SetVector4D( proxyParameter->GetValueVector4D() );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ProxyParameter_Vector4D::~Widget_ProxyParameter_Vector4D()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Vector4D::SetVector4D( const le::Vector4D_t& Value )
{
	ui->doubleSpinBox_x->setValue( Value.x );
	ui->doubleSpinBox_y->setValue( Value.y );
	ui->doubleSpinBox_z->setValue( Value.z );
	ui->doubleSpinBox_w->setValue( Value.w );

	proxyParameter->SetValueVector4D( Value );
}

// ------------------------------------------------------------------------------------ //
// Set X
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Vector4D::on_doubleSpinBox_x_valueChanged( double Value )
{
	le::Vector4D_t			value = proxyParameter->GetValueVector4D();
	value.x = Value;

	SetVector4D( value );
}

// ------------------------------------------------------------------------------------ //
// Set Y
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Vector4D::on_doubleSpinBox_y_valueChanged( double Value )
{
	le::Vector4D_t			value = proxyParameter->GetValueVector4D();
	value.y = Value;

	SetVector4D( value ); 
}

// ------------------------------------------------------------------------------------ //
// Set Z
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Vector4D::on_doubleSpinBox_z_valueChanged( double Value )
{
	le::Vector4D_t			value = proxyParameter->GetValueVector4D();
	value.z = Value;

	SetVector4D( value );
}

// ------------------------------------------------------------------------------------ //
// Set W
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Vector4D::on_doubleSpinBox_w_valueChanged( double Value )
{
	le::Vector4D_t			value = proxyParameter->GetValueVector4D();
	value.w = Value;

	SetVector4D( value );
}
