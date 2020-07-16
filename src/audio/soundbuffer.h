//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SOUNDBUFFER_H
#define SOUNDBUFFER_H

#include <vector>

#include "audio/isoundbuffer.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class SoundBuffer : public ISoundBuffer
	{
	public:
		// IReferenceObject
		virtual void			IncrementReference();
		virtual void			DecrementReference();
		virtual void			Release();

		virtual UInt32_t		GetCountReferences() const;

		// ISoundBuffer		
		virtual void			Create();
		virtual void			Append( IParserSoundBuffer* ParserSoundBuffer );
		virtual void			Append( SAMPLE_FORMAT SampleFormat, const Byte_t* Samples, UInt32_t SamplesSize, UInt32_t SampleRate );
		virtual void			Delete();
		
		virtual UInt32_t		GetCountSamples() const;
		virtual float*			GetSamples() const;
		virtual UInt32_t		GetSampleRate() const;

		// SoundBuffer
		SoundBuffer();
		~SoundBuffer();

		inline UInt32_t			GetHandle() const
		{
			return handle;
		}

	private:
		UInt32_t				countReferences;
		UInt32_t				handle;
		UInt32_t				sampleRate;

		std::vector< float >	samples;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SOUNDBUFFER_H
