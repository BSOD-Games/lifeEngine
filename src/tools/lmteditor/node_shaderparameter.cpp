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

#include "node_shaderparameter.h"
#include "node_shader.h"
#include "widget_nodeshaderparameter.h"
#include <qdebug.h>

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Node_ShaderParameter::Node_ShaderParameter() :
	widget_nodeShaderParameter( new Widget_NodeShaderParameter() ),
	nodeShader( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Node_ShaderParameter::~Node_ShaderParameter()
{}

// ------------------------------------------------------------------------------------ //
// Get caption
// ------------------------------------------------------------------------------------ //
QString Node_ShaderParameter::caption() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get name node
// ------------------------------------------------------------------------------------ //
QString Node_ShaderParameter::name() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get count ports
// ------------------------------------------------------------------------------------ //
unsigned int Node_ShaderParameter::nPorts( QtNodes::PortType PortType ) const
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
QtNodes::NodeDataType Node_ShaderParameter::dataType( QtNodes::PortType PortType, QtNodes::PortIndex PortIndex ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:	return QtNodes::NodeDataType{ "shaderParameter", "Parameter" };
	default:						return QtNodes::NodeDataType();
	}
}

// ------------------------------------------------------------------------------------ //
// Set in data
// ------------------------------------------------------------------------------------ //
void Node_ShaderParameter::setInData( std::shared_ptr< QtNodes::NodeData > NodeData, QtNodes::PortIndex Port )
{}

// ------------------------------------------------------------------------------------ //
// Get out data
// ------------------------------------------------------------------------------------ //
std::shared_ptr< QtNodes::NodeData > Node_ShaderParameter::outData( QtNodes::PortIndex Port )
{
	return std::shared_ptr<QtNodes::NodeData>();
}

// ------------------------------------------------------------------------------------ //
// Get embedded widget
// ------------------------------------------------------------------------------------ //
QWidget* Node_ShaderParameter::embeddedWidget()
{
	return widget_nodeShaderParameter;
}

// ------------------------------------------------------------------------------------ //
// Output connection created
// ------------------------------------------------------------------------------------ //
void Node_ShaderParameter::outputConnectionCreated( const QtNodes::Connection& Connection )
{
	nodeShader = static_cast< Node_Shader* >( Connection.getNode( QtNodes::PortType::In )->nodeDataModel() );
	connect( nodeShader, &Node_Shader::UpdateShader, this, &Node_ShaderParameter::OnUpdateShader );

	widget_nodeShaderParameter->SetShader( nodeShader->Data()->GetShaderName() );
}

// ------------------------------------------------------------------------------------ //
// Output connection deleted
// ------------------------------------------------------------------------------------ //
void Node_ShaderParameter::outputConnectionDeleted( const QtNodes::Connection& Connection )
{
	disconnect( nodeShader, &Node_Shader::UpdateShader, this, &Node_ShaderParameter::OnUpdateShader );
	nodeShader = nullptr;

	widget_nodeShaderParameter->Clear();
}

// ------------------------------------------------------------------------------------ //
// Event: update shader
// ------------------------------------------------------------------------------------ //
void Node_ShaderParameter::OnUpdateShader( QString ShaderName )
{
	widget_nodeShaderParameter->SetShader( ShaderName );
}