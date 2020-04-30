//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef NODE_BASE_H
#define NODE_BASE_H

#include <vector>
#include <nodes/Node>
#include <nodes/NodeDataModel>
#include <nodes/NodeState>
#include <nodes/FlowScene>

//---------------------------------------------------------------------//

class Node_Base : public QtNodes::NodeDataModel
{
	Q_OBJECT

public:
	// Node_Base
	Node_Base();
	virtual ~Node_Base();

	inline void			SetScene( QtNodes::FlowScene* Scene )		{ scene = Scene; }
	void				SetNode( QtNodes::Node* Node );

protected:
	QtNodes::FlowScene*											scene;
	QtNodes::Node*												node;

	std::vector< QtNodes::NodeState::ConnectionPtrSet >*		inPort;
	std::vector< QtNodes::NodeState::ConnectionPtrSet >*		outPort;
};

//---------------------------------------------------------------------//

#endif // !NODE_BASE_H

