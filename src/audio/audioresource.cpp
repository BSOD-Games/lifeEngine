//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "audio/soundbuffer.h"
#include "audio/sound.h"
#include "audio/streamsound.h"

#include "audioresource.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::AudioResource::AudioResource() :
	isLooped( false ),
	isPaused( false ),
	soundSourceType( SST_NONE ),
	samplesOffset( 0 ),
	maxCountSamples( 0 ),
	sound( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Constructor for copy
// ------------------------------------------------------------------------------------ //
le::AudioResource::AudioResource( AudioResource& Copy )
{
	isPaused = Copy.isPaused;
	isLooped = Copy.isLooped;
	soundSourceType = Copy.soundSourceType;
	samplesOffset = Copy.samplesOffset;
	maxCountSamples = Copy.maxCountSamples;
	sound = Copy.sound;

	switch ( soundSourceType )
	{
	case SST_SOUND:				static_cast< Sound* >( sound )->IncrementReference(); break;
	case SST_STREAMSOUND:		static_cast< StreamSound* >( sound )->IncrementReference(); break;
	}
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::AudioResource::~AudioResource()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Claer audio resource
// ------------------------------------------------------------------------------------ //
void le::AudioResource::Clear()
{
	DeleteSoundSource();

	isLooped = false;
	isPaused = false;
	samplesOffset = 0;
	maxCountSamples = 0;
}

// ------------------------------------------------------------------------------------ //
// Pause
// ------------------------------------------------------------------------------------ //
void le::AudioResource::Pause()
{
	isPaused = true;
}

// ------------------------------------------------------------------------------------ //
// Un pause
// ------------------------------------------------------------------------------------ //
void le::AudioResource::UnPause()
{
	isPaused = false;
}

// ------------------------------------------------------------------------------------ //
// Set sound source
// ------------------------------------------------------------------------------------ //
void le::AudioResource::SetSoundSource( Sound* Sound )
{
	if ( sound )		DeleteSoundSource();

	SoundBuffer*		soundBuffer = static_cast< SoundBuffer* >( Sound->GetBuffer() );
	maxCountSamples = ( soundBuffer->GetSampleRate() * soundBuffer->GetChannelCount() ) / 20;

	Sound->IncrementReference();
	sound = Sound;
	soundSourceType = SST_SOUND;
}

// ------------------------------------------------------------------------------------ //
// Set stream source
// ------------------------------------------------------------------------------------ //
void le::AudioResource::SetSoundSource( StreamSound* StreamSound )
{
	if ( sound )		DeleteSoundSource();

	le::StreamSound*		streamSound = static_cast< le::StreamSound* >( StreamSound );
	maxCountSamples = ( streamSound->GetSampleRate() * streamSound->GetChannelCount() ) / 20;

	StreamSound->IncrementReference();
	sound = StreamSound;
	soundSourceType = SST_STREAMSOUND;
}

// ------------------------------------------------------------------------------------ //
// Set loop
// ------------------------------------------------------------------------------------ //
void le::AudioResource::SetLoop( bool IsLooped )
{
	isLooped = IsLooped;
}

// ------------------------------------------------------------------------------------ //
// Set samples offset
// ------------------------------------------------------------------------------------ //
void le::AudioResource::SetSamplesOffset( UInt64_t SamplesOffset )
{
	samplesOffset = SamplesOffset;
}

// ------------------------------------------------------------------------------------ //
// Is looped
// ------------------------------------------------------------------------------------ //
bool le::AudioResource::IsLooped() const
{
	return isLooped;
}

// ------------------------------------------------------------------------------------ //
// Is pause
// ------------------------------------------------------------------------------------ //
bool le::AudioResource::IsPause() const
{
	return isPaused;
}

// ------------------------------------------------------------------------------------ //
// Get samples offset
// ------------------------------------------------------------------------------------ //
le::UInt64_t le::AudioResource::GetSamplesOffset() const
{
	return samplesOffset;
}

// ------------------------------------------------------------------------------------ //
// Get data
// ------------------------------------------------------------------------------------ //
bool le::AudioResource::GetData( Chunk& Chunk )
{
	if ( soundSourceType == SST_STREAMSOUND )	return false;
	
	bool		isExistsData = static_cast< Sound* >( sound )->GetData( Chunk, samplesOffset, maxCountSamples );
	samplesOffset += Chunk.sampleCount;

	return isExistsData;
}

// ------------------------------------------------------------------------------------ //
// Delete sound rource
// ------------------------------------------------------------------------------------ //
void le::AudioResource::DeleteSoundSource()
{
	if ( sound )
		switch ( soundSourceType )
		{
		case SST_SOUND:
		{
			Sound*			sound = static_cast< Sound* >( this->sound );
			if ( sound->GetCountReferences() <= 0 )			sound->Release();
			else											sound->DecrementReference();
			
			break;
		}

		case SST_STREAMSOUND:
		{
			StreamSound*		streamSound = static_cast< StreamSound* >( sound );
			if ( streamSound->GetCountReferences() <= 0 )	streamSound->Release();
			else											streamSound->DecrementReference();
			
			break;
		}
		}

	soundSourceType = SST_NONE;
	sound = nullptr;
}
