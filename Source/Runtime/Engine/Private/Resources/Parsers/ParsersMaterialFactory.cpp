// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Logging/LogMacros.h"
#include "Resources/Parsers/ParsersMaterialFactory.h"
#include "Resources/Parsers/IParserMaterial.h"

/**
 * Register parser
 */
void le::ParsersMaterialFactory::Register( const std::vector<std::string>& InSupportedExtensions, FCreateParserMaterialFn InCreateParserMaterialFunction )
{
	for ( uint32 index = 0, count = static_cast< uint32 >( InSupportedExtensions.size() ); index < count; ++index )
	{
		LIFEENGINE_LOG_INFO( "Engine", "Parser material for [%s] registered", InSupportedExtensions[ index ].c_str() );
		parsers[ InSupportedExtensions[ index ] ] = InCreateParserMaterialFunction;
	}
}

/**
 * Get parser by extension
 */
le::IParserMaterial* le::ParsersMaterialFactory::Get( const std::string& InExtension ) const
{
	auto		it = parsers.find( InExtension );
	if ( it == parsers.end() )
	{
		LIFEENGINE_LOG_INFO( "Engine", "Parser material for [%s] not founded", InExtension.c_str() );
		return nullptr;
	}

	LIFEENGINE_ASSERT( it->second );
	return it->second();
}
