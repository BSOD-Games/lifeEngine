// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IPARSERCONFIG_H
#define IPARSERCONFIG_H

#include <string>
#include <vector>

#include "Resources/Config.h"
#include "IParser.h"

namespace le
{
	class IParserConfig : public IParser
	{
	public:
		/* Destructor */
		virtual ~IParserConfig() {}

		/* Get groups */
		virtual std::vector< Config::Group > GetGroups() const = 0;
	};
}

#endif // !IPARSERCONFIG_H
