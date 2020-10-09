// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include <tmxlite/Map.hpp>

#include "Misc/EngineGlobals.h"
#include "System/FileSystem.h"
#include "ParserWorldTMX.h"

/**
 * Constructor
 */
le::ParserWorldTMX::ParserWorldTMX() :
	tmxMap( nullptr )
{}

/**
 * Destructor
 */
le::ParserWorldTMX::~ParserWorldTMX()
{
	Clear();
}

/**
 * Set file
 */
bool le::ParserWorldTMX::SetFile( FFileHandle InFileHandle )
{
	LIFEENGINE_ASSERT( InFileHandle );
	if ( IsOppened() )		Clear();

	std::string			levelData;
	GFileSystem->ReadLineFromFile( InFileHandle, levelData, '\0' );
	if ( levelData.empty() )	return false;
	
	tmx::Map*		tmxMap = new tmx::Map();
	if ( !tmxMap->loadFromString( levelData, GFileSystem->GetRootPath().GetFullPath() ) )
	{
		delete tmxMap;
		return false;
	}

	this->tmxMap = tmxMap;
	return true;
}

/**
 * Get sprite components on level
 */
std::vector< le::SpriteComponent > le::ParserWorldTMX::GetSpriteComponents() const
{
	if ( !tmxMap )		return std::vector< SpriteComponent >();
	
	tmx::Map*		tmxMap = static_cast< tmx::Map* >( this->tmxMap );
	const auto&		tmxLayers = tmxMap->getLayers();
	std::vector< SpriteComponent >			spriteComponents;

	for ( uint32 index = 0, count = static_cast< uint32 >( tmxLayers.size() ); index < count; ++index )
		if ( tmxLayers[ index ]->getType() == tmx::Layer::Type::Tile )
		{
			// TODO: [yehor.pohuliaka] Implement this
		}

	return spriteComponents;
}

/**
 * Get actors on level 
 */
std::vector< le::Actor* > le::ParserWorldTMX::GetActors() const
{
	// TODO: [yehor.pohuliaka] Implement this
	return std::vector<Actor*>();
}

/**
 * Clear
 */
void le::ParserWorldTMX::Clear()
{
	if ( !tmxMap )		return;
	delete static_cast< tmx::Map* >( tmxMap );
	tmxMap = nullptr;
}
