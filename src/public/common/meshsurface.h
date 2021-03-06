//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MESH_SURFACE_H
#define MESH_SURFACE_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct MeshSurface
	{
		UInt32_t			materialID;
		UInt32_t			lightmapID;
		UInt32_t			startVertexIndex;
		UInt32_t			startIndex;
		UInt32_t			countIndeces;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MESH_SURFACE_H
