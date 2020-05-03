//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "studiorender/istudiorenderinternal.h"
#include "studiorender/istudiorenderinfo.h"

#include "engineapi.h"
#include "widget_nodetechnique.h"
#include "ui_widget_nodetechnique.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_NodeTechnique::Widget_NodeTechnique( QWidget* Parent ) :
	QWidget( Parent ),
	ui( new Ui::Widget_NodeTechnique() )
{
	ui->setupUi( this );

	le::IStudioRenderInfo*				studioRenderInfo = EngineAPI::GetInstance()->GetStudioRenderInfo();
	le::UInt32_t						count = studioRenderInfo->GetCountTypesTechnique();

	for ( le::UInt32_t index = 0; index < count; ++index )
		ui->comboBox_type->addItem( studioRenderInfo->GetNameTechnique( index ) );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_NodeTechnique::~Widget_NodeTechnique()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: count techniques changed
// ------------------------------------------------------------------------------------ //
void Widget_NodeTechnique::on_spinBox_countPass_valueChanged( int Value )
{
	CountPassesChanged( Value );
}

// ------------------------------------------------------------------------------------ //
// Get count techniques
// ------------------------------------------------------------------------------------ //
quint32 Widget_NodeTechnique::GetCountPasses() const
{
	return ui->spinBox_countPass->value();
}
