//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ILISTENER_H
#define ILISTENER_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IListener
	{
	public:
		virtual ~IListener() {}
		virtual void				SetGlobalVolume( float Volume ) = 0;
		virtual void				SetPosition( const Vector3D_t& Position ) = 0;
		virtual void				SetDirection( const Vector3D_t& Direction ) = 0;
		virtual void				SetUp( const Vector3D_t& Up ) = 0;

		virtual float				GetGlobalVolume() const = 0;
		virtual Vector3D_t			GetPosition() const = 0;
		virtual Vector3D_t			GetDirection() const = 0;
		virtual Vector3D_t			GetUp() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ILISTENER_H
