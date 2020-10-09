// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Logging/LogMacros.h"
#include "Resources/Parsers/ParsersWorldFactory.h"
#include "Resources/Parsers/IParserWorld.h"

/**
 * Register parser
 */
void le::ParsersWorldFactory::Register( const std::vector<std::string>& InSupportedExtensions, FCreateParserWorldFn InCreateParserWorldFunction )
{
	for ( uint32 index = 0, count = static_cast< uint32 >( InSupportedExtensions.size() ); index < count; ++index )
	{
		LIFEENGINE_LOG_INFO( "Engine", "Parser world for [%s] registered", InSupportedExtensions[ index ].c_str() );
		parsers[ InSupportedExtensions[ index ] ] = InCreateParserWorldFunction;
	}
}

/**
 * Get parser by extension
 */
le::IParserWorld* le::ParsersWorldFactory::Get( const std::string& InExtension ) const
{
	auto		it = parsers.find( InExtension );
	if ( it == parsers.end() )
	{
		LIFEENGINE_LOG_INFO( "Engine", "Parser world for [%s] not founded", InExtension.c_str() );
		return nullptr;
	}

	LIFEENGINE_ASSERT( it->second );
	return it->second();
}