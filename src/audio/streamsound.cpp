//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <al.h>

#include "engine/iparsersoundbuffer.h"
#include "engine/iconsolesystem.h"

#include "global.h"
#include "audiosystem.h"
#include "streamsound.h"

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::StreamSound::IncrementReference()
{
	++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::StreamSound::DecrementReference()
{
	--countReferences;
}

// ------------------------------------------------------------------------------------ //
// Release
// ------------------------------------------------------------------------------------ //
void le::StreamSound::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::StreamSound::GetCountReferences() const
{
	return countReferences;
}

// ------------------------------------------------------------------------------------ //
// Create
// ------------------------------------------------------------------------------------ //
void le::StreamSound::Create()
{
	if ( handle != 0 )		return;
	alGenSources( 1, &handle );

	// Initialize propperties sound
	SetLoop( false );
	SetRelativeToListener( false );
	SetVolume( 100.f );
	SetPitch( 1.f );
	SetMinDistance( 1.f );
	SetAttenuation( 1.f );
	SetPosition( Vector3D_t( 0.f, 0.f, 0.f ) );
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::StreamSound::Delete()
{
	if ( handle == 0 )		return;
	if ( IsOpened() )		Close();
	
	alDeleteSources( 1, &handle );
	
	handle = 0;
}

// ------------------------------------------------------------------------------------ //
// Play
// ------------------------------------------------------------------------------------ //
void le::StreamSound::Play()
{
	if ( handle == 0 || sampleFormat == 0 )		return;
	
	bool				isStreaming = false;
	SOUND_STATUS		threadStartState = SS_STOPED;

	{
		std::lock_guard< std::mutex >			lock( mutexStreamSamples );

		isStreaming = this->isStreaming;
		threadStartState = this->threadStartState;
	}

	if ( isStreaming && ( threadStartState == SS_PAUSED ) )
	{
		std::lock_guard< std::mutex >			lock( mutexStreamSamples );

		this->threadStartState = SS_PLAYING;
		alSourcePlay( handle );
		return;
	}
	else if ( isStreaming && ( threadStartState == SS_PLAYING ) )
		Stop();

	this->isStreaming = true;
	this->threadStartState = SS_PLAYING;
	threadStreamSamples = new std::thread( &StreamSound::StreamData, this );
}

// ------------------------------------------------------------------------------------ //
// Pause
// ------------------------------------------------------------------------------------ //
void le::StreamSound::Pause()
{
	if ( handle == 0 )		return;
	
	// Handle Pause() being called before the thread has started
	{
		std::lock_guard< std::mutex >			lock( mutexStreamSamples );

		if ( !isStreaming )		return;
		threadStartState = SS_PAUSED;
	}

	alSourcePause( handle );
}

// ------------------------------------------------------------------------------------ //
// Stop
// ------------------------------------------------------------------------------------ //
void le::StreamSound::Stop()
{
	if ( handle == 0 )		return;
	
	// Request the thread to terminate
	{
		std::lock_guard< std::mutex >			lock( mutexStreamSamples );
		isStreaming = false;
	}

	// Wait for the thread to terminate
	threadStreamSamples->join();
	delete threadStreamSamples;
	threadStreamSamples = nullptr;

	// Move to the beginning
	parserSoundBuffer->Seek( 0 );
}

// ------------------------------------------------------------------------------------ //
// Open
// ------------------------------------------------------------------------------------ //
void le::StreamSound::Open( IParserSoundBuffer* ParserSoundBuffer )
{
	parserSoundBuffer = ParserSoundBuffer;
	
	sampleRate = parserSoundBuffer->GetSampleRate();
	sampleFormat = g_audioSystem->GetAudioDevice().GetSampleFormat( parserSoundBuffer->GetSampleFormat() );
	samplesProcessed = 0;
	isStreaming = false;

	// Check if the format is valid
	if ( sampleFormat == 0 )
	{
		sampleRate = 0;
		g_consoleSystem->PrintError( "Unsupported sample format" );
	}

	samples.resize( parserSoundBuffer->GetSampleRate() * 2 );
}

// ------------------------------------------------------------------------------------ //
// Close
// ------------------------------------------------------------------------------------ //
void le::StreamSound::Close()
{
	if ( !parserSoundBuffer ) return;

	Stop();
	parserSoundBuffer->Release();
	parserSoundBuffer = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Set loop
// ------------------------------------------------------------------------------------ //
void le::StreamSound::SetLoop( bool IsLoop )
{
	isLoop = IsLoop;
}

// ------------------------------------------------------------------------------------ //
// Set relative to listener
// ------------------------------------------------------------------------------------ //
void le::StreamSound::SetRelativeToListener( bool IsRelative )
{
	if ( handle == 0 )		return;
	alSourcei( handle, AL_SOURCE_RELATIVE, IsRelative );
}

// ------------------------------------------------------------------------------------ //
// Set volume
// ------------------------------------------------------------------------------------ //
void le::StreamSound::SetVolume( float Volume )
{
	if ( handle == 0 )		return;
	alSourcef( handle, AL_GAIN, Volume * 0.01f );
}

// ------------------------------------------------------------------------------------ //
// Set pitch
// ------------------------------------------------------------------------------------ //
void le::StreamSound::SetPitch( float Pitch )
{
	if ( handle == 0 )		return;
	alSourcef( handle, AL_PITCH, Pitch );
}

// ------------------------------------------------------------------------------------ //
// Set min distance
// ------------------------------------------------------------------------------------ //
void le::StreamSound::SetMinDistance( float Distance )
{
	if ( handle == 0 )		return;
	alSourcef( handle, AL_REFERENCE_DISTANCE, Distance );
}

// ------------------------------------------------------------------------------------ //
// Set attenuation
// ------------------------------------------------------------------------------------ //
void le::StreamSound::SetAttenuation( float Attenuation )
{
	if ( handle == 0 )		return;
	alSourcef( handle, AL_ROLLOFF_FACTOR, Attenuation );
}

// ------------------------------------------------------------------------------------ //
// Set position
// ------------------------------------------------------------------------------------ //
void le::StreamSound::SetPosition( const Vector3D_t & Position )
{
	if ( handle == 0 )		return;
	alSource3f( handle, AL_POSITION, Position.x, Position.y, Position.z );
}

// ------------------------------------------------------------------------------------ //
// Is opened
// ------------------------------------------------------------------------------------ //
bool le::StreamSound::IsOpened() const
{
	return parserSoundBuffer;
}

// ------------------------------------------------------------------------------------ //
// Is looped
// ------------------------------------------------------------------------------------ //
bool le::StreamSound::IsLooped() const
{
	return isLoop;
}

// ------------------------------------------------------------------------------------ //
// Is relative to listener
// ------------------------------------------------------------------------------------ //
bool le::StreamSound::IsRelativeToListener() const
{
	if ( handle == 0 )		return false;

	ALint			isRelativeToListener = 0;
	alGetSourcei( handle, AL_SOURCE_RELATIVE, &isRelativeToListener );

	return isRelativeToListener != 0;
}

// ------------------------------------------------------------------------------------ //
// Get volume
// ------------------------------------------------------------------------------------ //
float le::StreamSound::GetVolume() const
{
	if ( handle == 0 )		return 0.f;

	ALfloat			volume = 0.f;
	alGetSourcef( handle, AL_GAIN, &volume );

	return volume * 100.f;
}

// ------------------------------------------------------------------------------------ //
// Get pitch
// ------------------------------------------------------------------------------------ //
float le::StreamSound::GetPitch() const
{
	if ( handle == 0 )		return 0.f;

	ALfloat			pitch = 0.f;
	alGetSourcef( handle, AL_PITCH, &pitch );

	return pitch;
}

// ------------------------------------------------------------------------------------ //
// Get min distance
// ------------------------------------------------------------------------------------ //
float le::StreamSound::GetMinDistance() const
{
	if ( handle == 0 )		return 0.f;

	ALfloat			distance = 0.f;
	alGetSourcef( handle, AL_REFERENCE_DISTANCE, &distance );

	return distance;
}

// ------------------------------------------------------------------------------------ //
// Get attenuation
// ------------------------------------------------------------------------------------ //
float le::StreamSound::GetAttenuation() const
{
	if ( handle == 0 )		return 0.f;

	ALfloat			attenuation = 0.f;
	alGetSourcef( handle, AL_ROLLOFF_FACTOR, &attenuation );

	return attenuation;
}

// ------------------------------------------------------------------------------------ //
// Get status
// ------------------------------------------------------------------------------------ //
le::SOUND_STATUS le::StreamSound::GetStatus() const
{
	if ( handle == 0 )		return SS_STOPED;

	ALint			alStatus = 0;
	alGetSourcei( handle, AL_SOURCE_STATE, &alStatus );

	switch ( alStatus )
	{
	case AL_PLAYING:	return SS_PLAYING;
	case AL_PAUSED:		return SS_PAUSED;

	default:
	case AL_STOPPED:
		return SS_STOPED;
	}
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::StreamSound::GetPosition() const
{
	if ( handle == 0 )		return Vector3D_t( 0.f, 0.f, 0.f );

	Vector3D_t			position;
	alGetSource3f( handle, AL_POSITION, &position.x, &position.y, &position.z );

	return position;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::StreamSound::StreamSound() :
	isStreaming( false ),
	isLoop( false ),
	sampleRate( 0 ),
	sampleFormat( 0 ),
	samplesProcessed( 0 ),
	threadStartState( SS_STOPED ),
	countReferences( 0 ),
	handle( 0 ),
	parserSoundBuffer( nullptr ),
	threadStreamSamples( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::StreamSound::~StreamSound()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Stream data to buffers
// ------------------------------------------------------------------------------------ //
void le::StreamSound::StreamData()
{
	bool			requestStop = false;

	{
		std::lock_guard< std::mutex >			lock( mutexStreamSamples );
		if ( threadStartState == SS_STOPED )
		{
			isStreaming = false;
			return;
		}
	}

	// Create the buffers
	alGenBuffers( 3, buffers );
	for ( UInt32_t index = 0; index < 3; ++index )
		bufferSeeks[ index ] = -1;

	// Fill the queue
	requestStop = FillQueue();

	// Play the sound
	alSourcePlay( handle );

	{
		std::lock_guard< std::mutex >			lock( mutexStreamSamples );

		// Check if the thread was launched Paused
		if ( threadStartState == SS_PAUSED )
			alSourcePause( handle );
	}

	for ( ;; )
	{
		{
			std::lock_guard< std::mutex >		lock( mutexStreamSamples );
			if ( !isStreaming )					return;
		}

		// The stream has been interrupted!
		if ( GetStatus() == SS_STOPED )
		{
			if ( !requestStop )
			{
				// Just continue
				alSourcePlay( handle );
			}
			else
			{
				// End streaming
				std::lock_guard< std::mutex >			lock( mutexStreamSamples );
				isStreaming = false;
			}
		}
		
		// Get the number of buffers that have been processed (i.e. ready for reuse)
		ALint		nbProcessed = 0;
		alGetSourcei( handle, AL_BUFFERS_PROCESSED, &nbProcessed );

		while ( nbProcessed-- )
		{

			// Pop the first unused buffer from the queue
			ALuint			buffer;
			alSourceUnqueueBuffers( handle, 1, &buffer );

			// Find its number
			UInt32_t		bufferNum = 0;
			for ( UInt32_t index = 0; index < 3; ++index )
				if ( buffers[ index ] == buffer )
				{
					bufferNum = index;
					break;
				}

			// Retrieve its size and add it to the samples count
			if ( bufferSeeks[ bufferNum ] != -1 )
			{
				// This was the last buffer before EOF or Loop End: reset the sample count
				samplesProcessed = bufferSeeks[ bufferNum ];
				bufferSeeks[ bufferNum ] = -1;
			}
			else
			{
				ALint		size, bits;
				alGetBufferi( buffer, AL_SIZE, &size );
				alGetBufferi( buffer, AL_BITS, &bits );

				// Bits can be 0 if the format or parameters are corrupt, avoid division by zero
				if ( bits == 0 )
				{
					// Abort streaming (exit main loop)
					std::lock_guard< std::mutex >			lock( mutexStreamSamples );
					
					isStreaming = false;
					requestStop = true;
					break;
				}
				else
					samplesProcessed += size / bits;
			}

			// Fill it and push it back into the playing queue
			if ( !requestStop )
			{
				if ( FillAndPushBuffer( bufferNum, false ) )
					requestStop = true;
			}
		}

		// Leave some time for the other threads if the stream is still playing
		if ( GetStatus() != SS_STOPED )
			std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}

	// Stop the playback
	alSourceStop( handle );

	// Dequeue any buffer left in the queue
	ClearQueue();

	// Reset the playing position
	samplesProcessed = 0;

	// Delete the buffers
	alSourcei( handle, AL_BUFFER, 0 );
	alDeleteBuffers( 3, buffers );
}

// ------------------------------------------------------------------------------------ //
// Fill queue
// ------------------------------------------------------------------------------------ //
bool le::StreamSound::FillQueue()
{
	// Fill and enqueue all the available buffers
	bool			requestStop = false;
	for ( UInt32_t index = 0; ( index < 3 ) && !requestStop; ++index )
	{
		// Since no sound has been loaded yet, we can't schedule loop seeks preemptively,
		// So if we start on EOF or Loop End, we let fillAndPushBuffer() adjust the sample count
		if ( FillAndPushBuffer( index, index == 0 ) )
			requestStop = true;
	}

	return requestStop;
}

// ------------------------------------------------------------------------------------ //
// Clear queue
// ------------------------------------------------------------------------------------ //
void le::StreamSound::ClearQueue()
{    
	// Get the number of buffers still in the queue
	ALint			nbQueued;
	alGetSourcei( handle, AL_BUFFERS_QUEUED, &nbQueued );

	// Dequeue them all
	ALuint			buffer;
	for ( ALint index = 0; index < nbQueued; ++index )
		alSourceUnqueueBuffers( handle, 1, &buffer );
}

// ------------------------------------------------------------------------------------ //
// Fill and push buffer
// ------------------------------------------------------------------------------------ //
bool le::StreamSound::FillAndPushBuffer( UInt32_t BufferNum, bool ImmediateLoop )
{
	bool		requestStop = false;

	// Acquire audio data, also address EOF and error cases if they occur
	Chunk		data = { nullptr, 0 };
	for ( UInt32_t retryCount = 0; !GetData( data ) && retryCount < 2; ++retryCount )
	{
		// Check if the stream must loop or stop
		if ( !isLoop )
		{
			// Not looping: Mark this buffer as ending with 0 and request stop
			if ( data.samples != NULL && data.sampleCount != 0 )
				bufferSeeks[ BufferNum ] = 0;

			requestStop = true;
			break;
		}

		// Return to the beginning or loop-start of the stream source using onLoop(), and store the result in the buffer seek array
		// This marks the buffer as the "last" one (so that we know where to reset the playing position)
		parserSoundBuffer->Seek( 0 );
		bufferSeeks[ BufferNum ] = 0;

		// If we got data, break and process it, else try to fill the buffer once again
		if ( data.samples != NULL && data.sampleCount != 0 )
			break;

		// If immediateLoop is specified, we have to immediately adjust the sample count
		if ( ImmediateLoop && bufferSeeks[ BufferNum ] != -1 )
		{
			// We just tried to begin preloading at EOF or Loop End: reset the sample count
			samplesProcessed = bufferSeeks[ BufferNum ];
			bufferSeeks[ BufferNum ] = -1;
		}

		// We're a looping sound that got no data, so we retry GetData()
	}

	// Fill the buffer if some data was returned
	if ( data.samples && data.sampleCount )
	{
		UInt32_t			buffer = buffers[ BufferNum ];

		// Fill the buffer
		alBufferData( buffer, sampleFormat, data.samples, data.sampleCount, sampleRate );

		// Push it into the sound queue
		alSourceQueueBuffers( handle, 1, &buffer );
	}
	else
	{
		// If we get here, we most likely ran out of retries
		requestStop = true;
	}

	return requestStop;
}

// ------------------------------------------------------------------------------------ //
// Get data from file
// ------------------------------------------------------------------------------------ //
bool le::StreamSound::GetData( Chunk& Data )
{
	std::lock_guard< std::mutex >			lock( mutexStreamSamples );

	UInt32_t		toFill = samples.size();
	UInt64_t		currentOffset = parserSoundBuffer->GetSampleOffset();
	UInt64_t		loopEnd = parserSoundBuffer->GetSampleCount();

	// If the loop end is enabled and imminent, request less data.
	// This will trip an "onLoop()" call from the underlying SoundStream,
	// and we can then take action.
	if ( ( loopEnd != 0 ) && ( currentOffset <= loopEnd ) && ( currentOffset + toFill > loopEnd ) )
		toFill = loopEnd - currentOffset;

	// Fill the chunk parameters
	Data.samples = &samples[ 0 ];
	Data.sampleCount = parserSoundBuffer->Read( &samples[ 0 ], toFill );
	currentOffset += Data.sampleCount;

	// Check if we have stopped obtaining samples or reached either the EOF or the loop end point
	return ( Data.sampleCount != 0 ) && ( currentOffset < parserSoundBuffer->GetSampleCount() ) && !( currentOffset == loopEnd && parserSoundBuffer->GetSampleCount() != 0 );
}
