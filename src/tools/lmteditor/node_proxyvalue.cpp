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
#include "node_proxyvalue.h"
#include "widget_nodeproxyvalue.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Node_ProxyValue::Node_ProxyValue() :
	widget_nodeProxyValue( new Widget_NodeProxyValue() ),
	nodeProxy( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Node_ProxyValue::~Node_ProxyValue()
{}

// ------------------------------------------------------------------------------------ //
// Get caption
// ------------------------------------------------------------------------------------ //
QString Node_ProxyValue::caption() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get name node
// ------------------------------------------------------------------------------------ //
QString Node_ProxyValue::name() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get count ports
// ------------------------------------------------------------------------------------ //
unsigned int Node_ProxyValue::nPorts( QtNodes::PortType PortType ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:		return 1;
	default:							return 0;
	}
}

// ------------------------------------------------------------------------------------ //
// Get data type
// ------------------------------------------------------------------------------------ //
QtNodes::NodeDataType Node_ProxyValue::dataType( QtNodes::PortType PortType, QtNodes::PortIndex PortIndex ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:	return QtNodes::NodeDataType{ "proxyValue", "Value" };
	default:						return QtNodes::NodeDataType();
	}
}

// ------------------------------------------------------------------------------------ //
// Set in data
// ------------------------------------------------------------------------------------ //
void Node_ProxyValue::setInData( std::shared_ptr< QtNodes::NodeData > NodeData, QtNodes::PortIndex Port )
{}

// ------------------------------------------------------------------------------------ //
// Get out data
// ------------------------------------------------------------------------------------ //
std::shared_ptr< QtNodes::NodeData > Node_ProxyValue::outData( QtNodes::PortIndex Port )
{
	return std::shared_ptr<QtNodes::NodeData>();
}

// ------------------------------------------------------------------------------------ //
// Get embedded widget
// ------------------------------------------------------------------------------------ //
QWidget* Node_ProxyValue::embeddedWidget()
{
	return widget_nodeProxyValue;
}

// ------------------------------------------------------------------------------------ //
// Output connection created
// ------------------------------------------------------------------------------------ //
void Node_ProxyValue::outputConnectionCreated( const QtNodes::Connection& Connection )
{
	nodeProxy = static_cast< Node_Proxy* >( Connection.getNode( QtNodes::PortType::In )->nodeDataModel() );
	connect( nodeProxy, &Node_Proxy::UpdateProxy, this, &Node_ProxyValue::OnUpdateProxy );

	widget_nodeProxyValue->SetProxy( nodeProxy->Data()->GetProxyName() );
}

// ------------------------------------------------------------------------------------ //
// Output connection deleted
// ------------------------------------------------------------------------------------ //
void Node_ProxyValue::outputConnectionDeleted( const QtNodes::Connection& Connection )
{
	disconnect( nodeProxy, &Node_Proxy::UpdateProxy, this, &Node_ProxyValue::OnUpdateProxy );
	nodeProxy = nullptr;

	widget_nodeProxyValue->Clear();
}

// ------------------------------------------------------------------------------------ //
// Event: update shader
// ------------------------------------------------------------------------------------ //
void Node_ProxyValue::OnUpdateProxy( QString ProxyName )
{
	widget_nodeProxyValue->SetProxy( ProxyName );
}