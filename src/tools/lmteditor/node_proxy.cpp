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

#include "node_proxy.h"
#include "widget_nodeproxy.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Node_Proxy::Node_Proxy() :
	widget_nodeProxy( new Widget_NodeProxy() ),
	data( new ProxyData() )
{
	connect( widget_nodeProxy, &Widget_NodeProxy::CountValuesChanged, this, &Node_Proxy::OnCountValueChanged );
	connect( widget_nodeProxy, &Widget_NodeProxy::ProxyChanged, this, &Node_Proxy::OnProxyChanged );

	data->proxyName = widget_nodeProxy->GetProxyName();
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Node_Proxy::~Node_Proxy()
{
	disconnect( widget_nodeProxy, &Widget_NodeProxy::CountValuesChanged, this, &Node_Proxy::OnCountValueChanged );
	disconnect( widget_nodeProxy, &Widget_NodeProxy::ProxyChanged, this, &Node_Proxy::OnProxyChanged );
}

// ------------------------------------------------------------------------------------ //
// Proxy changed
// ------------------------------------------------------------------------------------ //
void Node_Proxy::OnProxyChanged( QString ProxyName )
{
	data->proxyName = ProxyName;
	emit UpdateProxy( ProxyName );
}

// ------------------------------------------------------------------------------------ //
// Get caption
// ------------------------------------------------------------------------------------ //
QString Node_Proxy::caption() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get name node
// ------------------------------------------------------------------------------------ //
QString Node_Proxy::name() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get count ports
// ------------------------------------------------------------------------------------ //
unsigned int Node_Proxy::nPorts( QtNodes::PortType PortType ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:		return 1;
	case QtNodes::PortType::In:			return widget_nodeProxy->GetCountValues();
	default:							return 0;
	}
}

// ------------------------------------------------------------------------------------ //
// Get data type
// ------------------------------------------------------------------------------------ //
QtNodes::NodeDataType Node_Proxy::dataType( QtNodes::PortType PortType, QtNodes::PortIndex PortIndex ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:	return data->type();
	case QtNodes::PortType::In:		return QtNodes::NodeDataType{ "proxyValue", "Value" };
	default:						return QtNodes::NodeDataType();
	}
}

// ------------------------------------------------------------------------------------ //
// Set in data
// ------------------------------------------------------------------------------------ //
void Node_Proxy::setInData( std::shared_ptr< QtNodes::NodeData > NodeData, QtNodes::PortIndex Port )
{}

// ------------------------------------------------------------------------------------ //
// Get out data
// ------------------------------------------------------------------------------------ //
std::shared_ptr< QtNodes::NodeData > Node_Proxy::outData( QtNodes::PortIndex Port )
{
	return std::shared_ptr<QtNodes::NodeData>();
}

// ------------------------------------------------------------------------------------ //
// Get embedded widget
// ------------------------------------------------------------------------------------ //
QWidget* Node_Proxy::embeddedWidget()
{
	return widget_nodeProxy;
}

// ------------------------------------------------------------------------------------ //
// Event: count techniques changed
// ------------------------------------------------------------------------------------ //
void Node_Proxy::OnCountValueChanged( quint32 Value )
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
	QPointF		positionWidget = widget_nodeProxy->graphicsProxyWidget()->pos();
	positionWidget.setX( node->nodeGeometry().widgetPosition().x() );
	widget_nodeProxy->graphicsProxyWidget()->setPos( positionWidget );

	inPort->resize( Value );
	node->nodeGraphicsObject().setGeometryChanged();
	node->onNodeSizeUpdated();
}