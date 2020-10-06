// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IGAME_H
#define IGAME_H

#include "Misc/Object.h"

namespace le
{
	class IGame : public Object
	{
	public:
		/* Destructor */
		virtual ~IGame() {}

		/* Initialize game */
		virtual bool Initialize() = 0;

		/* Tick game */
		virtual void Tick() = 0;

		/* Render frame */
		virtual void RenderFrame() = 0;
	};
}

#endif // !IGAME_H