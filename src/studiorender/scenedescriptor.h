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
                Vector3D_t	from;
                Vector3D_t	to;
                Vector3D_t      color;
	};

	//---------------------------------------------------------------------//

        struct Point
        {
            Vector3D_t      position;
            Vector3D_t      color;
        };

        //---------------------------------------------------------------------//

	struct SceneDescriptor
	{
                ICamera*					camera;
		std::vector< RenderObject >			renderObjects;
		std::vector< PointLight* >			pointLights;
		std::vector< SpotLight* >			spotLights;
                std::vector< DirectionalLight* >                directionalLights;

                std::vector< Line >				debugLines;
                std::vector< Point >                            debugPoints;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SCENE_DESCRIPTOR_H
