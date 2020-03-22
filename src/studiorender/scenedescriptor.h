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

#include "renderobject.h"
#include "pointlight.h"
#include "spotlight.h"
#include "directionallight.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ICamera;

	//---------------------------------------------------------------------//

	struct Line
	{
		Vector3D_t			from;
		Vector3D_t			to;
	};

	//---------------------------------------------------------------------//

	struct SceneDescriptor
	{
		ICamera*							camera;
		std::vector< RenderObject >			renderObjects;
		std::vector< PointLight* >			pointLights;
		std::vector< SpotLight* >			spotLights;
		std::vector< DirectionalLight* >	directionalLights;
		std::vector< Line >					lines;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SCENE_DESCRIPTOR_H
