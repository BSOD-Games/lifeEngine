//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISTREAMSOUNDINTERNAL_H
#define ISTREAMSOUNDINTERNAL_H

#include "audio/istreamsound.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IParserSoundBuffer;

	//---------------------------------------------------------------------//

	class IStreamSoundInternal : public IStreamSound
	{
	public:
		virtual ~IStreamSoundInternal() {}
		virtual void			Open( IParserSoundBuffer* ParserSoundBuffer ) = 0;
		virtual void			Close() = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISTREAMSOUNDINTERNAL_H