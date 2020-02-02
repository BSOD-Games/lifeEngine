//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISPOTLIGHT_H
#define ISPOTLIGHT_H

#include "studiorender/ilight.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ISpotLight : public ILight
	{
	public:
		virtual ~ISpotLight() {}
		virtual void						SetPosition( const Vector3D_t& Position ) = 0;
		virtual void						SetRotation( const Quaternion_t& Quaternion ) = 0;
		virtual void						SetRadius( float Radius ) = 0;
		virtual void						SetHeight( float Height ) = 0;

		virtual const Vector3D_t&			GetPosition() const = 0;
		virtual const Quaternion_t&			GetRotation() const = 0;
		virtual float						GetRadius() const = 0;
		virtual float						GetHeight() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define SPOTLIGHT_INTERFACE_VERSION "LE_SpotLight001"

//---------------------------------------------------------------------//

#endif // !ISPOTLIGHT_H

