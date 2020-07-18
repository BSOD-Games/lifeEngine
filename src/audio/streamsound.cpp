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
	if ( handle == 0 )		return;

	bool				isStreaming = false;
	SOUND_STATUS		threadState = SS_STOPED;

	// Getting values state
	{
		std::lock_guard< std::mutex >		lock( mutexStreamData );
		isStreaming = this->isStreaming;
		threadState = this->threadState;
	}

	// If sound is streaming and thread state is SS_PAUSED - start play sound
	// If sound is streaming and thread state is SS_PLAYING - restart play sound
	if ( isStreaming && threadState == SS_PAUSED )
	{
		std::lock_guard< std::mutex >		lock( mutexStreamData );

		this->threadState = SS_PLAYING;
		alSourcePlay( handle );
		return;
	}
	else if ( isStreaming && threadState == SS_PLAYING )
		Stop();

	// Start thread for streaming data
	this->isStreaming = true;
	this->threadState = SS_PLAYING;
	threadStreamData = new std::thread( &StreamSound::StreamData, this );
}

// ------------------------------------------------------------------------------------ //
// Pause
// ------------------------------------------------------------------------------------ //
void le::StreamSound::Pause()
{
	if ( handle == 0 )		return;

	// Set state to SS_PAUSED
	{
		std::lock_guard< std::mutex >		lock( mutexStreamData );
		
		if ( !isStreaming )		return;
		threadState = SS_PAUSED;
	}

	alSourcePause( handle );
}

// ------------------------------------------------------------------------------------ //
// Stop
// ------------------------------------------------------------------------------------ //
void le::StreamSound::Stop()
{
	if ( handle == 0  )		return;

	// Request the thread to terminate
	{
		std::lock_guard< std::mutex >		lock( mutexStreamData );
		isStreaming = false;
	}

	// Wait for the thread to terminate
	if ( threadStreamData )
	{
		threadStreamData->join();
		delete threadStreamData;
		threadStreamData = nullptr;
	}

	// Move to the beginning
	if ( parserSoundBuffer )	parserSoundBuffer->Seek( 0 );
}

// ------------------------------------------------------------------------------------ //
// Open
// ------------------------------------------------------------------------------------ //
void le::StreamSound::Open( IParserSoundBuffer* ParserSoundBuffer )
{
	if ( !ParserSoundBuffer )		return;	
	if ( IsOpened() )				Close();

	parserSoundBuffer = ParserSoundBuffer;

	// Getting info by sound buffer
	sampleRate = parserSoundBuffer->GetSampleRate();
	sampleFormat = g_audioSystem->GetAudioDevice().GetSampleFormat( parserSoundBuffer->GetSampleFormat() );
	isStreaming = false;

	// Check if the format is valid
	if ( sampleFormat == 0 )
	{
		Close();

		g_consoleSystem->PrintError( "Unsupported sample format" );
		return;
	}

	samples.resize( sampleRate * parserSoundBuffer->GetChannelCount() );
}

// ------------------------------------------------------------------------------------ //
// Close
// ------------------------------------------------------------------------------------ //
void le::StreamSound::Close()
{
	if ( !parserSoundBuffer )		return;

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
	{
		std::lock_guard< std::mutex >		lock( mutexStreamData );
		
		if ( isStreaming )		return threadState;
		return SS_STOPED;
	}
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
	countReferences( 0 ),
	handle( 0 ),
	sampleFormat( 0 ),
	sampleRate( 0 ),
	threadState( SS_STOPED ),
	parserSoundBuffer( nullptr ),
	threadStreamData( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::StreamSound::~StreamSound()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Stream data
// ------------------------------------------------------------------------------------ //
void le::StreamSound::StreamData()
{
	bool			requestStop = false;

	{
		std::lock_guard< std::mutex >		lock( mutexStreamData );

		// Check if the thread was launched Stopped
		if ( threadState == SS_STOPED )
		{
			isStreaming = false;
			return;
		}
	}

	// Create the buffers
	alGenBuffers( BufferCount, buffers );

	// Fill the queue
	requestStop = FillQueue();

	// Play the sound
	alSourcePlay( handle );

	{
		std::lock_guard< std::mutex >		lock( mutexStreamData );

		// Check if the thread was launched Paused
		if ( threadState == SS_PAUSED )
			alSourcePause( handle );
	}

	while ( true )
	{
		{
			std::lock_guard< std::mutex >		lock( mutexStreamData );
			if ( !isStreaming )					break;
		}

		// The stream has been interrupted!
		if ( GetStatus() == SS_STOPED )
		{
			// If not request stop - just continue
			if ( !requestStop )			alSourcePlay( handle );
			else
			{
				// End streaming
				std::lock_guard< std::mutex >		lock( mutexStreamData );
				isStreaming = false;
			}
		}

		// Get the number of buffers that have been processed (i.e. ready for reuse)
		ALint			processed = 0;
		alGetSourcei( handle, AL_BUFFERS_PROCESSED, &processed );

		while ( processed-- )
		{
			// Pop the first unused buffer from the queue
			ALuint			buffer = 0;
			alSourceUnqueueBuffers( handle, 1, &buffer );

			// Find its number
			UInt32_t		bufferID = 0;
			for ( UInt32_t index = 0; index < BufferCount; ++index )
				if ( buffers[ index ] == buffer )
				{
					bufferID = index;
					break;
				}

			// Fill it and push it back into the playing queue
			if ( FillAndPushBuffer( bufferID, false ) )
				requestStop = true;
		}

		// Leave some time for the other threads if the stream is still playing
		if ( GetStatus() != SS_STOPED )
			std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}

	// Stop the playback
	alSourceStop( handle );

	// Dequeue any buffer left in the queue
	ClearQueue();

	// Delete the buffers
	alSourcei( handle, AL_BUFFER, 0 );
	alDeleteBuffers( BufferCount, buffers );
}

// ------------------------------------------------------------------------------------ //
// Fill queue
// ------------------------------------------------------------------------------------ //
bool le::StreamSound::FillQueue()
{
	// Fill and enqueue all the available buffers
	bool			requestStop = false;
	for ( UInt32_t index = 0; index < BufferCount && !requestStop; ++index )
	{
		// Since no sound has been loaded yet, we can't schedule loop seeks preemptively,
		// So if we start on EOF or Loop End, we let FillAndPushBuffer() adjust the sample count
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
	ALint			queued;
	alGetSourcei( handle, AL_BUFFERS_QUEUED, &queued );

	// Dequeue them all
	ALuint			buffer;
	for ( ALint index = 0; index < queued; ++index )
		alSourceUnqueueBuffers( handle, 1, &buffer );
}

// ------------------------------------------------------------------------------------ //
// Fill and push buffer
// ------------------------------------------------------------------------------------ //
bool le::StreamSound::FillAndPushBuffer( UInt32_t BufferIndex, bool ImmediateLoop )
{
	bool		requestStop = false;

	// Acquire audio data, also address EOF and error cases if they occur
	Chunk		data = { nullptr, 0 };
	for ( UInt32_t retryCount = 0; !GetData( data ) && retryCount < BufferRetries; ++retryCount )
	{
		// Check if the stream must loop or stop
		if ( !isLoop )
		{
			requestStop = true;
			break;
		}

		//  If we looped - move to start file
		parserSoundBuffer->Seek( 0 );
	}

	// Fill the buffer if some data was returned
	if ( data.samples && data.sampleCount )
	{
		UInt32_t			buffer = buffers[ BufferIndex ];

		// Fill the buffer and push it into the sound queue
		alBufferData( buffer, sampleFormat, data.samples, data.sampleCount, sampleRate );
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
// Get data samples
// ------------------------------------------------------------------------------------ //
bool le::StreamSound::GetData( Chunk& Data )
{
	std::lock_guard< std::mutex >		lock( mutexStreamData );

	UInt32_t		toFill = samples.size();
	UInt64_t		currentOffset = parserSoundBuffer->GetSampleOffset();
	UInt64_t		sampleCount = parserSoundBuffer->GetSampleCount();

	// If there are less samples left than the buffer size, we count how many samples need to be read
	if ( currentOffset + toFill > sampleCount )
		toFill = sampleCount - currentOffset;

	// Fill the chunk parameters
	Data.samples = &samples[ 0 ];
	Data.sampleCount = parserSoundBuffer->Read( &samples[ 0 ], toFill );
	currentOffset += Data.sampleCount;

	// Check if we have stopped obtaining samples or reached either the EOF or the loop end point
	return Data.sampleCount != 0 && currentOffset != sampleCount;
}