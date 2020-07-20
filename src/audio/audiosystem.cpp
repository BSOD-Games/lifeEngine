//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <stdexcept>
#include <al.h>
#include <alext.h>
#include <phonon.h>
#include <phonon_version.h>

#include "engine/iengine.h"

#include "global.h"
#include "audiosystem.h"
#include "sound.h"

LIFEENGINE_AUDIOSYSTEM_API( le::AudioSystem );

// ------------------------------------------------------------------------------------ //
// Steam Audio logging
// ------------------------------------------------------------------------------------ //
void SteamAudio_Logging( char* Message )
{
	le::g_consoleSystem->PrintInfo( Message );
}

// ------------------------------------------------------------------------------------ //
// Initialize subsystem
// ------------------------------------------------------------------------------------ //
bool le::AudioSystem::Initialize( IEngine* Engine )
{
	g_consoleSystem = Engine->GetConsoleSystem();
	
	try
	{
		// Creatin OpenAL device
		if ( !g_consoleSystem )				throw std::runtime_error( "Console system not founded in engine" );
		if ( !audioDevice.Create() )		throw std::runtime_error( "Failed in creating audio device" );
	
		// Creating context Steam Audio
		IPLerror	error = iplCreateContext( SteamAudio_Logging, nullptr, nullptr, &steamAudioContext );
		if ( error != IPL_STATUS_SUCCESS )
			switch ( error )
			{
			case IPL_STATUS_FAILURE:		throw std::runtime_error( "An unspecified error occurred" );
			case IPL_STATUS_OUTOFMEMORY:	throw std::runtime_error( "The system ran out of memory" );
			case IPL_STATUS_INITIALIZATION:	throw std::runtime_error( "An error occurred while initializing an external dependency" );
			default:						throw std::runtime_error( "Unknown error" );
			}

		g_consoleSystem->PrintInfo( "Steam Audio: %i.%i.%i", STEAMAUDIO_VERSION_MAJOR, STEAMAUDIO_VERSION_MINOR, STEAMAUDIO_VERSION_PATCH );

		isUpdating = true;
		threadUpdate = new std::thread( &AudioSystem::Update, this );
	}
	catch ( const std::exception& Exception )
	{
		if ( g_consoleSystem )	g_consoleSystem->PrintError( Exception.what() );
		else					g_criticalError( Exception.what() );
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::AudioSystem::AudioSystem() :
	isUpdating( false ),
	steamAudioContext( nullptr ),
	threadUpdate( nullptr )
{
	LIFEENGINE_ASSERT( !g_audioSystem );
	g_audioSystem = this;
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::AudioSystem::~AudioSystem()
{
	// Wait for the thread to terminate
	if ( threadUpdate )
	{
		{
			std::lock_guard< std::mutex >			lock( mutexUpdate );
			isUpdating = false;
		}

		threadUpdate->join();
		delete threadUpdate;
	}

	// Destroy OpenAL device
	audioDevice.Destroy();
	
	// If Steam Audio is created - destroy and cleanup
	if ( steamAudioContext )
	{
		iplDestroyContext( &steamAudioContext );
		iplCleanup();
	}

	threadUpdate = nullptr;
	steamAudioContext = nullptr;
	g_audioSystem = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Sound play
// ------------------------------------------------------------------------------------ //
void le::AudioSystem::SoundPlay( Sound* Sound )
{	
	if ( !Sound ) return;

	// We check that the sound has not been previously added
	// If we do not find it, add it to play 
	{
		std::lock_guard< std::mutex >			lock( mutexUpdate );
		auto			it = updateSounds.find( Sound );
		if ( it != updateSounds.end() )
		{
			it->second.samplesOffset = 0;
			return;
		}

		Sound->IncrementReference();
		updateSounds[ Sound ] = SoundSource{ Sound, 0 };
	}
}

// ------------------------------------------------------------------------------------ //
// Sound pause
// ------------------------------------------------------------------------------------ //
void le::AudioSystem::SoundPause( Sound* Sound )
{
	if ( !Sound ) return;
	// TODO: Add paused sound
}

// ------------------------------------------------------------------------------------ //
// Sound stop
// ------------------------------------------------------------------------------------ //
void le::AudioSystem::SoundStop( Sound* Sound )
{
	if ( !Sound ) return;
	// TODO: Add stoped sound
}

// ------------------------------------------------------------------------------------ //
// Update
// ------------------------------------------------------------------------------------ //
void le::AudioSystem::Update()
{
	bool						isNeedPlay = false;
	std::vector<float>			samplesBuffer;		// this is temp
	
	// Create source and buffers
	alGenSources( 1, &openALSource );	
	alGenBuffers( BufferCount, openALBuffers );

	// Initialize OpenAL source 
	alSourcei( openALSource, AL_LOOPING, false );
	alSourcei( openALSource, AL_SOURCE_RELATIVE, true );
	alSourcef( openALSource, AL_GAIN, 100 * 0.01f );
	alSourcef( openALSource, AL_PITCH, 1 );
	alSourcef( openALSource, AL_REFERENCE_DISTANCE, 1 );
	alSourcef( openALSource, AL_ROLLOFF_FACTOR, 1 );

	while ( true )
	{
		{
			std::lock_guard< std::mutex >		lock( mutexUpdate );
			if ( !isUpdating )					break;
		}

		ALint			alStatus = 0;
		alGetSourcei( openALSource, AL_SOURCE_STATE, &alStatus );

		if ( isNeedPlay && alStatus != AL_PLAYING )		alSourcePlay( openALSource );
		isNeedPlay = false;

		// Get the number of buffers ready to be reused
		ALint			queueSize = 0;
		ALint			processed = 0;
		alGetSourcei( openALSource, AL_BUFFERS_QUEUED, &queueSize );

		if ( queueSize > 0 )
			alGetSourcei( openALSource, AL_BUFFERS_PROCESSED, &processed );
		else
		{
			processed = BufferCount;
			queueSize = -1;
		}
		
		// Update buffers
		while ( processed-- )
		{			
			// Pop the first unused buffer from the queue
			bool			isNeedPushBuffer = false;
			ALuint			buffer = 0;

			// Get unused buffer from queue or from array if queue is empty
			if ( queueSize > 0 )
				alSourceUnqueueBuffers( openALSource, 1, &buffer );
			else
				buffer = openALBuffers[ processed ];

			// Processing all sounds
			{
				std::lock_guard< std::mutex >			lock( mutexUpdate );
				for ( auto it = updateSounds.begin(), itEnd = updateSounds.end(); it != itEnd; ++it )
				{
					SoundSource&		soundSource = it->second;
					SoundBuffer*		soundBuffer = ( SoundBuffer* ) soundSource.sound->GetBuffer();

					samplesBuffer.resize( ( soundBuffer->GetSampleRate() * soundBuffer->GetChannelCount() ) / 20 );

					UInt32_t		toFill = samplesBuffer.size();
					UInt64_t		currentOffset = soundSource.samplesOffset;
					UInt64_t		sampleCount = soundBuffer->GetCountSamples();

					if ( currentOffset + toFill > sampleCount )
						toFill = sampleCount - currentOffset;

					memcpy( samplesBuffer.data(), soundBuffer->GetSamples() + currentOffset, toFill * sizeof( float ) );
					soundSource.samplesOffset += toFill;

					if ( soundSource.samplesOffset >= sampleCount )
					{
						if ( soundSource.sound->IsLooped() )
							soundSource.samplesOffset = 0;
						else
						{
							// TODO: Add decrement referecnes in Sound
							it = updateSounds.erase( it );
							itEnd = updateSounds.end();
							--it;
						}
					}

					isNeedPushBuffer = true;
				}
			}

			// Fill buffer and add in queue
			if ( isNeedPushBuffer )
			{
				alBufferData( buffer, AL_FORMAT_MONO_FLOAT32, samplesBuffer.data(), samplesBuffer.size() * sizeof( float ), 44100 );
				alSourceQueueBuffers( openALSource, 1, &buffer );
				isNeedPlay = true;
			}
		}

		// Leave some time for the other threads if the stream is still playing
		if ( alStatus != AL_STOPPED )
			std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}

	// Delete source and buffers
	alDeleteSources( 1, &openALSource );
	alDeleteBuffers( BufferCount, openALBuffers );
}

// ------------------------------------------------------------------------------------ //
// Get listener
// ------------------------------------------------------------------------------------ //
le::IListener* le::AudioSystem::GetListener() const
{
	return ( IListener* ) &listener;
}

// ------------------------------------------------------------------------------------ //
// Get factory
// ------------------------------------------------------------------------------------ //
le::IFactory* le::AudioSystem::GetFactory() const
{
	return ( IFactory* ) &factory;
}