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
#include "widget_proxyparameter_bool.h"
#include "ui_widget_proxyparameter_bool.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ProxyParameter_Bool::Widget_ProxyParameter_Bool( MaterialProxyParameterPtr MaterialProxyParameter, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ProxyParameter_Bool() ),
	proxyParameter( MaterialProxyParameter )
{
	ui->setupUi( this );

	if ( !proxyParameter )
	{
		Error_Critical( "Widget_ProxyParameter_Bool::Widget_ProxyParameter_Bool( MaterialProxyParameterPtr, QWidget* ) => MaterialProxyParameterPtr is null" );
		return;
	}

	switch ( proxyParameter->GetType() )
	{
	case le::MPVT_INT:
		SetBool( proxyParameter->GetValueInt() == 0 ? false : true );
		break;

	case le::MPVT_FLOAT:
		SetBool( proxyParameter->GetValueFloat() == 0.f ? false : true );
		break;

	case le::MPVT_BOOL:
		SetBool( proxyParameter->GetValueBool() );
		break;

	default:
		Error_Info( "Widget_ProxyParameter_Bool::Widget_ProxyParameter_Bool( MaterialProxyParameterPtr, QWidget* ) => proxy parameter mast be int or float, or bool" );
		break;
	}
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ProxyParameter_Bool::~Widget_ProxyParameter_Bool()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Bool::SetBool( bool Value )
{
	ui->checkBox_value->setChecked( Value );

	switch ( proxyParameter->GetType() )
	{
	case le::MPVT_INT:
		proxyParameter->SetValueInt( Value ? 1 : 0 );
		break;

	case le::MPVT_FLOAT:
		proxyParameter->SetValueFloat( Value ? 1.f : 0.f );
		break;

	case le::MPVT_BOOL:
		proxyParameter->SetValueBool( Value );
		break;
	}
}

// ------------------------------------------------------------------------------------ //
// Set int value
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_Bool::on_spinBox_value_stateChanged( int State )
{
	SetBool( ui->checkBox_value->isChecked() );
}