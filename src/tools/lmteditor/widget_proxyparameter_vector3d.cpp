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
#include "widget_proxyparameter_vector3d.h"
#include "ui_widget_proxyparameter_vector3d.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ProxyParameter_Vector3D::Widget_ProxyParameter_Vector3D( MaterialProxyParameterPtr MaterialProxyParameter, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ProxyParameter_Vector3D() ),
	proxyParameter( MaterialProxyParameter )
{
	ui->setupUi( this );

	if ( !proxyParameter )
	{
		Error_Critical( "Widget_ProxyParameter_Vector3D::Widget_ProxyParameter_Vector3D( MaterialProxyParameterPtr, QWidget* ) => MaterialProxyParameterPtr is null" );
		return;
	}

	SetVector3D( proxyParameter->GetValueVector3D() );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ProxyParameter_Vector3D::~Widget_ProxyParameter_Vector3D()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Vector3D::SetVector3D( const le::Vector3D_t& Value )
{
	ui->doubleSpinBox_x->setValue( Value.x );
	ui->doubleSpinBox_y->setValue( Value.y );
	ui->doubleSpinBox_z->setValue( Value.z );

	proxyParameter->SetValueVector3D( Value );
}

// ------------------------------------------------------------------------------------ //
// Set X
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Vector3D::on_doubleSpinBox_x_valueChanged( double Value )
{
	le::Vector3D_t			value = proxyParameter->GetValueVector3D();
	value.x = Value;

	SetVector3D( value );
}

// ------------------------------------------------------------------------------------ //
// Set Y
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Vector3D::on_doubleSpinBox_y_valueChanged( double Value )
{
	le::Vector3D_t			value = proxyParameter->GetValueVector3D();
	value.y = Value;

	SetVector3D( value ); 
}

// ------------------------------------------------------------------------------------ //
// Set Z
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Vector3D::on_doubleSpinBox_z_valueChanged( double Value )
{
	le::Vector3D_t			value = proxyParameter->GetValueVector3D();
	value.z = Value;

	SetVector3D( value );
}