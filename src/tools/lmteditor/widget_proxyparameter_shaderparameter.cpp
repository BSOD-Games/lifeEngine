//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <qwidget.h>
#include <qcolordialog.h>
#include <qmessagebox.h>

#include "common/gameinfo.h"
#include "engine/iengine.h"
#include "engine/iresourcesystem.h"

#include "engineapi.h"
#include "errors.h"
#include "window_editor.h"
#include "shaderparameter.h"
#include "widget_proxyparameter_shaderparameter.h"
#include "ui_widget_proxyparameter_shaderparameter.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_ProxyParameter_ShaderParameter::Widget_ProxyParameter_ShaderParameter( MaterialProxyParameterPtr MaterialProxyParameter, const std::vector< ShaderParameterPtr >& ShaderParameters, QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_ProxyParameter_ShaderParameter() ),
	proxyParameter( MaterialProxyParameter ),
	shaderParameters( ShaderParameters )
{
	ui->setupUi( this );

	if ( !proxyParameter )
	{
		Error_Critical( "Widget_ProxyParameter_ShaderParameter::Widget_ProxyParameter_ShaderParameter( MaterialProxyParameterPtr, const std::vector< ShaderParameterPtr >&, QWidget* ) => MaterialProxyParameterPtr is null" );
		return;
	}
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_ProxyParameter_ShaderParameter::~Widget_ProxyParameter_ShaderParameter()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: show widget
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_ShaderParameter::showEvent( QShowEvent* Event )
{
	UpdateComboBoxShaderParameters();
}

// ------------------------------------------------------------------------------------ //
// Update combobox shader parameters
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_ShaderParameter::UpdateComboBoxShaderParameters()
{
	ui->comboBox_shaderParameter->clear();
	if ( shaderParameters.empty() ) return;

	le::IShaderParameter*		shaderParameter = proxyParameter->GetValueShaderParameter();

	for ( quint32 index = 0, count = shaderParameters.size(); index < count; ++index )
		ui->comboBox_shaderParameter->addItem( shaderParameters[ index ]->GetName() );

	if ( shaderParameter )
	{
		bool			isFindParamater = false;
		for ( quint32 index = 0, count = shaderParameters.size(); index < count; ++index )
			if ( shaderParameters[ index ]->GetName() == shaderParameter->GetName() )
			{
				SetShaderParameter( index );
				isFindParamater = true;
				break;
			}

		if ( !isFindParamater )		SetShaderParameter( 0 );
	}
	else if ( !shaderParameters.empty() )
		SetShaderParameter( 0 );
}

// ------------------------------------------------------------------------------------ //
// Set value
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_ShaderParameter::SetShaderParameter( int Index )
{
	if ( Index >= shaderParameters.size() ) return;

	ui->comboBox_shaderParameter->setCurrentIndex( Index );
	proxyParameter->SetValueShaderParameter( shaderParameters[ Index ]->GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set int value
// ------------------------------------------------------------------------------------ //
void Widget_ProxyParameter_ShaderParameter::on_comboBox_shaderParameter_currentIndexChanged( int Value )
{
	SetShaderParameter( Value );
}