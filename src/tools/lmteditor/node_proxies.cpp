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

#include "node_proxies.h"
#include "widget_nodeproxies.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Node_Proxies::Node_Proxies() :
	widget_nodeProxies( new Widget_NodeProxies() )
{
	connect( widget_nodeProxies, &Widget_NodeProxies::CountProxiesChanged, this, &Node_Proxies::OnCountProxiesChanged );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Node_Proxies::~Node_Proxies()
{
	disconnect( widget_nodeProxies, &Widget_NodeProxies::CountProxiesChanged, this, &Node_Proxies::OnCountProxiesChanged );
}

// ------------------------------------------------------------------------------------ //
// Get caption
// ------------------------------------------------------------------------------------ //
QString Node_Proxies::caption() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get name node
// ------------------------------------------------------------------------------------ //
QString Node_Proxies::name() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get count ports
// ------------------------------------------------------------------------------------ //
unsigned int Node_Proxies::nPorts( QtNodes::PortType PortType ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:		return 1;
	case QtNodes::PortType::In:			return widget_nodeProxies->GetCountProxies();
	default:							return 0;
	}
}

// ------------------------------------------------------------------------------------ //
// Get data type
// ------------------------------------------------------------------------------------ //
QtNodes::NodeDataType Node_Proxies::dataType( QtNodes::PortType PortType, QtNodes::PortIndex PortIndex ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:	return QtNodes::NodeDataType{ "proxies", "Proxies" };
	case QtNodes::PortType::In:		return QtNodes::NodeDataType{ "proxy", "Proxy" };
	default:						return QtNodes::NodeDataType();
	}
}

// ------------------------------------------------------------------------------------ //
// Set in data
// ------------------------------------------------------------------------------------ //
void Node_Proxies::setInData( std::shared_ptr< QtNodes::NodeData > NodeData, QtNodes::PortIndex Port )
{}

// ------------------------------------------------------------------------------------ //
// Get out data
// ------------------------------------------------------------------------------------ //
std::shared_ptr< QtNodes::NodeData > Node_Proxies::outData( QtNodes::PortIndex Port )
{
	return std::shared_ptr<QtNodes::NodeData>();
}

// ------------------------------------------------------------------------------------ //
// Get embedded widget
// ------------------------------------------------------------------------------------ //
QWidget* Node_Proxies::embeddedWidget()
{
	return widget_nodeProxies;
}

// ------------------------------------------------------------------------------------ //
// Event: count techniques changed
// ------------------------------------------------------------------------------------ //
void Node_Proxies::OnCountProxiesChanged( quint32 Value )
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
	QPointF		positionWidget = widget_nodeProxies->graphicsProxyWidget()->pos();
	positionWidget.setX( node->nodeGeometry().widgetPosition().x() );
	widget_nodeProxies->graphicsProxyWidget()->setPos( positionWidget );

	inPort->resize( Value );
	node->nodeGraphicsObject().setGeometryChanged();
	node->onNodeSizeUpdated();
}