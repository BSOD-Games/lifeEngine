// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "Misc/Object.h"

namespace le
{
	class IActor;

	class World : public Object
	{
	public:
		/* Constructor */
		World();

		/* Destructor */
		~World();

		/* Spawn actor */
		void SpawnActor( IActor* InActor );

		/* Kill actor */
		void KillActor( IActor* InActor );

		/* Kill all actors */
		void KillAllActors();

		/* Update */
		void Update();

		/* Render */
		void Render();

	private:
		std::vector< IActor* >			actors;
	};
}

#endif // !WORLD_H
