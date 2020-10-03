// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "Misc/Object.h"
#include "Math/Math.h"

namespace le
{
	class Actor;

	class World : public Object
	{
	public:
		/* Constructor */
		World();

		/* Destructor */
		~World();

		/* Template spawn */
		template< typename T > FORCEINLINE void Spawn( const FVector3D& InPosition = FVector3D( 0.f, 0.f, 0.f ) )			
		{ 
			T*		newActor = new T();
			newActor->SetPosition( InPosition );
			
			Spawn( newActor );
		}

		/* Spawn actor */
		void Spawn( Actor* InActor );
		
		/* Kill actor */
		void Kill( Actor* InActor );

		/* Kill all actors */
		void KillAllActors();

		/* Tick */
		void Tick();

		/* Render */
		void Render();

	private:
		std::vector< Actor* >			actors;
	};
}

#endif // !WORLD_H
