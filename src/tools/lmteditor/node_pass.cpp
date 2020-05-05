//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "node_pass.h"
#include "node_technique.h"
#include "widget_nodepass.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Node_Pass::Node_Pass() :
	widget_nodePass( new Widget_NodePass() ),
	nodeTechnique( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Node_Pass::~Node_Pass()
{}

// ------------------------------------------------------------------------------------ //
// Get caption
// ------------------------------------------------------------------------------------ //
QString Node_Pass::caption() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get name node
// ------------------------------------------------------------------------------------ //
QString Node_Pass::name() const
{
	return NodeName();
}

// ------------------------------------------------------------------------------------ //
// Get count ports
// ------------------------------------------------------------------------------------ //
unsigned int Node_Pass::nPorts( QtNodes::PortType PortType ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:		return 1;
	case QtNodes::PortType::In:			return 2;
	default:							return 0;
	}
}

// ------------------------------------------------------------------------------------ //
// Get data type
// ------------------------------------------------------------------------------------ //
QtNodes::NodeDataType Node_Pass::dataType( QtNodes::PortType PortType, QtNodes::PortIndex PortIndex ) const
{
	switch ( PortType )
	{
	case QtNodes::PortType::Out:	return QtNodes::NodeDataType{ "pass", "Pass" };
	case QtNodes::PortType::In:		
		if ( PortIndex == 0 ) 
			return QtNodes::NodeDataType{ "shader", "Shader" };
		else if ( PortIndex == 1 )
			return QtNodes::NodeDataType{ "proxies", "Proxies" };

	default:						return QtNodes::NodeDataType();
	}
}

// ------------------------------------------------------------------------------------ //
// Set in data
// ------------------------------------------------------------------------------------ //
void Node_Pass::setInData( std::shared_ptr< QtNodes::NodeData > NodeData, QtNodes::PortIndex Port )
{}

// ------------------------------------------------------------------------------------ //
// Get out data
// ------------------------------------------------------------------------------------ //
std::shared_ptr< QtNodes::NodeData > Node_Pass::outData( QtNodes::PortIndex Port )
{
	return std::shared_ptr<QtNodes::NodeData>();
}

// ------------------------------------------------------------------------------------ //
// Get embedded widget
// ------------------------------------------------------------------------------------ //
QWidget* Node_Pass::embeddedWidget()
{
	return widget_nodePass;
}

// ------------------------------------------------------------------------------------ //
// Output connection created
// ------------------------------------------------------------------------------------ //
void Node_Pass::outputConnectionCreated( const QtNodes::Connection& Connection )
{
	nodeTechnique = static_cast< Node_Technique* >( Connection.getNode( QtNodes::PortType::In )->nodeDataModel() );
	connect( nodeTechnique, &Node_Technique::UpdateTypeTechnique, this, &Node_Pass::OnUpdateTypeTechnique );

	widget_nodePass->SetTechnique( nodeTechnique->Data()->GetIDTechnique() );
}

// ------------------------------------------------------------------------------------ //
// Output connection deleted
// ------------------------------------------------------------------------------------ //
void Node_Pass::outputConnectionDeleted( const QtNodes::Connection& Connection )
{
	disconnect( nodeTechnique, &Node_Technique::UpdateTypeTechnique, this, &Node_Pass::OnUpdateTypeTechnique );
	nodeTechnique = nullptr;

	widget_nodePass->Clear();
}

// ------------------------------------------------------------------------------------ //
// Event: update type technique
// ------------------------------------------------------------------------------------ //
void Node_Pass::OnUpdateTypeTechnique( quint32 IDTypeTechnique )
{
	widget_nodePass->SetTechnique( IDTypeTechnique );
}