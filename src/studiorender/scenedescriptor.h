//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SCENE_DESCRIPTOR_H
#define SCENE_DESCRIPTOR_H

#include <vector>
#include <unordered_map>

#include "openglstate.h"
#include "renderobject.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ICamera;

	//---------------------------------------------------------------------//

	struct SceneDescriptor
	{
		typedef		std::unordered_map< OpenGLState, std::vector< RenderObject >, Hash_OpenGLState >	MapRenderObjects_t;

		ICamera*				camera;
		MapRenderObjects_t		renderObjects;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SCENE_DESCRIPTOR_H
