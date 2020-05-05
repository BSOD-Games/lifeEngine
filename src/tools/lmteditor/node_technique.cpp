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

#include "node_technique.h"
#include "widget_nodetechnique.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Node_Technique::Node_Technique() :
	widget_nodeTechnique( new Widget_NodeTechnique() ),
	data( new TechniqueData() )
{
	connect( widget_nodeTechnique, &Widget_NodeTechnique::CountPassesChanged, this, &Node_Technique::OnCountPassesChanged );
	connect( widget_nodeTechnique, &Widget_NodeTechnique::TypeTechniqueChanged, this, &Node_Technique::OnTypeTechniqueChanged );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Node_Technique::~Node_Technique()
{
	disconnect( widget_nodeTechnique, &Widget_NodeTechnique::CountPassesChanged, this, &Node_Technique::OnCountPassesChanged );
	disconnect( widget_nodeTechnique, &Widget_NodeTechnique::TypeTechniqueChanged, this, &Node_Technique::OnTypeTechniqueChanged );
}

// ------------------------------------------------------------------------------------ //
// Event: type technique changed
// ------------------------------------------------------------------------------------ //
void Node_Technique::OnTypeTechniqueChanged( quint32 Value )
{
	data->idTechnique = Value;
	emit UpdateTypeTechnique( Value );
}

// ------------------------------------------------------------------------------------ //
// Get caption
// ------------------------------------------------------------------------------------ //
QString Node_Technique::caption() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get name node
// ------------------------------------------------------------------------------------ //
QString Node_Technique::name() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get count ports
// ------------------------------------------------------------------------------------ //
unsigned int Node_Technique::nPorts( QtNodes::PortType PortType ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:		return 1;
	case QtNodes::PortType::In:			return widget_nodeTechnique->GetCountPasses();
	default:							return 0;
	}
}

// ------------------------------------------------------------------------------------ //
// Get data type
// ------------------------------------------------------------------------------------ //
QtNodes::NodeDataType Node_Technique::dataType( QtNodes::PortType PortType, QtNodes::PortIndex PortIndex ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:	return TechniqueData().type();
	case QtNodes::PortType::In:		return QtNodes::NodeDataType{ "pass", "Pass" };
	default:						return QtNodes::NodeDataType();
	}
}

// ------------------------------------------------------------------------------------ //
// Set in data
// ------------------------------------------------------------------------------------ //
void Node_Technique::setInData( std::shared_ptr< QtNodes::NodeData > NodeData, QtNodes::PortIndex Port )
{}

// ------------------------------------------------------------------------------------ //
// Get out data
// ------------------------------------------------------------------------------------ //
std::shared_ptr< QtNodes::NodeData > Node_Technique::outData( QtNodes::PortIndex Port )
{
	return data;
}

// ------------------------------------------------------------------------------------ //
// Get embedded widget
// ------------------------------------------------------------------------------------ //
QWidget* Node_Technique::embeddedWidget()
{
	return widget_nodeTechnique;
}

// ------------------------------------------------------------------------------------ //
// Event: count passes changed
// ------------------------------------------------------------------------------------ //
void Node_Technique::OnCountPassesChanged( quint32 Value )
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
	QPointF		positionWidget = widget_nodeTechnique->graphicsProxyWidget()->pos();
	positionWidget.setX( node->nodeGeometry().widgetPosition().x() );
	widget_nodeTechnique->graphicsProxyWidget()->setPos( positionWidget );

	inPort->resize( Value );
	node->nodeGraphicsObject().setGeometryChanged();
	node->onNodeSizeUpdated();
}