//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IPOINTLIGHT_H
#define IPOINTLIGHT_H

#include "studiorender/ilight.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IPointLight : public ILight
	{
	public:
		virtual ~IPointLight() {}
		virtual void						SetPosition( const Vector3D_t& Position ) = 0;
		virtual void						SetRadius( float Radius ) = 0;

		virtual const Vector3D_t&			GetPosition() const = 0;
		virtual float						GetRadius() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define POINTLIGHT_INTERFACE_VERSION "LE_PointLight001"

//---------------------------------------------------------------------//

#endif // !IPOINTLIGHT_H

