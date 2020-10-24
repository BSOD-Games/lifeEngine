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
	class BaseComponent;
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

		/* Is initialized */
		FORCEINLINE bool IsInitialized() const								{ return isInitialized; }

		/* Is visible */
		virtual bool IsVisible( const CameraComponent& InCameraComponent ) const;

		/* Get component */
		template< typename T >
		FORCEINLINE T* GetComponent() const									{ return static_cast< T* >( GetComponent( T::GetTypeName() ) ); }

		/* Get actor status */
		FORCEINLINE EActorStatus GetStatus() const							{ return status; }

	protected:
		/* Get component */
		virtual BaseComponent* GetComponent( const std::string& InTypeName ) const;

		bool				isInitialized;
		EActorStatus		status;
	};
}

#endif // !ACTOR_H
