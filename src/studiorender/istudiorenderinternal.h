//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISTUDIORENDER_INTERNAL_H
#define ISTUDIORENDER_INTERNAL_H

#include "studiorender/istudiorender.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngine;

	//---------------------------------------------------------------------//

	class IStudioRenderInternal : public IStudioRender
	{
	public:
		virtual bool			Initialize( IEngine* Engine ) = 0;
		virtual void			RenderFrame() = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDER_INTERNAL_H
