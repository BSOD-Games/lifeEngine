// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef PARSERSMATERIALFACTORY_H
#define PARSERSMATERIALFACTORY_H

#include <string>
#include <unordered_map>

#include "Misc/Object.h"

namespace le
{
	class IParserMaterial;
	typedef IParserMaterial*			( *FCreateParserMaterialFn )( );

	class ParsersMaterialFactory : public Object
	{
	public:
		/* Register parser */
		void Register( const std::vector< std::string >& InSupportedExtensions, FCreateParserMaterialFn InCreateParserMaterialFunction );

		/* Get parser by extension */
		IParserMaterial* Get( const std::string& InExtension ) const;

	private:
		mutable std::unordered_map< std::string, FCreateParserMaterialFn >			parsers;
	};
}

#endif // !PARSERSMATERIALFACTORY_H
