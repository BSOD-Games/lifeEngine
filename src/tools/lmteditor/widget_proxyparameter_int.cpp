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
#include "widget_proxyparameter_int.h"
#include "ui_widget_proxyparameter_int.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ProxyParameter_Int::Widget_ProxyParameter_Int( MaterialProxyParameterPtr MaterialProxyParameter, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ProxyParameter_Int() ),
	proxyParameter( MaterialProxyParameter )
{
	ui->setupUi( this );

	if ( !proxyParameter )
	{
		Error_Critical( "Widget_ProxyParameter_Int::Widget_ProxyParameter_Int( MaterialProxyParameterPtr, QWidget* ) => MaterialProxyParameterPtr is null" );
		return;
	}

	switch ( proxyParameter->GetType() )
	{
	case le::MPVT_INT:
		SetInt( proxyParameter->GetValueInt() );
		break;

	case le::MPVT_FLOAT:
		SetInt( proxyParameter->GetValueFloat() );
		break;

	default:
		Error_Info( "Widget_ProxyParameter_Int::Widget_ProxyParameter_Int( MaterialProxyParameterPtr, QWidget* ) => proxy parameter mast be int or float" );
		break;
	}
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ProxyParameter_Int::~Widget_ProxyParameter_Int()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Int::SetInt( int Value )
{
	ui->spinBox_value->setValue( Value );

	switch ( proxyParameter->GetType() )
	{
	case le::MPVT_INT:
		proxyParameter->SetValueInt( Value );
		break;

	case le::MPVT_FLOAT:
		proxyParameter->SetValueFloat( Value );
		break;
	}
}

// ------------------------------------------------------------------------------------ //
// Set int value
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Int::on_spinBox_value_valueChanged( int Value )
{
	SetInt( Value );
}