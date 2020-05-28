//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef NODE_PROXYVALUE_H
#define NODE_PROXYVALUE_H

#include "node_base.h"

//---------------------------------------------------------------------//

class Widget_NodeProxyValue;
class Node_Proxy;

//---------------------------------------------------------------------//

class Node_ProxyValue : public Node_Base
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

	virtual void									outputConnectionCreated( const QtNodes::Connection& Connection );
	virtual void									outputConnectionDeleted( const QtNodes::Connection& Connection );

	// Node_ProxyValue
	Node_ProxyValue();
	~Node_ProxyValue();

	static QString					NodeName() { return "Proxy value"; }

private slots:
	void							OnUpdateProxy( QString ProxyName );

private:
	Widget_NodeProxyValue*			widget_nodeProxyValue;
	Node_Proxy*						nodeProxy;
};

//---------------------------------------------------------------------//

#endif // !NODE_PROXYVALUE_H
