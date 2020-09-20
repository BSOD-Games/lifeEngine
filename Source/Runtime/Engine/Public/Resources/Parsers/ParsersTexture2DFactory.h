// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef PARSERSTEXTURE2DFACTORY_H
#define PARSERSTEXTURE2DFACTORY_H

#include <string>
#include <unordered_map>
#include <vector>

#include "Misc/Object.h"

namespace le
{
	class IParserTexture2D;
	typedef IParserTexture2D*		( *FCreateParserTexture2DFn )();

	class ParsersTexture2DFactory : public Object
	{
	public:
		/* Register parser */
		void Register( const std::vector< std::string >& InSupportedExtensions, FCreateParserTexture2DFn InCreateParserTexture2DFunction );

		/* Get parser by extension */
		IParserTexture2D* Get( const std::string& InExtension ) const;

	private:
		mutable std::unordered_map< std::string, FCreateParserTexture2DFn >			parsers;
	};
}

#endif // !PARSERSTEXTURE2DFACTORY_H
