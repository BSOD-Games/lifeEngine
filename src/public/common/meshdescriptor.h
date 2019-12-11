//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MESH_DESCRIPTOR_H
#define MESH_DESCRIPTOR_H

#include "common/types.h"
#include "studiorender/studiovertexformat.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct MeshSurface;
	class IMaterial;
	enum PRIMITIVE_TYPE;

	//---------------------------------------------------------------------//

	struct MeshDescriptor
	{
		UInt32_t				sizeVerteces;
		const void*				verteces;
		UInt32_t				countIndeces;
		UInt32_t*				indeces;
		UInt32_t				countMaterials;
		IMaterial**				materials;
		UInt32_t				countSurfaces;
		MeshSurface*			surfaces;
		StudioVertexFormat		vertexFormat;
		PRIMITIVE_TYPE			primitiveType;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MESH_DESCRIPTOR_H

