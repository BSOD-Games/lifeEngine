//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IPARSERIMAGE_H
#define IPARSERIMAGE_H

#include "common/image.h"
#include "engine/iparser.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IParserImage : public IParser
	{
	public:
		virtual ~IParserImage() {}
		virtual void				Read( const char* Path, Image& Image, bool& IsError, bool IsFlipVertical = false, bool IsSwitchRedAndBlueChannels = false ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IPARSERGPUPROGRAM_H
