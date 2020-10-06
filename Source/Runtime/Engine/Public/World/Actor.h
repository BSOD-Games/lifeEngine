// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef ACTOR_H
#define ACTOR_H

#include "Misc/EngineDefines.h"
#include "Misc/Object.h"
#include "Math/Math.h"

namespace le
{
	class Actor : public Object
	{
	public:
		/* Constructor */
		Actor();

		/* Destructor */
		virtual ~Actor() {}

		/* Initialize */
		virtual void Initialize();

		/* Tick */
		virtual void Tick();

		/* Render */
		virtual void Render();

		/* Set position */
		FORCEINLINE void SetPosition( const FVector3D& InPosition )			{ position = InPosition; }

		/* Is initialized */
		FORCEINLINE bool IsInitialized() const								{ return isInitialized; }

		/* Get position */
		FORCEINLINE const FVector3D& GetPosition()							{ return position; }

	protected:
		bool				isInitialized;
		FVector3D			position;		
	};
}

#endif // !ACTOR_H