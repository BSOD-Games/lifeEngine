//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "common/shaderparaminfo.h"
#include "engine/shaderdescriptor.h"
#include "engine/ishaderfactory.h"

#include "engineapi.h"
#include "widget_nodeshaderparameter.h"
#include "ui_widget_nodeshaderparameter.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_NodeShaderParameter::Widget_NodeShaderParameter( QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_NodeShaderParameter() )
{
	ui->setupUi( this );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_NodeShaderParameter::~Widget_NodeShaderParameter()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Clear widget
// ------------------------------------------------------------------------------------ //
void Widget_NodeShaderParameter::Clear()
{
	ui->comboBox_name->clear();
}

// ------------------------------------------------------------------------------------ //
// Set shader
// ------------------------------------------------------------------------------------ //
void Widget_NodeShaderParameter::SetShader( const QString& ShaderName )
{
	le::IMaterialSystem*		materialSystem = EngineAPI::GetInstance()->GetMaterialSystem();
	le::IShaderFactory*			shaderFactory = materialSystem->GetShaderFactory();
	
	for ( quint32 index = 0, count = shaderFactory->GetCountShaders(); index < count; ++index )
	{
		le::ShaderDescriptor		shaderDescriptor = shaderFactory->GetShader( index );
		if ( strcmp( shaderDescriptor.name, ShaderName.toLocal8Bit().data() ) == 0 )
		{
			Clear();

			for ( quint32 indexPrameter = 0; indexPrameter < shaderDescriptor.countParameters; ++indexPrameter )
				ui->comboBox_name->addItem( shaderDescriptor.parametersInfo[ indexPrameter ].name );
			return;
		}
	}
}
