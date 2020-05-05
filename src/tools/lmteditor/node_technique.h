//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef NODE_TECHNIQUE_H
#define NODE_TECHNIQUE_H

#include "node_base.h"
#include "techniquedata.h"

//---------------------------------------------------------------------//

class Widget_NodeTechnique;

//---------------------------------------------------------------------//

class Node_Technique : public Node_Base
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

	// Node_Technique
	Node_Technique();
	~Node_Technique();

	static inline QString							NodeName()		{ return "Technique"; }
	inline std::shared_ptr< TechniqueData >			Data() const	{ return data; }

signals:
	void							UpdateTypeTechnique( quint32 IDTypeTechnique );

private slots:
	void							OnCountPassesChanged( quint32 Value );
	void							OnTypeTechniqueChanged( quint32 Value );

private:
	std::shared_ptr< TechniqueData >		data;
	Widget_NodeTechnique*					widget_nodeTechnique;
};

//---------------------------------------------------------------------//

#endif // !NODE_TECHNIQUE_H
