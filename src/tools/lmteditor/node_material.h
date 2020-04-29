//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef NODE_MATERIAL_H
#define NODE_MATERIAL_H

#include <nodes/NodeDataModel>

//---------------------------------------------------------------------//

class Widget_NodeMaterial;

//---------------------------------------------------------------------//

class Node_Material : public QtNodes::NodeDataModel
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

	// Node_Material
	Node_Material();
	~Node_Material();

	static QString					NodeName()		{ return "Material"; }

private:
	Widget_NodeMaterial*				widget_nodematerial;
};

//---------------------------------------------------------------------//

#endif // NODE_MATERIAL_H