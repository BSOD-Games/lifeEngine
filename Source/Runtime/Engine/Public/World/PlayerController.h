// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "Misc/EngineDefines.h"
#include "Misc/Object.h"
#include "World/Actor.h"
#include "World/Components/CameraComponent.h"

namespace le
{
	class PlayerController : public Object
	{
	public:
		/* Constructor */
		PlayerController();

		/* Destructor */
		virtual ~PlayerController();

		/* Initialize controller */
		virtual void Initialize();

		/* Destroy controller */
		virtual void Destroy();

		/* Update player controller */
		virtual void Tick();

		/* Set actor for controlling by the player */
		FORCEINLINE virtual void SetActor( FActorConstRef& InActor )	{ actor = InActor; }

		/* Get an actor controlled by the player */
		FORCEINLINE FActorRef GetActor() const							{ return actor; }

	protected:
		FActorRef				actor;
	};
}

#endif // !PLAYERCONTROLLER_H
