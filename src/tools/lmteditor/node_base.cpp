//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "node_base.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Node_Base::Node_Base() :
	node( nullptr ),
	scene( nullptr ),
	inPort( nullptr ),
	outPort( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Node_Base::~Node_Base()
{}

// ------------------------------------------------------------------------------------ //
// Set node
// ------------------------------------------------------------------------------------ //
void Node_Base::SetNode( QtNodes::Node* Node )
{
	node = Node;
	if ( node )
	{
		auto&		nodeState = node->nodeState();
		inPort = &nodeState.getEntries( QtNodes::PortType::In );
		outPort = &nodeState.getEntries( QtNodes::PortType::Out );
	}
	else
	{
		inPort = nullptr;
		outPort = nullptr;
	}
}