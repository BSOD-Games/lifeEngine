//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <vorbisfile.h>
#include <vorbisenc.h>
#include <ogg/ogg.h>

#include "global.h"
#include "consolesystem.h"
#include "parsersoundbuffer_ogg.h"

// ------------------------------------------------------------------------------------ //
// Release
// ------------------------------------------------------------------------------------ //
void le::ParserSoundBufferOGG::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get version
// ------------------------------------------------------------------------------------ //
const char* le::ParserSoundBufferOGG::GetVersion() const
{
	return vorbis_version_string();
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::ParserSoundBufferOGG::GetName() const
{
	return "Ogg Vorbis";
}

// ------------------------------------------------------------------------------------ //
// Get count file extensions
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ParserSoundBufferOGG::GetCountFileExtensions() const
{
	return 1;
}

// ------------------------------------------------------------------------------------ //
// Get file extensions
// ------------------------------------------------------------------------------------ //
const char** le::ParserSoundBufferOGG::GetFileExtensions() const
{
	static const char*		extensions[ 1 ] = { "ogg" };
	return extensions;
}

// ------------------------------------------------------------------------------------ //
// Get author
// ------------------------------------------------------------------------------------ //
const char* le::ParserSoundBufferOGG::GetAuthor() const
{
	return "Egor Pogulyaka";
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::ParserSoundBufferOGG::ParserSoundBufferOGG() :
	oggVorbisFile( nullptr ),
	vorbisInfo( nullptr ),
	sampleCount( 0 ),
	sampleRate( 0 ),
	sampleOffset( 0 ),
	channelCount( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::ParserSoundBufferOGG::~ParserSoundBufferOGG()
{
	if ( IsOpened() )		Close();
}

// ------------------------------------------------------------------------------------ //
// Open
// ------------------------------------------------------------------------------------ //
bool le::ParserSoundBufferOGG::Open( const char* Path )
{
	oggVorbisFile = new OggVorbis_File();
	int		error = ov_fopen( Path, oggVorbisFile );
	
	if ( error < 0 )
	{		
		std::string					strError = "unknow error";
		switch ( error )
		{
		case OV_FALSE:				strError = "not found";															break;
		case OV_EREAD:				strError = "a read from media returned an error";								break;
		case OV_ENOTVORBIS:			strError = "bitstream does not contain any Vorbis data";						break;
		case OV_EVERSION:			strError = "vorbis version mismatch";											break;
		case OV_EBADHEADER:			strError = "invalid Vorbis bitstream header";									break;
		case OV_EFAULT:				strError = "internal logic fault; indicates a bug or heap/stack corruption";	break;
		}

		g_consoleSystem->PrintError( "Failed open sound buffer: %s", strError.c_str() );

		Close();
		return false;
	}

	vorbisInfo = ov_info( oggVorbisFile, -1 );
	channelCount = vorbisInfo->channels;
	sampleFormat = vorbisInfo->channels == 1 ? le::SF_MONO16 : le::SF_STEREO16;
	sampleRate = vorbisInfo->rate;
	sampleCount = ov_pcm_total( oggVorbisFile, -1 ) * vorbisInfo->channels * 2;

	return true;
}

// ------------------------------------------------------------------------------------ //
// Close
// ------------------------------------------------------------------------------------ //
void le::ParserSoundBufferOGG::Close()
{
	if ( vorbisInfo )		vorbis_info_clear( vorbisInfo );
	if ( oggVorbisFile )
	{
		ov_clear( oggVorbisFile );
		delete oggVorbisFile;
	}

	oggVorbisFile = nullptr;
	vorbisInfo = nullptr;
	sampleCount = 0;
	sampleOffset = 0;
	sampleRate = 0;
}

// ------------------------------------------------------------------------------------ //
// Seek
// ------------------------------------------------------------------------------------ //
void le::ParserSoundBufferOGG::Seek( UInt64_t SampleOffset )
{
	if ( !oggVorbisFile ) return;

	sampleOffset = SampleOffset;
	ov_pcm_seek( oggVorbisFile, sampleOffset / channelCount );
}

// ------------------------------------------------------------------------------------ //
// Read
// ------------------------------------------------------------------------------------ //
le::UInt64_t le::ParserSoundBufferOGG::Read( Byte_t* Samples, UInt64_t MaxSize )
{
	if ( !IsOpened() || !Samples )		return 0;

	// Try to read the requested number of samples, stop only on error or end of file
	UInt64_t			size = 0;
	while ( size < MaxSize )
	{
		UInt64_t		bytesToRead = ( MaxSize - size ) * sizeof( Byte_t );
		long			bytesRead = ov_read( oggVorbisFile, ( char* ) Samples, bytesToRead, 0, 2, 1, nullptr );

		if ( bytesRead > 0 )
		{
			long		samplesRead = bytesRead / sizeof( Byte_t );
			size += samplesRead;
			Samples += samplesRead;
			sampleOffset += samplesRead;
		}
		else
		{
			// error or end of file
			break;
		}
	}

	return size;
}

// ------------------------------------------------------------------------------------ //
// Is opened
// ------------------------------------------------------------------------------------ //
bool le::ParserSoundBufferOGG::IsOpened() const
{
	return oggVorbisFile;
}

// ------------------------------------------------------------------------------------ //
// Get sample count
// ------------------------------------------------------------------------------------ //
le::UInt64_t le::ParserSoundBufferOGG::GetSampleCount() const
{
	return sampleCount;
}

// ------------------------------------------------------------------------------------ //
// Get channel count
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ParserSoundBufferOGG::GetChannelCount() const
{
	if ( !IsOpened() ) return 0;
	return vorbisInfo->channels;
}

// ------------------------------------------------------------------------------------ //
// Get sample rate
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ParserSoundBufferOGG::GetSampleRate() const
{
	return sampleRate;
}

// ------------------------------------------------------------------------------------ //
// Get sample offset
// ------------------------------------------------------------------------------------ //
le::UInt64_t le::ParserSoundBufferOGG::GetSampleOffset() const
{
	return sampleOffset;
}

// ------------------------------------------------------------------------------------ //
// Get sample format
// ------------------------------------------------------------------------------------ //
le::SAMPLE_FORMAT le::ParserSoundBufferOGG::GetSampleFormat() const
{
	return sampleFormat;
}