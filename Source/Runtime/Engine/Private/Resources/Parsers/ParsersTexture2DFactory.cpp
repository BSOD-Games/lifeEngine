// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Resources/Parsers/ParsersTexture2DFactory.h"
#include "Resources/Parsers/IParserTexture2D.h"

/**
 * Register parser
 */
void le::ParsersTexture2DFactory::Register( IParserTexture2D* InParserTexture2D )
{
	parsers.insert( InParserTexture2D );
}

/**
 * Get parser by extension
 */
le::IParserTexture2D* le::ParsersTexture2DFactory::Get( const std::string& InExtension ) const
{
	for ( auto it = parsers.begin(), itEnd = parsers.end(); it != itEnd; ++it )
		if ( ( *it )->IsSupportedExtension( InExtension ) )
			return *it;

	return nullptr;
}
