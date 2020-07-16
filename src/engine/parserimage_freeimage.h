//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PARSERIMAGE_FREEIMAGE_H
#define PARSERIMAGE_FREEIMAGE_H

#include "engine/iparserimage.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ParserImageFreeImage : public IParserImage
	{
	public:
		// IParser
		virtual const char*				GetVersion() const;
		virtual const char*				GetName() const;
		virtual UInt32_t				GetCountFileExtensions() const;
		virtual const char**			GetFileExtensions() const;
		virtual const char*				GetAuthor() const;

		// IParserImage		
		virtual void					Read( const char* Path, Image& Image, bool& IsError, bool IsFlipVertical = false, bool IsSwitchRedAndBlueChannels = false );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !PARSERIMAGE_FREEIMAGE_H
