// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Object.hpp>

#include "Misc/EngineGlobals.h"
#include "Logging/LogMacros.h"
#include "System/FileSystem.h"
#include "Resources/ResourceSystem.h"
#include "ParserWorldTMX.h"

/**
 * Convert TMX property type to string
*/
FORCEINLINE std::string Convert_TMXPropertyTypeToString( tmx::Property::Type InTmxPropertyType )
{
	switch ( InTmxPropertyType )
	{
	case tmx::Property::Type::Boolean:		return "Boolean";
	case tmx::Property::Type::Colour:		return "Colour";
	case tmx::Property::Type::File:			return "File";
	case tmx::Property::Type::Float:		return "Float";
	case tmx::Property::Type::Int:			return "Int"; 
	case tmx::Property::Type::Object:		return "Object";
	case tmx::Property::Type::String:		return "String";
	case tmx::Property::Type::Undef:		return "Undef";
	default:								return "Unknown";
	}
}

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
std::vector< le::SWorldObject > le::ParserWorldTMX::GetObjects() const
{
	if ( !tmxMap )	return std::vector< SWorldObject >();

	tmx::Map*									tmxMap = static_cast< tmx::Map* >( this->tmxMap );
	const std::vector< tmx::Layer::Ptr >&		tmxLayers = tmxMap->getLayers();
	std::vector< SWorldObject >					worldObjects;

	// Getting max coord XY in coords system of Tiled Map Editor
	FVector2D					tmxMaxXY;
	{
		const tmx::Vector2u&	mapSize = tmxMap->getTileCount();
		const tmx::Vector2u&	mapTileSize = tmxMap->getTileSize();

		tmxMaxXY.x = ( mapSize.x * mapTileSize.x + mapTileSize.x / 2.f ) / 2.f;
		tmxMaxXY.y = mapSize.y * mapTileSize.y - mapTileSize.y / 2.f;
	}

	for ( uint32 indexLayer = 0, countLayers = static_cast< uint32 >( tmxLayers.size() ); indexLayer < countLayers; ++indexLayer )
		if ( tmxLayers[ indexLayer ]->getType() == tmx::Layer::Type::Object )
		{
			tmx::ObjectGroup*						tmxObjectGroup = static_cast< tmx::ObjectGroup* >( tmxLayers[ indexLayer ].get() );
			const std::vector< tmx::Object >&		tmxObjects = tmxObjectGroup->getObjects();
		
			for ( uint32 indexObject = 0, countObjects = static_cast< uint32 >( tmxObjects.size() ); indexObject < countObjects; ++indexObject )
			{
				const tmx::Object&			object = tmxObjects[ indexObject ];
				const std::string&			objectName = object.getName();
				if ( objectName.empty() )	continue;

				SWorldObject		worldObject;
				worldObject.name = objectName;

				// Getting transformation and settings from object
				{
					const tmx::FloatRect&	objectAABB = object.getAABB();
					
					// Getting position
					{
						ActorVar		actorVar;
						actorVar.SetName( "Position" );
						actorVar.SetValueVector3D( MathCartesianToIsometric( tmxMaxXY.x - objectAABB.top, tmxMaxXY.y - objectAABB.left, static_cast< float >( indexLayer ) ) );
						worldObject.actorVars.push_back( actorVar );
					}

					// Getting size
					{
						ActorVar		actorVar;
						actorVar.SetName( "Size" );
						actorVar.SetValueVector2D( FVector2D( objectAABB.width, objectAABB.height ) );
						worldObject.actorVars.push_back( actorVar );
					}

					// If tile id in object more 0 - getting tile for seting actor view
					uint32		tileID = object.getTileID();
					if ( tileID > 0 )
					{
						// Find material for tile
						for ( uint32 indexTileset = 0, countTilesets = static_cast< uint32 >( tilesets.size() ); indexTileset < countTilesets; ++indexTileset )
						{
							const STMXTileset&		tileset = tilesets[ indexTileset ];
							if ( tileID< tileset.firstGID || tileID > tileset.lastGID )
								continue;

							ActorVar		varMaterial;
							varMaterial.SetName( "Material" );
							varMaterial.SetValueMaterial( tileset.material );
							
							ActorVar		varTextureRect;
							varTextureRect.SetName( "TextureRect" );
							varTextureRect.SetValueRectFloat( tileset.textureRects[ tileID - tileset.firstGID ] );
							
							worldObject.actorVars.push_back( varMaterial );
							worldObject.actorVars.push_back( varTextureRect );
							break;
						}
					}
				}

				// Getting all parameters from object
				const std::vector< tmx::Property >&		objectProperties = object.getProperties();
				for ( uint32 indexPropery = 0, countProperties = objectProperties.size(); indexPropery < countProperties; ++indexPropery )
				{
					const tmx::Property&		objectProperty = objectProperties[ indexPropery ];
					ActorVar					actorVar;

					actorVar.SetName( objectProperty.getName() );
					switch ( objectProperty.getType() )
					{
					case tmx::Property::Type::Boolean:		
						actorVar.SetValueBool( objectProperty.getBoolValue() );	
						break;

					case tmx::Property::Type::Colour:		
					{
						const tmx::Colour&		tmxColor = objectProperty.getColourValue();
						actorVar.SetValueColor( SColor( tmxColor.r, tmxColor.g, tmxColor.b, tmxColor.a ) );
						break;
					}

					case tmx::Property::Type::Float:
						actorVar.SetValueFloat( objectProperty.getFloatValue() );
						break;

					case tmx::Property::Type::Int:
						actorVar.SetValueInt( objectProperty.getIntValue() );
						break;

					default:
						LIFEENGINE_LOG_WARNING( "Engine", "Not supported type [%s] in actor var", Convert_TMXPropertyTypeToString( objectProperty.getType() ).c_str() );
						break;
					}

					worldObject.actorVars.push_back( actorVar );
				}
				
				worldObjects.push_back( worldObject );
			}
		}

	return worldObjects;
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
