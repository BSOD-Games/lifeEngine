//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <qgraphicsproxywidget.h>

#include "node_shader.h"
#include "widget_nodeshader.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Node_Shader::Node_Shader() :
	widget_nodeShader( new Widget_NodeShader() )
{
	connect( widget_nodeShader, &Widget_NodeShader::CountParametersChanged, this, &Node_Shader::OnCountParametersChanged );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Node_Shader::~Node_Shader()
{
	disconnect( widget_nodeShader, &Widget_NodeShader::CountParametersChanged, this, &Node_Shader::OnCountParametersChanged );
}

// ------------------------------------------------------------------------------------ //
// Get caption
// ------------------------------------------------------------------------------------ //
QString Node_Shader::caption() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get name node
// ------------------------------------------------------------------------------------ //
QString Node_Shader::name() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get count ports
// ------------------------------------------------------------------------------------ //
unsigned int Node_Shader::nPorts( QtNodes::PortType PortType ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:		return 1;
	case QtNodes::PortType::In:			return widget_nodeShader->GetCountParameters();
	default:							return 0;
	}
}

// ------------------------------------------------------------------------------------ //
// Get data type
// ------------------------------------------------------------------------------------ //
QtNodes::NodeDataType Node_Shader::dataType( QtNodes::PortType PortType, QtNodes::PortIndex PortIndex ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:	return QtNodes::NodeDataType{ "shader", "Shader" };
	case QtNodes::PortType::In:		return QtNodes::NodeDataType{ "shaderParameter", "Parameter" };
	default:						return QtNodes::NodeDataType();
	}
}

// ------------------------------------------------------------------------------------ //
// Set in data
// ------------------------------------------------------------------------------------ //
void Node_Shader::setInData( std::shared_ptr< QtNodes::NodeData > NodeData, QtNodes::PortIndex Port )
{}

// ------------------------------------------------------------------------------------ //
// Get out data
// ------------------------------------------------------------------------------------ //
std::shared_ptr< QtNodes::NodeData > Node_Shader::outData( QtNodes::PortIndex Port )
{
	return std::shared_ptr<QtNodes::NodeData>();
}

// ------------------------------------------------------------------------------------ //
// Get embedded widget
// ------------------------------------------------------------------------------------ //
QWidget* Node_Shader::embeddedWidget()
{
	return widget_nodeShader;
}

// ------------------------------------------------------------------------------------ //
// Event: count parameters changed
// ------------------------------------------------------------------------------------ //
void Node_Shader::OnCountParametersChanged( quint32 Value )
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
	QPointF		positionWidget = widget_nodeShader->graphicsProxyWidget()->pos();
	positionWidget.setX( node->nodeGeometry().widgetPosition().x() );
	widget_nodeShader->graphicsProxyWidget()->setPos( positionWidget );

	inPort->resize( Value );
	node->nodeGraphicsObject().setGeometryChanged();
	node->onNodeSizeUpdated();
}