//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IDIRECTIONALLIGHT_H
#define IDIRECTIONALLIGHT_H

#include "studiorender/ilight.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IDirectionalLight : public ILight
	{
	public:
		virtual ~IDirectionalLight() {}
		virtual void						SetDirection( const Vector3D_t& Direction ) = 0;

		virtual const Vector3D_t&			GetDirection() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define DIRECTIONALLIGHT_INTERFACE_VERSION "LE_DirectionalLight001"

//---------------------------------------------------------------------//

#endif // !IDIRECTIONALLIGHT_H

