//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef NODE_SHADER_H
#define NODE_SHADER_H

#include "node_base.h"
#include "shaderdata.h"

//---------------------------------------------------------------------//

class Widget_NodeShader;

//---------------------------------------------------------------------//

class Node_Shader : public Node_Base
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

	// Node_Shader
	Node_Shader();
	~Node_Shader();

	static QString									NodeName() { return "Shader"; }
	inline std::shared_ptr< ShaderData >			Data() const { return data; }

signals:
	void							UpdateShader( QString ShaderName );

private slots:
	void							OnCountParametersChanged( quint32 Value );
	void							OnShaderChanged( QString ShaderName );

private:
	std::shared_ptr< ShaderData >	data;
	Widget_NodeShader*				widget_nodeShader;
};

//---------------------------------------------------------------------//

#endif // !NODE_SHADER_H
