//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADER_DATA_H
#define SHADER_DATA_H

#include <qstring.h>
#include <nodes/NodeDataModel>

//---------------------------------------------------------------------//

class Node_Shader;

//---------------------------------------------------------------------//

class ShaderData : public QtNodes::NodeData
{
public:
	friend Node_Shader;

	// NodeData
	virtual QtNodes::NodeDataType			type() const;

	// ShaderData
	ShaderData();
	ShaderData( const QString& ShaderName );
	~ShaderData();

	inline const QString&			GetShaderName() const { return shaderName; }

private:
	QString				shaderName;
};

//---------------------------------------------------------------------//

#endif // !SHADER_DATA_H
