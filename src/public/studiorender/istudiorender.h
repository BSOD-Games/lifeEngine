//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISTUDIORENDER_H
#define ISTUDIORENDER_H

#include "common/types.h"
#include "engine/lifeengine.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IFactory;
	class ICamera;
	class IMesh;

	//---------------------------------------------------------------------//

	class IStudioRender
	{
	public:
		virtual void			ResizeViewport( UInt32_t X, UInt32_t Y, UInt32_t Width, UInt32_t Height ) = 0;
		virtual void			Draw( IMesh* Mesh, const Matrix4x4_t& Transformation, ICamera* Camera ) = 0;
		
		virtual void			SetVerticalSyncEnabled( bool IsEnabled = true ) = 0;
		virtual IFactory*		GetFactory() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDER_H
