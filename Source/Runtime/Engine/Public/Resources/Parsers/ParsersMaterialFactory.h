// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef PARSERSMATERIALFACTORY_H
#define PARSERSMATERIALFACTORY_H

#include <string>
#include <unordered_map>

#include "Misc/Object.h"
#include "Misc/Types.h"

namespace le
{
	typedef FIParserMaterialRef			( *FCreateParserMaterialFn )( );

	class ParsersMaterialFactory : public Object
	{
	public:
		/* Register parser */
		void Register( const std::vector< std::string >& InSupportedExtensions, FCreateParserMaterialFn InCreateParserMaterialFunction );

		/* Get parser by extension */
		FIParserMaterialRef Get( const std::string& InExtension ) const;

	private:
		mutable std::unordered_map< std::string, FCreateParserMaterialFn >			parsers;
	};
}

#endif // !PARSERSMATERIALFACTORY_H
