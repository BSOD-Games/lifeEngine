// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef ACTOR_H
#define ACTOR_H

#include <vector>

#include "Misc/EngineDefines.h"
#include "Misc/Object.h"
#include "Misc/RefCounted.h"
#include "Misc/Types.h"
#include "Math/Math.h"
#include "World/ActorVar.h"
#include "World/ActorFactory.h"

namespace le
{
	enum EActorStatus
	{
		AS_Live,
		AS_Dead
	};

	class Actor : public Object, public RefCounted
	{
	public:
		/* Constructor */
		Actor();

		/* Destructor */
		virtual ~Actor() {}

		/* Initialize */
		virtual void Initialize( FWorldConstRef& InWorld, const std::vector< ActorVar >* InActorVars = nullptr );

		/* Tick */
		virtual void Tick();

		/* Render */
		virtual void Render();

		/* Is initialized */
		FORCEINLINE bool IsInitialized() const								{ return isInitialized; }

		/* Is visible */
		virtual bool IsVisible( FCameraComponentConstRef& InCameraComponent ) const;

		/* Get component */
		template< typename T >
		FORCEINLINE TRefCountPtr< T > GetComponent() const					{ return TRefCountPtr< T >( static_cast< T* >( GetComponent( T::GetTypeName() ).GetPtr() ) ); }

		/* Get actor status */
		FORCEINLINE EActorStatus GetStatus() const							{ return status; }

	protected:
		/* Get component */
		virtual FBaseComponentRef GetComponent( const std::string& InTypeName ) const;

		bool				isInitialized;
		EActorStatus		status;
	};
}

#define DECLARE_ACTOR( Class )		FORCEINLINE static void RegisterActor()		{ le::GActorFactory->Register( #Class, []() -> le::FActorRef { return le::FActorRef( new Class() ); } ); }

#endif // !ACTOR_H
