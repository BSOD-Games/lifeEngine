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
#include "widget_proxyparameter_vector2d.h"
#include "ui_widget_proxyparameter_vector2d.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ProxyParameter_Vector2D::Widget_ProxyParameter_Vector2D( MaterialProxyParameterPtr MaterialProxyParameter, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ProxyParameter_Vector2D() ),
	proxyParameter( MaterialProxyParameter )
{
	ui->setupUi( this );

	if ( !proxyParameter )
	{
		Error_Critical( "Widget_ProxyParameter_Vector2D::Widget_ProxyParameter_Vector2D( MaterialProxyParameterPtr, QWidget* ) => MaterialProxyParameterPtr is null" );
		return;
	}

	SetVector2D( proxyParameter->GetValueVector2D() );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ProxyParameter_Vector2D::~Widget_ProxyParameter_Vector2D()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Vector2D::SetVector2D( const le::Vector2D_t& Value )
{
	ui->doubleSpinBox_x->setValue( Value.x );
	ui->doubleSpinBox_y->setValue( Value.y );

	proxyParameter->SetValueVector2D( Value );
}

// ------------------------------------------------------------------------------------ //
// Set X
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Vector2D::on_doubleSpinBox_x_valueChanged( double Value )
{
	le::Vector2D_t		value = proxyParameter->GetValueVector2D();
	value.x = Value;

	SetVector2D( value );
}

// ------------------------------------------------------------------------------------ //
// Set Y
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Vector2D::on_doubleSpinBox_y_valueChanged( double Value )
{
	le::Vector2D_t		value = proxyParameter->GetValueVector2D();
	value.y = Value;

	SetVector2D( value ); 
}