//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef AUDIORESOURCE_H
#define AUDIORESOURCE_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Sound;
	class StreamSound;

	struct Chunk
	{
		const float*	samples;
		UInt32_t		sampleCount;
	};

	//---------------------------------------------------------------------//

	class AudioResource
	{
	public:	
		AudioResource();
		AudioResource( AudioResource& Copy );
		~AudioResource();

		void			Clear();
		void			Pause();
		void			UnPause();

		void			SetSoundSource( Sound* Sound );
		void			SetSoundSource( StreamSound* StreamSound );
		void			SetLoop( bool IsLooped = true );
		void			SetSamplesOffset( UInt64_t SamplesOffset );

		bool			IsLooped() const;
		bool			IsPause() const;
		UInt64_t		GetSamplesOffset() const;
		bool			GetData( Chunk& Chunk );

	private:

		//---------------------------------------------------------------------//

		enum SOUND_SOURCE_TYPE
		{
			SST_NONE,
			SST_SOUND,
			SST_STREAMSOUND
		};

		//---------------------------------------------------------------------//

		void					DeleteSoundSource();

		bool					isLooped;
		bool					isPaused;
		SOUND_SOURCE_TYPE		soundSourceType;
		UInt64_t				samplesOffset;
		UInt64_t				maxCountSamples;
		
		void*					sound;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !AUDIORESOURCE_H