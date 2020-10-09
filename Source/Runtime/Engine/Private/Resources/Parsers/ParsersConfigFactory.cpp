// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Logging/LogMacros.h"
#include "Resources/Parsers/ParsersConfigFactory.h"
#include "Resources/Parsers/IParserConfig.h"

/**
 * Register parser
 */
void le::ParsersConfigFactory::Register( const std::vector< std::string >& InSupportedExtensions, FCreateParserConfigFn InCreateParserConfigFunction )
{
	for ( uint32 index = 0, count = static_cast< uint32 >( InSupportedExtensions.size() ); index < count; ++index )
	{
		LIFEENGINE_LOG_INFO( "Engine", "Parser config for [%s] registered", InSupportedExtensions[ index ].c_str() );
		parsers[ InSupportedExtensions[ index ] ] = InCreateParserConfigFunction;
	}
}

/**
 * Get parser by extension
 */
le::IParserConfig* le::ParsersConfigFactory::Get( const std::string& InExtension ) const
{
	auto		it = parsers.find( InExtension );
	if ( it == parsers.end() )
	{
		LIFEENGINE_LOG_INFO( "Engine", "Parser config for [%s] not founded", InExtension.c_str() );
		return nullptr;
	}

	LIFEENGINE_ASSERT( it->second );
	return it->second();
}
