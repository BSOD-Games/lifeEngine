//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef NODE_PROXIES_H
#define NODE_PROXIES_H

#include "node_base.h"

//---------------------------------------------------------------------//

class Widget_NodeProxies;

//---------------------------------------------------------------------//

class Node_Proxies : public Node_Base
{
	Q_OBJECT

public:
	// NodeDataModel
	virtual QString									caption() const;
	virtual QString									name() const;
	virtual unsigned int							nPorts( QtNodes::PortType PortType ) const;
	virtual QtNodes::NodeDataType					dataType( QtNodes::PortType PortType, QtNodes::PortIndex PortIndex ) const;
	virtual void									setInData( std::shared_ptr< QtNodes::NodeData > NodeData, QtNodes::PortIndex Port );
	virtual std::shared_ptr< QtNodes::NodeData >	outData( QtNodes::PortIndex Port );
	virtual QWidget*								embeddedWidget();

	// Node_Proxies
	Node_Proxies();
	~Node_Proxies();

	static QString					NodeName() { return "Proxies"; }

private slots:
	void							OnCountProxiesChanged( quint32 Value );

private:
	Widget_NodeProxies*				widget_nodeProxies;
};

//---------------------------------------------------------------------//

#endif // !NODE_PROXIES_H
