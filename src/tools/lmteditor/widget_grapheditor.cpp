//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <qpoint.h>
#include <qdebug.h>
#include <nodes\Node>
#include <qdebug.h>

#include "widget_grapheditor.h"
#include "node_base.h"
#include "node_material.h"
#include "node_surface.h"
#include "node_techniques.h"

// ------------------------------------------------------------------------------------ //
// Register data models
// ------------------------------------------------------------------------------------ //
std::shared_ptr<QtNodes::DataModelRegistry> Widget_GraphEditor::RegisterDataModels()
{
	auto			dataModels = std::make_shared< QtNodes::DataModelRegistry >();
	dataModels->registerModel< Node_Material >( "Nodes" );
	dataModels->registerModel< Node_Surface >( "Nodes" );
	dataModels->registerModel< Node_Techniques >( "Nodes" );
	return dataModels;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Widget_GraphEditor::Widget_GraphEditor( QWidget* Parent ) :
	QWidget( Parent ),
	vboxLayout( new QVBoxLayout( this ) ),
	flowScene( new QtNodes::FlowScene( RegisterDataModels() ) ),
	flowView( new QtNodes::FlowView( flowScene ) ),
	isNodeMaterialExist( false )
{
	connect( flowScene, Q_SIGNAL( &QtNodes::FlowScene::nodeCreated ), this, Q_SLOT( &Widget_GraphEditor::NodeCreated ) );

	auto		type = flowScene->registry().create( Node_Material::NodeName() );
	if ( type )
	{
		auto&			node = flowScene->createNode( std::move( type ) );		
		node.nodeGraphicsObject().setPos( QPoint( flowView->width() / 2, flowView->height() / 2 ) );
		NodeCreated( node );
		flowScene->nodePlaced( node );
	}
	
	vboxLayout->addWidget( flowView ); 
	vboxLayout->setContentsMargins( 0, 0, 0, 0 );
	vboxLayout->setSpacing( 0 );	
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Widget_GraphEditor::~Widget_GraphEditor()
{
	disconnect( flowScene, Q_SIGNAL( &QtNodes::FlowScene::nodeCreated ), this, Q_SLOT( &Widget_GraphEditor::NodeCreated ) );
	
	delete flowView;
	delete flowScene;
	delete vboxLayout;
}

// ------------------------------------------------------------------------------------ //
// Event: node created
// ------------------------------------------------------------------------------------ //
void Widget_GraphEditor::NodeCreated( QtNodes::Node& Node )
{
	Node_Base*			node = static_cast< Node_Base* >( Node.nodeDataModel() );
	node->SetNode( &Node );
	node->SetScene( flowScene );
}