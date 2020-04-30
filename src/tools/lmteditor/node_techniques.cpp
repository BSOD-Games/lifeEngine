//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <nodes/Node>
#include <nodes/FlowScene>
#include <qgraphicsproxywidget.h>

#include "node_techniques.h"
#include "widget_nodetechniques.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Node_Techniques::Node_Techniques() :
	widget_nodeTechniques( new Widget_NodeTechniques() )
{
	connect( widget_nodeTechniques, &Widget_NodeTechniques::CountTechniquesChanged, this, &Node_Techniques::OnCountTechniquesChanged );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Node_Techniques::~Node_Techniques()
{
	disconnect( widget_nodeTechniques, &Widget_NodeTechniques::CountTechniquesChanged, this, &Node_Techniques::OnCountTechniquesChanged );
}

// ------------------------------------------------------------------------------------ //
// Get caption
// ------------------------------------------------------------------------------------ //
QString Node_Techniques::caption() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get name node
// ------------------------------------------------------------------------------------ //
QString Node_Techniques::name() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get count ports
// ------------------------------------------------------------------------------------ //
unsigned int Node_Techniques::nPorts( QtNodes::PortType PortType ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:		return 1;
	case QtNodes::PortType::In:			return widget_nodeTechniques->GetCountTechniques();
	default:							return 0;
	}
}

// ------------------------------------------------------------------------------------ //
// Get data type
// ------------------------------------------------------------------------------------ //
QtNodes::NodeDataType Node_Techniques::dataType( QtNodes::PortType PortType, QtNodes::PortIndex PortIndex ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:	return QtNodes::NodeDataType{ "techiques", "Techiques" };
	case QtNodes::PortType::In:		return QtNodes::NodeDataType{ "techique", "Techique" };
	default:						return QtNodes::NodeDataType();
	}
}

// ------------------------------------------------------------------------------------ //
// Set in data
// ------------------------------------------------------------------------------------ //
void Node_Techniques::setInData( std::shared_ptr< QtNodes::NodeData > NodeData, QtNodes::PortIndex Port )
{}

// ------------------------------------------------------------------------------------ //
// Get out data
// ------------------------------------------------------------------------------------ //
std::shared_ptr< QtNodes::NodeData > Node_Techniques::outData( QtNodes::PortIndex Port )
{
	return std::shared_ptr<QtNodes::NodeData>();
}

// ------------------------------------------------------------------------------------ //
// Get embedded widget
// ------------------------------------------------------------------------------------ //
QWidget* Node_Techniques::embeddedWidget()
{
	return widget_nodeTechniques;
}

// ------------------------------------------------------------------------------------ //
// Event: count techniques changed
// ------------------------------------------------------------------------------------ //
void Node_Techniques::OnCountTechniquesChanged( quint32 Value )
{
	if ( !node || !scene || inPort->size() == Value ) return;

	// If we deleting any ports
	if ( inPort->size() > Value )
	{
		quint32		countForDeleting = inPort->size() - Value;
		for ( quint32 index = inPort->size() - 1, countDeleted = 0; index >= 0 && countDeleted < countForDeleting; --index, ++countDeleted )
		{
			// Deleting all connections in current port
			auto&			connections = inPort->at( index );
			while ( !connections.empty() )
			{
				auto		itConnection = connections.begin();
				scene->deleteConnection( *itConnection->second );
			}
		}	
	}

	// Updating position widget, size graphicsObject and count ports
	QPointF		positionWidget = widget_nodeTechniques->graphicsProxyWidget()->pos();
	positionWidget.setX( node->nodeGeometry().widgetPosition().x() );
	widget_nodeTechniques->graphicsProxyWidget()->setPos( positionWidget );

	inPort->resize( Value );
	node->nodeGraphicsObject().setGeometryChanged();
	node->onNodeSizeUpdated();
}