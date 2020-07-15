//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "engine/lifeengine.h"
#include "audio/iaudiosysteminternal.h"

#include "listener.h"
#include "audiosystemfactory.h"
#include "audiodevice.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class AudioSystem : public IAudioSystemInternal
	{
	public:
		// IAudioSystem
		virtual IListener*				GetListener() const;			
		virtual IFactory*				GetFactory() const;	

		// IAudioSystemInternal
		virtual bool					Initialize( IEngine* Engine );		

		// AudioSystem
		AudioSystem();
		~AudioSystem();

		inline AudioDevice&				GetAudioDevice()		
		{
			return audioDevice;
		}
		inline const AudioDevice&		GetAudioDevice() const
		{
			return audioDevice;
		}

	private:
		AudioDevice				audioDevice;
		Listener				listener;
		AudioSystemFactory		factory;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !AUDIOSYSTEM_H