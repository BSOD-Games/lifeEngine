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
#include "widget_shaderparameter_bool.h"
#include "ui_widget_shaderparameter_bool.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Bool::Widget_ShaderParameter_Bool( ShaderParameterPtr ShaderParameter, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ShaderParameter_Bool() ),
	shaderParameter( ShaderParameter ),
	value( false )
{
	ui->setupUi( this );

	if ( !shaderParameter || !shaderParameter->IsDefined() )
	{
		SetBool( value );
		return;
	}
	else
	{
		switch ( shaderParameter->GetType() )
		{
		case le::SPT_INT:
			SetBool( shaderParameter->GetValueInt() == 0 ? false : true );
			break;

		case le::SPT_FLOAT:
			SetBool( shaderParameter->GetValueFloat() == 0.f ? false : true );
			break;

		case le::SPT_SHADER_FLAG:
			SetBool( shaderParameter->GetValueBool() );
			break;

		default:
			Error_Info( "Widget_ShaderParameter_Bool::Widget_ShaderParameter_Bool( ShaderParameterPtr, QWidget* ) => shader parameter mast be int or float, or bool" );
			break;
		}
	}
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Bool::~Widget_ShaderParameter_Bool()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Bool::SetBool( bool Value )
{
	ui->checkBox_value->setChecked( Value );
	value = Value;

	switch ( shaderParameter->GetType() )
	{
	case le::SPT_INT:
		shaderParameter->SetValueInt( Value ? 1 : 0 );
		break;

	case le::SPT_FLOAT:
		shaderParameter->SetValueFloat( Value ? 1.f : 0.f );
		break;

	case le::SPT_SHADER_FLAG:
		shaderParameter->SetValueBool( Value );
		break;
	}
}

// ------------------------------------------------------------------------------------ //
// Set int value
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Bool::on_spinBox_value_stateChanged( int State )
{
	SetBool( ui->checkBox_value->isChecked() );
}