// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef IPARSERWORLD_H
#define IPARSERWORLD_H

#include <vector>

#include "World/Components/SpriteComponent.h"
#include "IParser.h"

namespace le
{
	class SpriteComponent;
	class Actor;

	class IParserWorld : public IParser
	{
	public:
		/* Destructor */
		virtual ~IParserWorld() {}

		/* Get sprite components */
		virtual std::vector< SpriteComponent > GetSpriteComponents() const = 0;

		/* Get actors */
		virtual std::vector< Actor* > GetActors() const = 0;
	};
}

#endif // !IPARSERWORLD_H
