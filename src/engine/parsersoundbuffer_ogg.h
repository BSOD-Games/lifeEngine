//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PARSERSOUNDBUFFER_OGG_H
#define PARSERSOUNDBUFFER_OGG_H

#include "engine/iparsersoundbuffer.h"

//---------------------------------------------------------------------//

struct OggVorbis_File;
struct vorbis_info;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ParserSoundBufferOGG : public IParserSoundBuffer
	{
	public:
		// IParser
		virtual void				Release();

		virtual const char*			GetVersion() const;
		virtual const char*			GetName() const;
		virtual UInt32_t			GetCountFileExtensions() const;
		virtual const char**		GetFileExtensions() const;
		virtual const char*			GetAuthor() const;

		// IParserSoundBuffer
		ParserSoundBufferOGG();
		~ParserSoundBufferOGG();

		virtual bool				Open( const char* Path );
		virtual void				Close();
		virtual void				Seek( UInt64_t SampleOffset );
		virtual UInt64_t			Read( Byte_t* Samples, UInt64_t MaxSize );

		virtual bool				IsOpened() const;
		virtual UInt64_t			GetSampleCount() const;
		virtual UInt32_t			GetChannelCount() const;
		virtual UInt32_t			GetSampleRate() const;
		virtual UInt64_t			GetSampleOffset() const;
		virtual SAMPLE_FORMAT		GetSampleFormat() const;

	private:
		SAMPLE_FORMAT			sampleFormat;
		UInt32_t				sampleRate;
		UInt64_t				sampleCount;
		UInt64_t				sampleOffset;
		UInt32_t				channelCount;

		OggVorbis_File*			oggVorbisFile;
		vorbis_info*			vorbisInfo;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !PARSERSOUNDBUFFER_OGG_H
