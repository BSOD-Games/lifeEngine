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
#include <alc.h>

#include "common/types.h"

#include "global.h"
#include "audiodevice.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::AudioDevice::AudioDevice() :
	listenerVolume( 100.f ),
	audioDevice( nullptr ),
	audioContext( nullptr ),
	listenerPosition( 0.f, 0.f, 0.f ),
	listenerDirection( 0.f, 0.f, -1.f ),
	listenerUp( 0.f, 1.f, 0.f )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::AudioDevice::~AudioDevice()
{
	Destroy();
}

// ------------------------------------------------------------------------------------ //
// Create audio device
// ------------------------------------------------------------------------------------ //
bool le::AudioDevice::Create()
{
	try
	{
		// Open audio device
		audioDevice = alcOpenDevice( nullptr );
		if ( !audioDevice )				throw std::runtime_error( "Failed to create the audio device" );

		// Create nad make current context
		audioContext = alcCreateContext( audioDevice, nullptr );
		if ( !audioContext )			throw std::runtime_error( "Failed to create the audio context" );

		alcMakeContextCurrent( audioContext );

		// Print avaliable playback devices
		g_consoleSystem->PrintInfo( "*** Audio device info ***" );
		g_consoleSystem->PrintInfo( "Available playback devices: %s", IsExtensionSupported( "ALC_ENUMERATE_ALL_EXT" ) ? alcGetString( nullptr, ALC_ALL_DEVICES_SPECIFIER ) : alcGetString( nullptr, ALC_DEVICE_SPECIFIER ) );
		g_consoleSystem->PrintInfo( "Available capture devices: %s", alcGetString( nullptr, ALC_CAPTURE_DEVICE_SPECIFIER ) );
		g_consoleSystem->PrintInfo( "Default playback device: %s", IsExtensionSupported( "ALC_ENUMERATE_ALL_EXT" ) ? alcGetString( nullptr, ALC_DEFAULT_ALL_DEVICES_SPECIFIER ) : alcGetString( nullptr, ALC_DEFAULT_DEVICE_SPECIFIER ) );
		g_consoleSystem->PrintInfo( "Default capture device: %s", alcGetString( nullptr, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER ) );
		g_consoleSystem->PrintInfo( "Selected audio device: %s", IsExtensionSupported( "ALC_ENUMERATE_ALL_EXT" ) ? alcGetString( audioDevice, ALC_ALL_DEVICES_SPECIFIER ) : alcGetString( audioDevice, ALC_DEVICE_SPECIFIER ) );

		ALCint			major = 0;
		ALCint			minor = 0;
		alcGetIntegerv( audioDevice, ALC_MAJOR_VERSION, 1, &major );
		alcGetIntegerv( audioDevice, ALC_MINOR_VERSION, 1, &minor );
		g_consoleSystem->PrintInfo( "ALC version %i.%i", major, minor );
		g_consoleSystem->PrintInfo( "ALC extensions: %s", alcGetString( audioDevice, ALC_EXTENSIONS ) );

		// Print info by OpenAL and extensions		
		g_consoleSystem->PrintInfo( "OpenAL vendor: %s", alGetString( AL_VENDOR ) );
		g_consoleSystem->PrintInfo( "OpenAL renderer: %s", alGetString( AL_RENDERER ) );
		g_consoleSystem->PrintInfo( "OpenAL version: %s", alGetString( AL_VERSION ) );
		g_consoleSystem->PrintInfo( "OpenAL extensions: %s", alGetString( AL_EXTENSIONS ) );
		g_consoleSystem->PrintInfo( "*** Audio device info end ***" );

		// Initialize listener
		SetGlobalVolume( listenerVolume );
		SetListenerPosition( listenerPosition );
		SetListenerDirection( listenerDirection );
		SetListenerUp( listenerUp );
	}
	catch ( const std::exception& Exception )
	{
		g_consoleSystem->PrintError( Exception.what() );
		return false;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// Destroy audio device
// ------------------------------------------------------------------------------------ //
void le::AudioDevice::Destroy()
{
	alcMakeContextCurrent( nullptr );

	if ( audioContext )			alcDestroyContext( audioContext );
	if ( audioDevice )			alcCloseDevice( audioDevice );

	audioContext = nullptr;
	audioDevice = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Set global volume
// ------------------------------------------------------------------------------------ //
void le::AudioDevice::SetGlobalVolume( float Volume )
{
	if ( !audioContext ) return;

	alListenerf( AL_GAIN, Volume * 0.01f );
	listenerVolume = Volume;
}

// ------------------------------------------------------------------------------------ //
// Set listener position
// ------------------------------------------------------------------------------------ //
void le::AudioDevice::SetListenerPosition( const Vector3D_t& Position )
{
	if ( !audioContext ) return;

	alListener3f( AL_POSITION, listenerPosition.x, listenerPosition.y, listenerPosition.z );
	listenerPosition = Position;
}

// ------------------------------------------------------------------------------------ //
// Set listener direction
// ------------------------------------------------------------------------------------ //
void le::AudioDevice::SetListenerDirection( const Vector3D_t& Direction )
{
	if ( !audioContext ) return;

	float		orientation[] = { Direction.x, Direction.y, Direction.z, listenerUp.x, listenerUp.y, listenerUp.z };
	alListenerfv( AL_ORIENTATION, orientation );

	listenerDirection = Direction;
}

// ------------------------------------------------------------------------------------ //
// Set listener up
// ------------------------------------------------------------------------------------ //
void le::AudioDevice::SetListenerUp( const Vector3D_t& Up )
{
	if ( !audioContext ) return;

	float		orientation[] = { listenerDirection.x, listenerDirection.y, listenerDirection.z, Up.x, Up.y, Up.z };
	alListenerfv( AL_ORIENTATION, orientation );

	listenerUp = Up;
}

// ------------------------------------------------------------------------------------ //
// Get sample format
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::AudioDevice::GetSampleFormat( SAMPLE_FORMAT SampleFormat )
{
	switch ( SampleFormat )
	{
	case SF_MONO8:		return AL_FORMAT_MONO8;
	case SF_MONO16:		return AL_FORMAT_MONO16;
	case SF_STEREO8:	return AL_FORMAT_STEREO8;
	case SF_STEREO16:	return AL_FORMAT_STEREO16;
	}

	return 0;
}

// ------------------------------------------------------------------------------------ //
// Get global volume
// ------------------------------------------------------------------------------------ //
float le::AudioDevice::GetGlobalVolume() const
{
	return listenerVolume;
}

// ------------------------------------------------------------------------------------ //
// Get listener position
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::AudioDevice::GetListenerPosition() const
{
	return listenerPosition;
}

// ------------------------------------------------------------------------------------ //
// Get listener direction
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::AudioDevice::GetListenerDirection() const
{
	return listenerDirection;
}

// ------------------------------------------------------------------------------------ //
// Get listener up
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::AudioDevice::GetListenerUp() const
{
	return listenerUp;
}

// ------------------------------------------------------------------------------------ //
// Is extension supported
// ------------------------------------------------------------------------------------ //
bool le::AudioDevice::IsExtensionSupported( const std::string& Extension )
{
	if ( Extension.size() > 2 && Extension.substr( 0, 3 ) == "ALC" )
	{
		if ( !audioDevice )			return false;
		return alcIsExtensionPresent( audioDevice, Extension.c_str() ) != ALC_FALSE;
	}
	else
		return alIsExtensionPresent( Extension.c_str() ) != ALC_FALSE;
}
