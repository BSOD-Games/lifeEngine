//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef STUDIORENDER_H
#define STUDIORENDER_H

#include "engine/istudiorenderinternal.h"
#include "rendercontext.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class StudioRender : public IStudioRenderInternal
	{
	public:
		// IStudioRenderInternal
		virtual bool			Initialize( IEngine* Engine );

		// IStudioRender
		virtual void			ResizeViewport( UInt32_t X, UInt32_t Y, UInt32_t Width, UInt32_t Height );
		virtual void			RenderFrame();

		virtual void			SetVerticalSyncEnabled( bool IsEnabled = true );

	private:
		RenderContext			renderContext;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDER_INTERNAL_H
