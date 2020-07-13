//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SOUND_H
#define SOUND_H

#include "audio/isound.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class SoundBuffer;

	//---------------------------------------------------------------------//

	class Sound : public ISound
	{
	public:
		// ISound
		virtual void				Create();
		virtual void				Delete();
		virtual void				Play();
		virtual void				Pause();
		virtual void				Stop();

		virtual void				SetBuffer( ISoundBuffer* SoundBuffer );

		virtual SOUND_STATUS		GetStatus() const;
		virtual ISoundBuffer*		GetBuffer() const;

		// Sound
		Sound();
		~Sound();

	private:
		UInt32_t				handle;
		SoundBuffer*			soundBuffer;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SOUND_H