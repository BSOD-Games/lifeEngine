//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef TECHNIQUE_DATA_H
#define TECHNIQUE_DATA_H

#include <qtypeinfo.h>
#include <nodes/NodeDataModel>

//---------------------------------------------------------------------//

class Node_Technique;

//---------------------------------------------------------------------//

class TechniqueData : public QtNodes::NodeData
{
public:
	friend Node_Technique;

	// NodeData
	virtual QtNodes::NodeDataType			type() const;

	// TechniqueData
	TechniqueData();
	TechniqueData( quint32 IDTechnique );
	~TechniqueData();

	inline quint32			GetIDTechnique() const		{ return idTechnique; }

private:
	quint32			idTechnique;
};

//---------------------------------------------------------------------//

#endif // !TECHNIQUE_DATA_H

