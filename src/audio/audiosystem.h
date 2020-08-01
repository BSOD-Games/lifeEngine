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

#include <mutex>
#include <thread>
#include <queue>
#include <unordered_map>

#include "engine/lifeengine.h"
#include "audio/iaudiosysteminternal.h"

#include "listener.h"
#include "audiosystemfactory.h"
#include "audiodevice.h"
#include "audioresource.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class AudioSystem : public IAudioSystemInternal
	{
	public:
		// IAudioSystem
		virtual void					Pause();
		virtual void					UnPause();
		virtual void					StopAllSounds();

		virtual IListener*				GetListener() const;			
		virtual IFactory*				GetFactory() const;	

		// IAudioSystemInternal
		virtual bool					Initialize( IEngine* Engine );		

		// AudioSystem
		AudioSystem();
		~AudioSystem();

		void							SoundPlay( Sound* Sound );
		void							SoundPause( Sound* Sound );
		void							SoundStop( Sound* Sound );
		void							SoundPlay( StreamSound* StreamSound );
		void							SoundPause( StreamSound* StreamSound );
		void							SoundStop( StreamSound* StreamSound );
		inline AudioDevice&				GetAudioDevice()		
		{
			return audioDevice;
		}
		inline const AudioDevice&		GetAudioDevice() const
		{
			return audioDevice;
		}		

	private:

		//---------------------------------------------------------------------//

		enum
		{
			BufferCount = 3
		};

		//---------------------------------------------------------------------//

		void							Update();

		bool											isUpdating;
		UInt32_t										openALSource;
		UInt32_t										openALBuffers[ BufferCount ];
		AudioDevice										audioDevice;
		Listener										listener;
		AudioSystemFactory								factory;
		
		std::mutex										mutexUpdate;
		std::thread*									threadUpdate;
		void*											steamAudioContext;

		std::queue < UInt32_t >							unusedBuffers;
		std::unordered_map< void*, AudioResource >		updateSounds;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !AUDIOSYSTEM_H