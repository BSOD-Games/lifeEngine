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
#include "widget_shaderparameter_int.h"
#include "ui_widget_shaderparameter_int.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Int::Widget_ShaderParameter_Int( ShaderParameterPtr ShaderParameter, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ShaderParameter_Int() ),
	shaderParameter( ShaderParameter ),
	value( 0 )
{
	ui->setupUi( this );

	if ( !shaderParameter || !shaderParameter->IsDefined() )
	{
		SetInt( value );
		return;
	}
	else
	{
		switch ( shaderParameter->GetType() )
		{
		case le::SPT_INT:
			SetInt( shaderParameter->GetValueInt() );
			break;

		case le::SPT_FLOAT:
			SetInt( shaderParameter->GetValueFloat() );
			break;

		default:
			Error_Info( "Widget_ShaderParameter_Int::Widget_ShaderParameter_Int( ShaderParameterPtr, QWidget* ) => shader parameter mast be int or float" );
			break;
		}
	}
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Int::~Widget_ShaderParameter_Int()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Int::SetInt( int Value )
{
	ui->spinBox_value->setValue( Value );
	value = Value;

	switch ( shaderParameter->GetType() )
	{
	case le::SPT_INT:
		shaderParameter->SetValueInt( Value );
		break;

	case le::SPT_FLOAT:
		shaderParameter->SetValueFloat( Value );
		break;
	}
}

// ------------------------------------------------------------------------------------ //
// Set int value
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Int::on_spinBox_value_valueChanged( int Value )
{
	SetInt( Value );
}