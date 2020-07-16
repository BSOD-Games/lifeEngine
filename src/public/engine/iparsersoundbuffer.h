//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
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

	class IFactory;
	class ISoundBuffer;

	//---------------------------------------------------------------------//

	class IParserSoundBuffer : public IParser
	{
	public:
		virtual ~IParserSoundBuffer() {}
		virtual ISoundBuffer*		 Open( const char* Path, IFactory* AudioSystemFactory ) = 0;
		virtual ISoundBuffer*		 Read( const char* Path, IFactory* AudioSystemFactory ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IPARSERSOUNDBUFFER_H
