//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <string>

#include "common/types.h"
#include "soundbuffer.h"

//---------------------------------------------------------------------//

struct ALCdevice;
struct ALCcontext;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class AudioDevice
	{
	public:
		AudioDevice();
		~AudioDevice();

		bool					Create();
		void					Destroy();

		void					SetGlobalVolume( float Volume );
		void					SetListenerPosition( const Vector3D_t& Position );
		void					SetListenerDirection( const Vector3D_t& Direction );
		void					SetListenerUp( const Vector3D_t& Up );

		static UInt32_t			GetSampleFormat( SAMPLE_FORMAT SampleFormat );
		float					GetGlobalVolume() const;
		const Vector3D_t&		GetListenerPosition() const;
		const Vector3D_t&		GetListenerDirection() const;
		const Vector3D_t&		GetListenerUp() const;

	private:
		bool					IsExtensionSupported( const std::string& Extension );

		float				listenerVolume;
		ALCdevice*			audioDevice;
		ALCcontext*			audioContext;
		Vector3D_t			listenerPosition;
		Vector3D_t			listenerDirection;
		Vector3D_t			listenerUp;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !AUDIODEVICE_H