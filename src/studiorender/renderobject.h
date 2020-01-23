//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef RENDER_OBJECT_H
#define RENDE_ROBJECT_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class VertexArrayObject;
	class IMaterial;
	class Texture;

	//---------------------------------------------------------------------//

	struct RenderObject
	{
		VertexArrayObject*		vertexArrayObject;
		IMaterial*				material;
		Texture*				lightmap;
		UInt32_t				startVertexIndex;
		UInt32_t				startIndex;
		UInt32_t				countIndeces;
		UInt32_t				primitiveType;
		Matrix4x4_t				transformation;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !RENDE_ROBJECT_H
