//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef NODE_SHADERPARAMETER_H
#define NODE_SHADERPARAMETER_H

#include "node_base.h"

//---------------------------------------------------------------------//

class Widget_NodeShaderParameter;
class Node_Shader;

//---------------------------------------------------------------------//

class Node_ShaderParameter : public Node_Base
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

	// Node_ShaderParameter
	Node_ShaderParameter();
	~Node_ShaderParameter();

	static QString					NodeName() { return "Shader parameter"; }

private slots:
	void							OnUpdateShader( QString ShaderName );

private:
	Widget_NodeShaderParameter*		widget_nodeShaderParameter;
	Node_Shader*					nodeShader;
};

//---------------------------------------------------------------------//

#endif // !NODE_TECHNIQUES_H
