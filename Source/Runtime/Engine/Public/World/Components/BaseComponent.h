// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include "Misc/EngineDefines.h"
#include "Misc/Object.h"
#include "Math/Math.h"
#include "World/Actor.h"

namespace le
{
	class BaseComponent : public Object
	{
	public:
		/* Constructor */
		BaseComponent();

		/* Destructor */
		virtual ~BaseComponent() {}

		/* Set actor */
		FORCEINLINE void SetActor( Actor* InActor )
		{
			if ( actor )	actor->ReleaseRef();

			actor = InActor;
			if ( InActor )	InActor->AddRef();
		}

		/* Set local position */
		// TODO: Implement Trasformation class
		FORCEINLINE void SetLocalPosition( const FVector3D& InPositin )			{ position = InPositin; }

		/* Get actor */
		FORCEINLINE Actor* GetActor() const										{ return actor; }

		/* Get global position */
		FORCEINLINE FVector3D GetGlobalPosition() const
		{
			if ( actor )		return actor->GetPosition() + position;
			else				return position;
		}

		/* Get local position */
		FORCEINLINE const FVector3D& GetLocalPosition() const					{ return position; }

	protected:
		Actor*			actor;
		FVector3D		position;
	};
}

#endif // !BASECOMPONENT_H
