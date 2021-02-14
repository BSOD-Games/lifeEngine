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
#include "studiorender/imesh.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct MeshSurface;
	struct StudioVertexElement;
	class IMaterial;
	class ITexture;
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
		UInt32_t				countLightmaps;
		ITexture**				lightmaps;
		UInt32_t				countSurfaces;
		MeshSurface*			surfaces;
		UInt32_t				countVertexElements;
		Vector3D_t				min;
		Vector3D_t				max;
		StudioVertexElement*	vertexElements;
		PRIMITIVE_TYPE			primitiveType;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MESH_DESCRIPTOR_H

