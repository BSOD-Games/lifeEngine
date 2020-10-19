// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef IPARSERWORLD_H
#define IPARSERWORLD_H

#include <string>
#include <vector>

#include "World/ActorVar.h"
#include "World/Components/SpriteComponent.h"
#include "IParser.h"

namespace le
{
	class SpriteComponent;
	class Actor;

	struct SWorldObject
	{
		std::string						name;
		std::vector< ActorVar >			actorVars;
	};

	class IParserWorld : public IParser
	{
	public:
		/* Destructor */
		virtual ~IParserWorld() {}

		/* Get sprite components */
		virtual std::vector< SpriteComponent > GetSpriteComponents() const = 0;

		/* Get actors */
		virtual std::vector< SWorldObject > GetObjects() const = 0;
	};
}

#endif // !IPARSERWORLD_H
