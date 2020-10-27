// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef PARSERWORLDTMX_H
#define PARSERWORLDTMX_H

#include <string>
#include <vector>

#include "Resources/Parsers/IParserWorld.h"

namespace le
{
	class ParserWorldTMX : public IParserWorld
	{
	public:
		/* Constructor */
		ParserWorldTMX();

		/* Destructor */
		~ParserWorldTMX();

		/* Set file */
		bool SetFile( FFileHandle InFileHandle ) override;

		/* Get supported extensions */
		FORCEINLINE static std::vector< std::string > GetSupportedExtensions()
		{
			return { "tmx" };
		}

		/* Is oppened */
		FORCEINLINE bool IsOppened() const
		{
			return tmxMap;
		}

		/* Get sprite components */
		std::vector< FSpriteComponentRef > GetSpriteComponents() const override;

		/* Get actors */
		std::vector< SWorldObject > GetObjects() const override;

	private:

		struct STMXTileset
		{
			uint32							firstGID;
			uint32							lastGID;
			FVector2D						tileSize;
			FVector2D						tileOffset;
			FMaterialRef					material;
			std::vector< FSRectFloat >		textureRects;
		};

		/* Clear parser */
		void Clear();

		void*							tmxMap;
		std::vector< STMXTileset >		tilesets;
	};
}

#endif // !PARSERWORLDTMX_H
