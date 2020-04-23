//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SCENE_H
#define SCENE_H

#include "engine/icamera.h"
#include "studiorender/istudiorender.h"
#include "tsingleton.h"

//---------------------------------------------------------------------//

class Scene : public TSingleton<Scene>
{
public:
	// Scene
	Scene();
	~Scene();

	void				Render();

	inline void			SetStudioRender( le::IStudioRender* StudioRender )		{ studioRender = StudioRender; }
	void				SetCamera( le::ICamera* Camera );

private:
	void				Clear();

	le::ICamera*					camera;
	le::IStudioRender*				studioRender;
};

//---------------------------------------------------------------------//

#endif // SCENE_H
