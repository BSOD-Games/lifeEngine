//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IPARSER_H
#define IPARSER_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IParser
	{
	public:
		virtual ~IParser() {}
		virtual void				Release() = 0;

		virtual const char*			GetVersion() const = 0;
		virtual const char*			GetName() const = 0;
		virtual UInt32_t			GetCountFileExtensions() const = 0;
		virtual const char**		GetFileExtensions() const = 0;
		virtual const char*			GetAuthor() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IPARSER_H
