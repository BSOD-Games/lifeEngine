// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IPARSER_H
#define IPARSER_H

#include <string>
#include "System/Object.h"

namespace le
{
	class IParser : public Object
	{
	public:
		/* Destructor */
		virtual ~IParser() {}

		/* Is supported extension */
		virtual bool IsSupportedExtension( const std::string& InExtension ) const = 0;
	};
}

#endif // !IPARSER_H