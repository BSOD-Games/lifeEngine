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

		g_consoleSystem->PrintInfo( "SteamAudio: %i.%i.%i", STEAMAUDIO_VERSION_MAJOR, STEAMAUDIO_VERSION_MINOR, STEAMAUDIO_VERSION_PATCH );

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
// Unpause all sounds
// ------------------------------------------------------------------------------------ //
void le::AudioSystem::UnPause()
{
	// Remove all sounds from pause
	{
		std::lock_guard< std::mutex >			lock( mutexUpdate );
		if ( updateSounds.empty() )		return;

		for ( auto it = updateSounds.begin(), itEnd = updateSounds.end(); it != itEnd; ++it )
			it->second.UnPause();
	}
}

// ------------------------------------------------------------------------------------ //
// Pause all sounds
// ------------------------------------------------------------------------------------ //
void le::AudioSystem::Pause()
{
	// Set on pause all sounds in updates list
	{
		std::lock_guard< std::mutex >			lock( mutexUpdate );
		if ( updateSounds.empty() )		return;

		for ( auto it = updateSounds.begin(), itEnd = updateSounds.end(); it != itEnd; ++it )
			it->second.Pause();
	}
}

// ------------------------------------------------------------------------------------ //
// Stop all sounds
// ------------------------------------------------------------------------------------ //
void le::AudioSystem::StopAllSounds()
{
	// Delete from updates all sounds
	{
		std::lock_guard< std::mutex >			lock( mutexUpdate );
		if ( updateSounds.empty() )		return;

		updateSounds.clear();
	}
}

// ------------------------------------------------------------------------------------ //
// Sound play
// ------------------------------------------------------------------------------------ //
void le::AudioSystem::SoundPlay( Sound* Sound )
{
	if ( !Sound ) return;

	{
		std::lock_guard< std::mutex >			lock( mutexUpdate );
		auto			it = updateSounds.find( Sound );

		// If sound alrady added to update see:
		// if sound not paused - restart play from start
		// if sound on pause - play next
		if ( it != updateSounds.end() )
		{
			if ( !it->second.IsPause() )	it->second.SetSamplesOffset( 0 );
			else							it->second.UnPause();

			return;
		}

		// If sound not finded - add to update
		AudioResource			audioResource;
		audioResource.SetSoundSource( Sound );
		audioResource.SetLoop( Sound->IsLooped() );
		updateSounds[ Sound ] = audioResource;
	}
}

// ------------------------------------------------------------------------------------ //
// Sound pause
// ------------------------------------------------------------------------------------ //
void le::AudioSystem::SoundPause( Sound* Sound )
{
	if ( !Sound ) return;

	{
		std::lock_guard< std::mutex >			lock( mutexUpdate );
		auto			it = updateSounds.find( Sound );
		if ( it == updateSounds.end() )		return;

		// Set sound on pause
		it->second.Pause();
	}
}

// ------------------------------------------------------------------------------------ //
// Sound stop
// ------------------------------------------------------------------------------------ //
void le::AudioSystem::SoundStop( Sound* Sound )
{
	if ( !Sound ) return;

	{
		std::lock_guard< std::mutex >			lock( mutexUpdate );
		auto			it = updateSounds.find( Sound );
		if ( it == updateSounds.end() )		return;

		// Remove sound from update
		it->second.Clear();
		updateSounds.erase( it );
	}
}

// ------------------------------------------------------------------------------------ //
// Update
// ------------------------------------------------------------------------------------ //
void le::AudioSystem::Update()
{
	bool						isNeedPlay = false;

	// Create source and buffers
	alGenSources( 1, &openALSource );
	alGenBuffers( BufferCount, openALBuffers );

	// Initialize queue unused buffers
	for ( UInt32_t index = 0; index < BufferCount; ++index )
		unusedBuffers.push( openALBuffers[ index ] );

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
		ALint			processed = 0;
		alGetSourcei( openALSource, AL_BUFFERS_PROCESSED, &processed );

		// Add processed buffers to queue
		while ( processed-- )
		{
			ALuint			buffer = 0;
			alSourceUnqueueBuffers( openALSource, 1, &buffer );
			unusedBuffers.push( buffer );
		}

		// Updating the buffers while there are sounds on the update
		while ( !updateSounds.empty() && !unusedBuffers.empty() )
		{
			bool			isNeedPushBuffer = false;
			ALuint			buffer = unusedBuffers.front();
			Chunk			chunk;

			// Processing all sounds
			{
				std::lock_guard< std::mutex >			lock( mutexUpdate );
				for ( auto it = updateSounds.begin(), itEnd = updateSounds.end(); it != itEnd; ++it )
				{
					AudioResource&				audioResource = it->second;
					if ( audioResource.IsPause() )				continue;

					if ( !audioResource.GetData( chunk ) )
					{
						if ( audioResource.IsLooped() )			audioResource.SetSamplesOffset( 0 );
						else
						{
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
				isNeedPlay = true;
				unusedBuffers.pop();

				alBufferData( buffer, AL_FORMAT_STEREO_FLOAT32, chunk.samples, chunk.sampleCount * sizeof( float ), 44100 );
				alSourceQueueBuffers( openALSource, 1, &buffer );
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