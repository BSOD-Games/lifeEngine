//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <qdebug.h>

#include "engineapi.h"
#include "widget_nodepass.h"
#include "ui_widget_nodepass.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_NodePass::Widget_NodePass( QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_NodePass() )
{
	ui->setupUi( this );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_NodePass::~Widget_NodePass()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void Widget_NodePass::Clear()
{
	ui->comboBox_type->clear();
	ui->checkBox_blend->setChecked( false );
	ui->checkBox_cullface->setChecked( false );
	ui->checkBox_depthTest->setChecked( false );
	ui->checkBox_depthWrite->setChecked( false );
	ui->comboBox_cullfaceType->setCurrentIndex( 0 );
}

// ------------------------------------------------------------------------------------ //
// Set technique
// ------------------------------------------------------------------------------------ //
void Widget_NodePass::SetTechnique( quint32 IDTechnique )
{
	Clear();

	le::IStudioRenderInfo*					studioRenderInfo = EngineAPI::GetInstance()->GetStudioRenderInfo();
	le::IStudioRenderInfo::PassInfo*		passes = studioRenderInfo->GetPasses( IDTechnique );
	if ( !passes )
	{
		qCritical() << "Failed getting passes info with IDTechnique =" << IDTechnique;
		return;
	}

	for ( quint32 index = 0, count = studioRenderInfo->GetCountPasses( IDTechnique ); index < count; ++index )
		ui->comboBox_type->addItem( passes[ index ].name );
}
