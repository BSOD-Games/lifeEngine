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
#include "widget_shaderparameter_float.h"
#include "ui_widget_shaderparameter_float.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Float::Widget_ShaderParameter_Float( ShaderParameterPtr ShaderParameter, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ShaderParameter_Float() ),
	shaderParameter( ShaderParameter ),
	value( 0.f )
{
	ui->setupUi( this );

	if ( !shaderParameter || !shaderParameter->IsDefined() )
	{
		SetFloat( value );
		return;
	}
	else
	{
		switch ( shaderParameter->GetType() )
		{
		case le::SPT_INT:
			SetFloat( shaderParameter->GetValueInt() );
			break;

		case le::SPT_FLOAT:
			SetFloat( shaderParameter->GetValueFloat() );
			break;

		default:
			Error_Info( "Widget_ShaderParameter_Float::Widget_ShaderParameter_Float( ShaderParameterPtr, QWidget* ) => shader parameter mast be int or float" );
			break;
		}
	}
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ShaderParameter_Float::~Widget_ShaderParameter_Float()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void Widget_ShaderParameter_Float::SetFloat( float Value )
{
	ui->doubleSpinBox_value->setValue( Value );
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
void Widget_ShaderParameter_Float::on_doubleSpinBox_value_valueChanged( double Value )
{
	SetFloat( Value );
}