//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PARSERFONT_FREETYPE_H
#define PARSERFONT_FREETYPE_H

#include "engine/iparserfont.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ParserFontFreeType : public IParserFont
	{
	public:
		// IParser
		virtual const char*				GetVersion() const;
		virtual const char*				GetName() const;
		virtual UInt32_t				GetCountFileExtensions() const;
		virtual const char**			GetFileExtensions() const;
		virtual const char*				GetAuthor() const;

		// IParserFont
		virtual IFont*					Read( const char* Path );

		// ParserFontFreeType
		bool		InitializeFreeType();
		void		UninitializeFreeType();
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !PARSERFONT_FREETYPE_H
