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

#include "engine/iwindow.h"
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
		virtual bool							Initialize( IEngine* Engine ) = 0;
        virtual bool							CreateContext( WindowHandle_t WindowHandle, UInt32_t Width, UInt32_t Height ) = 0;
		virtual void							Begin() = 0;
		virtual void							End() = 0;
		virtual void							Present() = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDER_INTERNAL_H
