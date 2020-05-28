//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/materialproxydescriptor.h"
#include "engine/imaterialsystem.h"
#include "engine/imaterialproxyfactory.h"

#include "engineapi.h"
#include "widget_nodeproxy.h"
#include "ui_widget_nodeproxy.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_NodeProxy::Widget_NodeProxy( QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_NodeProxy() )
{
	ui->setupUi( this );

	le::IMaterialProxyFactory*			materialProxyFactory = EngineAPI::GetInstance()->GetMaterialSystem()->GetMaterialProxyFactory();
	le::UInt32_t						countMaterialProxes = materialProxyFactory->GetCountMaterialProxes();
	le::MaterialProxyDescriptor*		materialProxes = materialProxyFactory->GetMaterialProxes();

	for ( le::UInt32_t index = 0; index < countMaterialProxes; ++index )
		ui->comboBox_type->addItem( materialProxes[ index ].name );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_NodeProxy::~Widget_NodeProxy()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: count techniques changed
// ------------------------------------------------------------------------------------ //
void Widget_NodeProxy::on_spinBox_countValues_valueChanged( int Value )
{
	CountValuesChanged( Value );
}

// ------------------------------------------------------------------------------------ //
// Get count techniques
// ------------------------------------------------------------------------------------ //
quint32 Widget_NodeProxy::GetCountValues() const
{
	return ui->spinBox_countValues->value();
}

// ------------------------------------------------------------------------------------ //
// Get proxy name
// ------------------------------------------------------------------------------------ //
QString Widget_NodeProxy::GetProxyName() const
{
	return ui->comboBox_type->currentText();
}

// ------------------------------------------------------------------------------------ //
// Change proxy
// ------------------------------------------------------------------------------------ //
void Widget_NodeProxy::on_comboBox_type_currentIndexChanged( int Index )
{
	emit ProxyChanged( ui->comboBox_type->currentText() );
}
