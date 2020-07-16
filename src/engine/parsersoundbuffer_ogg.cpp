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

#include "audio/isoundbuffer.h"

#include "global.h"
#include "consolesystem.h"
#include "parsersoundbuffer_ogg.h"

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
// Open
// ------------------------------------------------------------------------------------ //
le::ISoundBuffer* le::ParserSoundBufferOGG::Open( const char* Path, IFactory* AudioSystemFactory )
{
	g_consoleSystem->PrintWarning( "le::ParserSoundBufferOGG::Open( const char*, IFactory* ) - not implemented" );
	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Read
// ------------------------------------------------------------------------------------ //
le::ISoundBuffer* le::ParserSoundBufferOGG::Read( const char* Path, IFactory* AudioSystemFactory )
{
	OggVorbis_File		oggVorbisFile;
	if ( ov_fopen( Path, &oggVorbisFile ) < 0 )
	{
		g_consoleSystem->PrintError( "Failed open sound buffer" );
		return nullptr;
	}

	vorbis_info*		vorbisInfo = ov_info( &oggVorbisFile, -1 );

	le::UInt32_t		sizeData = ov_pcm_total( &oggVorbisFile, -1 ) * vorbisInfo->channels * 2;
	le::UInt16_t*		data = ( le::UInt16_t* ) malloc( sizeData );

	le::UInt32_t		offset = 0;
	le::UInt32_t		size = 0;
	le::UInt32_t		sel = 0;

	do
	{
		size = ov_read( &oggVorbisFile, ( char* ) data + offset, 4096, 0, 2, 1, ( int* ) &sel );
		offset += size;
	}
	while ( size != 0 );

	le::ISoundBuffer* soundBuffer = ( le::ISoundBuffer* ) AudioSystemFactory->Create( SOUNDBUFFER_INTERFACE_VERSION );
	soundBuffer->IncrementReference();
	soundBuffer->Create();
	soundBuffer->Append( vorbisInfo->channels == 1 ? le::SF_MONO16 : le::SF_STEREO16, data, sizeData, vorbisInfo->rate );

	free( data );
	vorbis_info_clear( vorbisInfo );
	ov_clear( &oggVorbisFile );
	return soundBuffer;
}