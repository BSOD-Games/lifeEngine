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
	audioDevice( nullptr ),
	audioContext( nullptr )
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
