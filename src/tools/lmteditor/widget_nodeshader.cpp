//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/shaderdescriptor.h"
#include "engine/ishaderfactory.h"

#include "engineapi.h"
#include "widget_nodeshader.h"
#include "ui_widget_nodeshader.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_NodeShader::Widget_NodeShader( QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_NodeShader() )
{
	ui->setupUi( this );

	le::IShaderFactory*			shaderFactory = EngineAPI::GetInstance()->GetMaterialSystem()->GetShaderFactory();
	le::UInt32_t				countShaders = shaderFactory->GetCountShaders();
	le::ShaderDescriptor*		shaders = shaderFactory->GetShaders();

	for ( le::UInt32_t index = 0; index < countShaders; ++index )
		ui->comboBox_name->addItem( shaders[ index ].name );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_NodeShader::~Widget_NodeShader()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: count parameters changed
// ------------------------------------------------------------------------------------ //
void Widget_NodeShader::on_spinBox_countParameters_valueChanged( int Value )
{
	CountParametersChanged( Value );
}

// ------------------------------------------------------------------------------------ //
// Get count parameters
// ------------------------------------------------------------------------------------ //
quint32 Widget_NodeShader::GetCountParameters() const
{
	return ui->spinBox_countParameters->value();
}

// ------------------------------------------------------------------------------------ //
// Get shader name
// ------------------------------------------------------------------------------------ //
QString Widget_NodeShader::GetShaderName() const
{
	return ui->comboBox_name->currentText();
}

// ------------------------------------------------------------------------------------ //
// Shader changed
// ------------------------------------------------------------------------------------ //
void Widget_NodeShader::on_comboBox_name_currentIndexChanged( int Index )
{
	emit ShaderChanged( ui->comboBox_name->currentText() );
}
