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

#include "global.h"
#include "audiosystem.h"
#include "soundbuffer.h"
#include "sound.h"

// ------------------------------------------------------------------------------------ //
// Create
// ------------------------------------------------------------------------------------ //
void le::Sound::Create()
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
void le::Sound::Delete()
{
	if ( handle == 0 )		return;

	alDeleteSources( 1, &handle );
	handle = 0;
}


// ------------------------------------------------------------------------------------ //
// Play
// ------------------------------------------------------------------------------------ //
void le::Sound::Play()
{
	if ( handle == 0 )		return;	
	g_audioSystem->SoundPlay( this );
}

// ------------------------------------------------------------------------------------ //
// Pause
// ------------------------------------------------------------------------------------ //
void le::Sound::Pause()
{
	if ( handle == 0 )		return;
	g_audioSystem->SoundPause( this );
}

// ------------------------------------------------------------------------------------ //
// Stop
// ------------------------------------------------------------------------------------ //
void le::Sound::Stop()
{
	if ( handle == 0 )		return;
	g_audioSystem->SoundStop( this );
}

// ------------------------------------------------------------------------------------ //
// Set loop
// ------------------------------------------------------------------------------------ //
void le::Sound::SetLoop( bool IsLoop )
{
	if ( handle == 0 )		return;
	alSourcei( handle, AL_LOOPING, IsLoop );
}

// ------------------------------------------------------------------------------------ //
// Set relative to listener
// ------------------------------------------------------------------------------------ //
void le::Sound::SetRelativeToListener( bool IsRelative )
{
	if ( handle == 0 )		return;
	alSourcei( handle, AL_SOURCE_RELATIVE, IsRelative );
}

// ------------------------------------------------------------------------------------ //
// Set volume
// ------------------------------------------------------------------------------------ //
void le::Sound::SetVolume( float Volume )
{
	if ( handle == 0 )		return;
	alSourcef( handle, AL_GAIN, Volume * 0.01f );
}

// ------------------------------------------------------------------------------------ //
// Set pitch
// ------------------------------------------------------------------------------------ //
void le::Sound::SetPitch( float Pitch )
{
	if ( handle == 0 )		return;
	alSourcef( handle, AL_PITCH, Pitch );
}

// ------------------------------------------------------------------------------------ //
// Set min distance
// ------------------------------------------------------------------------------------ //
void le::Sound::SetMinDistance( float Distance )
{
	if ( handle == 0 )		return;
	alSourcef( handle, AL_REFERENCE_DISTANCE, Distance );
}

// ------------------------------------------------------------------------------------ //
// Set attenuation
// ------------------------------------------------------------------------------------ //
void le::Sound::SetAttenuation( float Attenuation )
{
	if ( handle == 0 )		return;
	alSourcef( handle, AL_ROLLOFF_FACTOR, Attenuation );
}

// ------------------------------------------------------------------------------------ //
// Set buffer
// ------------------------------------------------------------------------------------ //
void le::Sound::SetBuffer( ISoundBuffer* SoundBuffer )
{
	if ( handle == 0 )		return;

	soundBuffer = ( le::SoundBuffer* ) SoundBuffer;
	alSourcei( handle, AL_BUFFER, soundBuffer->GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set position
// ------------------------------------------------------------------------------------ //
void le::Sound::SetPosition( const Vector3D_t& Position )
{
	if ( handle == 0 )		return;
	alSource3f( handle, AL_POSITION, Position.x, Position.y, Position.z );
}

// ------------------------------------------------------------------------------------ //
// Is looped
// ------------------------------------------------------------------------------------ //
bool le::Sound::IsLooped() const
{
	if ( handle == 0 )		return false;

	ALint			isLoop = 0;
	alGetSourcei( handle, AL_LOOPING, &isLoop );

	return isLoop != 0;
}

// ------------------------------------------------------------------------------------ //
// Is relative to listener
// ------------------------------------------------------------------------------------ //
bool le::Sound::IsRelativeToListener() const
{
	if ( handle == 0 )		return false;

	ALint			isRelativeToListener = 0;
	alGetSourcei( handle, AL_SOURCE_RELATIVE, &isRelativeToListener );

	return isRelativeToListener != 0;
}

// ------------------------------------------------------------------------------------ //
// Get volume
// ------------------------------------------------------------------------------------ //
float le::Sound::GetVolume() const
{
	if ( handle == 0 )		return 0.f;

	ALfloat			volume = 0.f;
	alGetSourcef( handle, AL_GAIN, &volume );

	return volume * 100.f;
}

// ------------------------------------------------------------------------------------ //
// Get pitch
// ------------------------------------------------------------------------------------ //
float le::Sound::GetPitch() const
{
	if ( handle == 0 )		return 0.f;

	ALfloat			pitch = 0.f;
	alGetSourcef( handle, AL_PITCH, &pitch );

	return pitch;
}

// ------------------------------------------------------------------------------------ //
// Get min distance
// ------------------------------------------------------------------------------------ //
float le::Sound::GetMinDistance() const
{
	if ( handle == 0 )		return 0.f;

	ALfloat			distance = 0.f;
	alGetSourcef( handle, AL_REFERENCE_DISTANCE, &distance );

	return distance;
}

// ------------------------------------------------------------------------------------ //
// Get attenuation
// ------------------------------------------------------------------------------------ //
float le::Sound::GetAttenuation() const
{
	if ( handle == 0 )		return 0.f;

	ALfloat			attenuation = 0.f;
	alGetSourcef( handle, AL_ROLLOFF_FACTOR, &attenuation );

	return attenuation;
}

// ------------------------------------------------------------------------------------ //
// Get status
// ------------------------------------------------------------------------------------ //
le::SOUND_STATUS le::Sound::GetStatus() const
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
// Get buffer
// ------------------------------------------------------------------------------------ //
le::ISoundBuffer* le::Sound::GetBuffer() const
{
	return soundBuffer;
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::Sound::GetPosition() const
{
	if ( handle == 0 )		return Vector3D_t( 0.f, 0.f, 0.f );

	Vector3D_t			position;
	alGetSource3f( handle, AL_POSITION, &position.x, &position.y, &position.z );

	return position;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::Sound::Sound() :
	countReferences( 0 ),
	handle( 0 ),
	soundBuffer( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::Sound::~Sound()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::Sound::IncrementReference()
{
	++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::Sound::DecrementReference()
{
	--countReferences;
}

// ------------------------------------------------------------------------------------ //
// Release
// ------------------------------------------------------------------------------------ //
void le::Sound::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Sound::GetCountReferences() const
{
	return countReferences;
}
