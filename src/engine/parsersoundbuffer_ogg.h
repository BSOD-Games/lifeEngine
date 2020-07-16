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

namespace le
{
	//---------------------------------------------------------------------//

	class ParserSoundBufferOGG : public IParserSoundBuffer
	{
	public:
		// IParser
		virtual const char*			GetVersion() const;
		virtual const char*			GetName() const;
		virtual UInt32_t			GetCountFileExtensions() const;
		virtual const char**		GetFileExtensions() const;
		virtual const char*			GetAuthor() const;

		// IParserSoundBuffer
		virtual ISoundBuffer*		Open( const char* Path, IFactory* AudioSystemFactory );
		virtual ISoundBuffer*		Read( const char* Path, IFactory* AudioSystemFactory );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !PARSERSOUNDBUFFER_OGG_H
