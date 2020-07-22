//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IAUDIOSYSTEM_H
#define IAUDIOSYSTEM_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IFactory;
	class IListener;

	//---------------------------------------------------------------------//

	class IAudioSystem
	{
	public:
		virtual ~IAudioSystem() {}
		virtual void				Pause() = 0;
		virtual void				UnPause() = 0;
		virtual void				StopAllSounds() = 0;

		virtual IListener*			GetListener() const = 0;
		virtual IFactory*			GetFactory() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IAUDIOSYSTEM_H