// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Logging/LogMacros.h"
#include "Resources/Parsers/ParsersTexture2DFactory.h"
#include "Resources/Parsers/IParserTexture2D.h"

/**
 * Register parser
 */
void le::ParsersTexture2DFactory::Register( const std::vector< std::string >& InSupportedExtensions, FCreateParserTexture2DFn InCreateParserTexture2DFunction )
{
	for ( uint32 index = 0, count = static_cast< uint32 >( InSupportedExtensions.size() ); index < count; ++index )
	{
		LIFEENGINE_LOG_INFO( "Engine", "Parser texture 2D for [%s] registered", InSupportedExtensions[ index ].c_str() );
		parsers[ InSupportedExtensions[ index ] ] = InCreateParserTexture2DFunction;
	}
}

/**
 * Get parser by extension
 */
le::FIParserTexture2DRef le::ParsersTexture2DFactory::Get( const std::string& InExtension ) const
{
	auto		it = parsers.find( InExtension );
	if ( it == parsers.end() )
	{
		LIFEENGINE_LOG_INFO( "Engine", "Parser texture 2D for [%s] not founded", InExtension.c_str() );
		return nullptr;
	}

	LIFEENGINE_ASSERT( it->second );
	return it->second();
}
