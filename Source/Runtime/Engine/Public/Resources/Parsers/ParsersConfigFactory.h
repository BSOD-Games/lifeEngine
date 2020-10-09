// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef PARSERSCONFIGFACTORY_H
#define PARSERSCONFIGFACTORY_H

#include <string>
#include <vector>
#include <unordered_map>

#include "Misc/Object.h"

namespace le
{
	class IParserConfig;
	typedef IParserConfig*		( *FCreateParserConfigFn )();

	class ParsersConfigFactory : public Object
	{
	public:
		/* Register parser */
		void Register( const std::vector< std::string >& InSupportedExtensions, FCreateParserConfigFn InCreateParserConfigFunction );

		/* Get parser by extension */
		IParserConfig* Get( const std::string& InExtension ) const;

	private:
		mutable std::unordered_map< std::string, FCreateParserConfigFn >			parsers;
	};
}

#endif // !PARSERSCONFIGFACTORY_H
