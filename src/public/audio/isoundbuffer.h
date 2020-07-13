//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISOUNDBUFFER_H
#define ISOUNDBUFFER_H

#include "common/types.h"
#include "engine/ireferenceobject.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	enum SAMPLE_FORMAT
	{
		SF_MONO8,
		SF_MONO16,
		SF_STEREO8,
		SF_STEREO16
	};

	//---------------------------------------------------------------------//

	class ISoundBuffer : public IReferenceObject
	{
	public:
		virtual ~ISoundBuffer() {}
		virtual void				Create() = 0;
		virtual void				Append( SAMPLE_FORMAT SampleFormat, const void* Samples, UInt32_t SamplesSize, UInt32_t SampleRate ) = 0;
		virtual void				Delete() = 0;

		virtual UInt32_t			GetCountSamples() const = 0;
		virtual float*				GetSamples() const = 0;
		virtual UInt32_t			GetSampleRate() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define SOUNDBUFFER_INTERFACE_VERSION "LE_SoundBuffer001"

//---------------------------------------------------------------------//

#endif // !ISOUNDBUFFER_H