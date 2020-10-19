// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef ACTOR_H
#define ACTOR_H

#include <vector>

#include "Misc/EngineDefines.h"
#include "Misc/Object.h"
#include "Math/Math.h"
#include "World/ActorVar.h"

namespace le
{
	class CameraComponent;
	class World;

	enum EActorStatus
	{
		AS_Live,
		AS_Dead
	};

	class Actor : public Object
	{
	public:
		/* Constructor */
		Actor();

		/* Destructor */
		virtual ~Actor() {}

		/* Initialize */
		virtual void Initialize( World* InWorld, const std::vector< ActorVar >* InActorVars = nullptr );

		/* Tick */
		virtual void Tick();

		/* Render */
		virtual void Render();

		/* Set position */
		FORCEINLINE void SetPosition( const FVector3D& InPosition )			{ position = InPosition; }

		/* Is initialized */
		FORCEINLINE bool IsInitialized() const								{ return isInitialized; }

		/* Is visible */
		virtual bool IsVisible( const CameraComponent& InCameraComponent ) const;

		/* Get position */
		FORCEINLINE const FVector3D& GetPosition()							{ return position; }

		/* Get actor status */
		FORCEINLINE EActorStatus GetStatus() const							{ return status; }

	protected:
		bool				isInitialized;
		FVector3D			position;
		EActorStatus		status;
	};
}

#endif // !ACTOR_H
