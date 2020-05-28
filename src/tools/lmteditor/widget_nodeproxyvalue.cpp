//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/imaterialsystem.h"
#include "engine/imaterialproxyfactory.h"
#include "engine/materialproxydescriptor.h"
#include "engine/materialproxyvarinfo.h"

#include "engineapi.h"
#include "widget_nodeproxyvalue.h"
#include "ui_widget_nodeproxyvalue.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_NodeProxyValue::Widget_NodeProxyValue( QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_NodeProxyValue() )
{
	ui->setupUi( this );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_NodeProxyValue::~Widget_NodeProxyValue()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void Widget_NodeProxyValue::Clear()
{
	ui->comboBox_name->clear();
}

// ------------------------------------------------------------------------------------ //
// Set proxy
// ------------------------------------------------------------------------------------ //
void Widget_NodeProxyValue::SetProxy( QString ProxyName )
{
	le::IMaterialSystem*				materialSystem = EngineAPI::GetInstance()->GetMaterialSystem();
	le::IMaterialProxyFactory*			materialProxyFactory = materialSystem->GetMaterialProxyFactory();

	for ( quint32 index = 0, count = materialProxyFactory->GetCountMaterialProxes(); index < count; ++index )
	{
		le::MaterialProxyDescriptor		materialProxyDescriptor = materialProxyFactory->GetMaterialProxy( index );
		if ( strcmp( materialProxyDescriptor.name, ProxyName.toLocal8Bit().data() ) == 0 )
		{
			Clear();

			for ( quint32 indexPrameter = 0; indexPrameter < materialProxyDescriptor.countParameters; ++indexPrameter )
				ui->comboBox_name->addItem( materialProxyDescriptor.parametersInfo[ indexPrameter ].name );
			return;
		}
	}
}
