//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LISTENER_H
#define LISTENER_H

#include "audio/ilistener.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Listener : public IListener
	{
	public:
		// IListener
		virtual void			SetGlobalVolume( float Volume );
		virtual void			SetPosition( const Vector3D_t& Position );
		virtual void			SetDirection( const Vector3D_t& Direction );
		virtual void			SetUp( const Vector3D_t& Up );

		virtual float			GetGlobalVolume() const;
		virtual Vector3D_t		GetPosition() const;
		virtual Vector3D_t		GetDirection() const;
		virtual Vector3D_t		GetUp() const;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LISTENER_H