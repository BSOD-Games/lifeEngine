//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PARSERTEXTURE_FREEIMAGE_H
#define PARSERTEXTURE_FREEIMAGE_H

#include "engine/iparsertexture.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ParserTextureFreeImage : public IParserTexture
	{
	public:
		// IParser
		virtual const char*				GetVersion() const;
		virtual const char*				GetName() const;
		virtual UInt32_t				GetCountFileExtensions() const;
		virtual const char**			GetFileExtensions() const;
		virtual const char*				GetAuthor() const;

		// IParserTexture	
		virtual ITexture*				Read( const char* Path, IFactory* StudioRenderFactory );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !PARSERTEXTURE_FREEIMAGE_H
