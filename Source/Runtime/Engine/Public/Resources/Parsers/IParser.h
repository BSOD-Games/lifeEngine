// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef IPARSER_H
#define IPARSER_H

#include <string>

#include "Misc/Object.h"
#include "System/FileSystem.h"

namespace le
{
	class IParser : public Object
	{
	public:
		/* Destructor */
		virtual ~IParser() {}

		/* Set file */
		virtual bool SetFile( FFileHandle InFileHandle ) = 0;
	};
}

#endif // !IPARSER_H