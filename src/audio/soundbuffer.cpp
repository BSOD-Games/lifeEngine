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
#include <alc.h>

#include "engine/iparsersoundbuffer.h"

#include "global.h"
#include "audiodevice.h"
#include "soundbuffer.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::SoundBuffer::SoundBuffer() :
	countReferences( 0 ),
	handle( 0 ),
	sampleRate( 0 ),
    channelCount( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::SoundBuffer::~SoundBuffer()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Increment references
// ------------------------------------------------------------------------------------ //
void le::SoundBuffer::IncrementReference()
{
	++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement references
// ------------------------------------------------------------------------------------ //
void le::SoundBuffer::DecrementReference()
{
	--countReferences;
}

// ------------------------------------------------------------------------------------ //
// Release
// ------------------------------------------------------------------------------------ //
void le::SoundBuffer::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::SoundBuffer::GetCountReferences() const
{
	return countReferences;
}

// ------------------------------------------------------------------------------------ //
// Create
// ------------------------------------------------------------------------------------ //
void le::SoundBuffer::Create()
{
	if ( handle != 0 )		return;
	alGenBuffers( 1, &handle );
}

#include <phonon.h>
#include <fstream>
#include <alext.h>
// ------------------------------------------------------------------------------------ //
// Append
// ------------------------------------------------------------------------------------ //
void le::SoundBuffer::Append( SAMPLE_FORMAT SampleFormat, const Byte_t* Samples, UInt32_t SamplesSize, UInt32_t SampleRate )
{
	if ( handle == 0 || !Samples )		return;

    // Convert samples to float
    switch ( SampleFormat )
    {
    case SF_MONO8:
    case SF_STEREO8:
    {
        Byte_t*         samples_8 = ( Byte_t* ) Samples;
        samples.resize( SamplesSize / sizeof( Byte_t ) );

        for ( UInt32_t index = 0, count = samples.size(); index < count; ++index )
            samples[ index ] = samples_8[ index ] / ( float ) std::numeric_limits<Byte_t>::max();

        break;
    }

    case SF_MONO16:
    case SF_STEREO16:
    {
        Int16_t*            samples_16 = ( Int16_t* ) Samples;
        samples.resize( SamplesSize / sizeof( Int16_t ) );

        for ( UInt32_t index = 0, count = samples.size(); index < count; ++index )
            samples[ index ] = samples_16[ index ] / ( float ) std::numeric_limits<Int16_t>::max();
        
        break;
    }
    }
 
    alBufferData( handle, AudioDevice::GetSampleFormat( SampleFormat ), Samples, SamplesSize, SampleRate );
   
    sampleRate = SampleRate;
    channelCount = SampleFormat == SF_MONO8 || SampleFormat == SF_MONO16 ? 1 : 2;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::SoundBuffer::Delete()
{
	if ( handle == 0 )		return;

	alDeleteBuffers( 1, &handle );

	handle = 0;
	sampleRate = 0;
	samples.clear();
}

// ------------------------------------------------------------------------------------ //
// Get count samples
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::SoundBuffer::GetCountSamples() const
{
	return samples.size();
}

// ------------------------------------------------------------------------------------ //
// Get samples
// ------------------------------------------------------------------------------------ //
float* le::SoundBuffer::GetSamples() const
{
	return ( float* ) samples.data();
}

// ------------------------------------------------------------------------------------ //
// Get sample rate
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::SoundBuffer::GetSampleRate() const
{
	return sampleRate;
}