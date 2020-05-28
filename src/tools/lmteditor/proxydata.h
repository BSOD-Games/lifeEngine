//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PROXY_DATA_H
#define PROXY_DATA_H

#include <qstring.h>
#include <nodes/NodeDataModel>

//---------------------------------------------------------------------//

class Node_Proxy;

//---------------------------------------------------------------------//

class ProxyData : public QtNodes::NodeData
{
public:
	friend Node_Proxy;

	// NodeData
	virtual QtNodes::NodeDataType			type() const;

	// ProxyData
	ProxyData();
	ProxyData( const QString& ProxyName );
	~ProxyData();

	inline const QString&			GetProxyName() const { return proxyName; }

private:
	QString				proxyName;
};

//---------------------------------------------------------------------//

#endif // !SHADER_DATA_H
