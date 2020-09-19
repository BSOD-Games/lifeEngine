// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef PARSERSTEXTURE2DFACTORY_H
#define PARSERSTEXTURE2DFACTORY_H

#include <string>
#include <unordered_set>

#include "System/Object.h"

namespace le
{
	class IParserTexture2D;

	class ParsersTexture2DFactory : public Object
	{
	public:
		/* Register parser */
		void Register( IParserTexture2D* InParserTexture2D );

		/* Get parser by extension */
		IParserTexture2D* Get( const std::string& InExtension ) const;

	private:
		mutable std::unordered_set< IParserTexture2D* >		parsers;
	};
}

#endif // !PARSERSTEXTURE2DFACTORY_H
