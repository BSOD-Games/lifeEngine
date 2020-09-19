// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Logging/LogMacros.h"
#include "Resources/Parsers/ParsersTexture2DFactory.h"
#include "Resources/Parsers/IParserTexture2D.h"
#include "Resources/Texture2D.h"

/**
 * Constructor
 */
le::Texture2D::Texture2D()
{}

/**
 * Destructor
 */
le::Texture2D::~Texture2D()
{}

/**
 * Serialize resource
 */
bool le::Texture2D::Serialize( const std::string& InPath )
{
	LIFEENGINE_LOG_WARNING( "Engine", "le::Texture2D::Serialize( const std::string& InPath ) :: Not implemented" );
	return false;
}

/**
 * Deserialize resource
 */
bool le::Texture2D::Deserialize( const std::string& InPath )
{
	return false;
}

/**
 * Get type
 */
le::EResourceType le::Texture2D::GetType() const
{
	return RT_Texture2D;
}
