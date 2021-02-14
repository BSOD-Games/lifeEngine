//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IPARSERSOUNDBUFFER_H
#define IPARSERSOUNDBUFFER_H

#include "engine/iparser.h"
#include "audio/isoundbuffer.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IParserSoundBuffer : public IParser
	{
	public:
		virtual ~IParserSoundBuffer() {}
		virtual bool				Open( const char* Path ) = 0;
		virtual void				Close() = 0;
		virtual void				Seek( UInt64_t SampleOffset ) = 0;
		virtual UInt64_t			Read( Byte_t* Samples, UInt64_t MaxSize ) = 0;

		virtual bool				IsOpened() const = 0;
		virtual UInt64_t			GetSampleCount() const = 0;
		virtual UInt32_t			GetChannelCount() const = 0;
		virtual UInt32_t			GetSampleRate() const = 0;
		virtual UInt64_t			GetSampleOffset() const = 0;
		virtual SAMPLE_FORMAT		GetSampleFormat() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IPARSERSOUNDBUFFER_H
