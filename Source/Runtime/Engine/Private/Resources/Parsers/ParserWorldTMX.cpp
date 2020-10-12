// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include "Misc/EngineGlobals.h"
#include "Logging/LogMacros.h"
#include "System/FileSystem.h"
#include "Resources/ResourceSystem.h"
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
	if ( !tmxMap->loadFromString( levelData, "./" ) )
	{
		delete tmxMap;
		return false;
	}

	this->tmxMap = tmxMap;

	// Loading tilesets from TMX
	const std::vector< tmx::Tileset >&		tmxTilesets = tmxMap->getTilesets();
	const tmx::Vector2u&					mapTileSize = tmxMap->getTileSize();
	for ( uint32 index = 0, count = static_cast< uint32 >( tmxTilesets.size() ); index < count; ++index )
	{
		const tmx::Tileset&		tmxTileset = tmxTilesets[ index ];
		tmx::Vector2u			tmxTilesetSize = tmxTileset.getImageSize();
		tmx::Vector2u			tmxTileSize = tmxTileset.getTileSize();
		int32					countCulumns = tmxTilesetSize.x / tmxTileSize.x;
		int32					countRows = tmxTilesetSize.y / tmxTileSize.y;

		STMXTileset					tileset;
		tileset.firstGID = tmxTileset.getFirstGID();
		tileset.lastGID = tmxTileset.getLastGID();
		tileset.material = Cast< Material >( GResourceSystem->FindResource( tmxTileset.getImagePath(), RT_Material ) );
		tileset.tileSize = FVector2D( tmxTileSize.x, tmxTileSize.y );
		tileset.tileOffset = FVector2D( ( float ) tmxTileSize.x / mapTileSize.x, ( float ) tmxTileSize.y / mapTileSize.y );

		// Precalculates rectangles in tileset
		// The Y pass starts from the end, because the Tiled Map Editor scores from the top
		for ( int32 y = countRows-1; y >= 0; --y )
			for ( int32 x = 0; x < countCulumns; ++x )
			{			
				FSRectFloat		rect;
				rect.bottom = static_cast< float >( y * tmxTileSize.y );
				rect.left = static_cast< float >( x * tmxTileSize.x );
				rect.width = static_cast< float >( tmxTileSize.x );
				rect.height = static_cast< float >( tmxTileSize.y );

				// Convert texture rect in range from 0 to 1
				rect.left /= tmxTilesetSize.x;
				rect.bottom /= tmxTilesetSize.y;
				rect.width /= tmxTilesetSize.x;
				rect.height /= tmxTilesetSize.y;
				
				tileset.textureRects.push_back( rect );
			}

		tilesets.push_back( tileset );
	}

	return true;
}

/**
 * Get sprite components on level
 */
std::vector< le::SpriteComponent > le::ParserWorldTMX::GetSpriteComponents() const
{
	if ( !tmxMap )		return std::vector< SpriteComponent >();
	
	tmx::Map*									tmxMap = static_cast< tmx::Map* >( this->tmxMap );
	const std::vector< tmx::Layer::Ptr >&		tmxLayers = tmxMap->getLayers();
	std::vector< SpriteComponent >				spriteComponents;

	// Getting tiles from map
	const tmx::Vector2u&		mapSize = tmxMap->getTileCount();
	const tmx::Vector2u&		mapTileSize = tmxMap->getTileSize();
	for ( uint32 indexLayer = 0, countLayers = static_cast< uint32 >( tmxLayers.size() ); indexLayer < countLayers; ++indexLayer )
		if ( tmxLayers[ indexLayer ]->getType() == tmx::Layer::Type::Tile )
		{
			tmx::TileLayer*									tmxLayer = static_cast< tmx::TileLayer* >( tmxLayers[ indexLayer ].get() );
			const std::vector< tmx::TileLayer::Tile >&		tmxTiles = tmxLayer->getTiles();	
			
			float		x = 0.f,	nextX = 0.5f;
			float		y = 0.f,	nextY = 0.5f;
			float		maxX = -( mapSize.x / 2.f );		
			for ( int32 indexTitle = static_cast< int32 >( tmxTiles.size() - 1 ); indexTitle >= 0; --indexTitle )
			{
				const tmx::TileLayer::Tile&		tile = tmxTiles[ indexTitle ];
				if ( tile.ID != 0 )
				{
					STMXTileset			tileset = { 0, 0, FVector2D( 0.f, 0.f ), FVector2D( 0.f, 0.f ), nullptr };
					FSRectFloat			rect;
					SpriteComponent		spriteComponent;

					// Find material for tile
					for ( uint32 indexTileset = 0, countTilesets = static_cast< uint32 >( tilesets.size() ); indexTileset < countTilesets; ++indexTileset )
					{
						const STMXTileset&		tmpTileset = tilesets[ indexTileset ];
						if ( tile.ID < tmpTileset.firstGID || tile.ID > tmpTileset.lastGID )
							continue;
						
						uint32		id = tile.ID - tmpTileset.firstGID;
						tileset = tmpTileset;
						rect = tmpTileset.textureRects[ id ];
						break;
					}

					spriteComponent.SetMaterial( tileset.material ? tileset.material : Cast< Material >( GResourceSystem->FindDefaultResource( RT_Material ) ) );
					spriteComponent.SetLocalPosition( FVector3D( x * mapTileSize.x / tileset.tileOffset.x, y * mapTileSize.y / tileset.tileOffset.y, indexLayer ) );
					spriteComponent.SetSize( FVector2D( mapTileSize.x, mapTileSize.y ) );				
					if ( tileset.material )
						spriteComponent.SetTextureRect( rect );		
					
					spriteComponents.push_back( spriteComponent );
				}

				x -= 0.5f; 
				y += 0.5f;
				if ( x <= maxX )
				{
					x = nextX; 
					y = nextY;
					
					nextX += 0.5f; 
					nextY += 0.5f; 
					maxX += 0.5f;
				}
			}
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
	tilesets.clear();

	tmxMap = nullptr;
}
