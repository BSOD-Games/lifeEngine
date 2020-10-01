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

		/* Template spawn */
		template< typename T > FORCEINLINE void Spawn()			{ Spawn( new T() ); }

		/* Spawn actor */
		void Spawn( IActor* InActor );
		
		/* Kill actor */
		void Kill( IActor* InActor );

		/* Kill all actors */
		void KillAllActors();

		/* Tick */
		void Tick();

		/* Render */
		void Render();

	private:
		std::vector< IActor* >			actors;
	};
}

#endif // !WORLD_H
