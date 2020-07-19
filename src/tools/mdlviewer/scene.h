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

#include <list>
#include <qtimer.h>

#include "engine/icamera.h"
#include "engine/imodel.h"
#include "studiorender/idirectionallight.h"
#include "studiorender/istudiorender.h"
#include "widget_viewport.h"

#include "tsingleton.h"

//---------------------------------------------------------------------//

class Scene : public QObject, public TSingleton<Scene>
{
	Q_OBJECT

public:
	// Scene
	Scene();
	~Scene();

	void				Render();
	void				Clear();
	void				AddModel( le::IModel* Model );
	void				AddLight( le::IDirectionalLight* Light );

	inline void			SetStudioRender( le::IStudioRender* StudioRender ) { studioRender = StudioRender; }
	void				SetCamera( le::ICamera* Camera );
	void				EnableGround( bool IsEnable );
	void				EnableRotate( bool IsEnable );

	bool				GetIsGround();
	bool				GetIsRotate();

private slots:
	void				Update();

private:
	QTimer									timerUpdate;

	le::ICamera* camera;
	le::IStudioRender* studioRender;
	std::list< le::IModel* >				models;
	std::list< le::IDirectionalLight* >		lights;

	bool									isGround = false;
	bool									isRotate = false;
};

//---------------------------------------------------------------------//

#endif // SCENE_H
