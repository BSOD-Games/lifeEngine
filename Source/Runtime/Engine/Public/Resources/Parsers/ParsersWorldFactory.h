// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef PARSERSWORLDFACTORY_H
#define PARSERSWORLDFACTORY_H

#include <string>
#include <vector>
#include <unordered_map>

#include "Misc/Object.h"

namespace le
{
	class IParserWorld;
	typedef IParserWorld*		( *FCreateParserWorldFn )( );

	class ParsersWorldFactory : public Object
	{
	public:
		/* Register parser */
		void Register( const std::vector< std::string >& InSupportedExtensions, FCreateParserWorldFn InCreateParserWorldFunction );

		/* Get parser by extension */
		IParserWorld* Get( const std::string& InExtension ) const;

	private:
		mutable std::unordered_map< std::string, FCreateParserWorldFn >			parsers;
	};
}

#endif // !PARSERSWORLDFACTORY_H
