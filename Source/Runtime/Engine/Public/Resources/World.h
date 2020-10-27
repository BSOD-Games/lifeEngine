// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "Misc/Types.h"
#include "World/Components/SpriteComponent.h"
#include "World/Actor.h"
#include "Resources/Resource.h"

namespace le
{
	class World : public Resource
	{
	public:
		/* Constructor */
		World();

		/* Constructor of copy */
		World( const World& InCopy );

		/* Destructor */
		~World();

		/* Serialize resource */
		bool Serialize( const Path& InPath ) override;

		/* Deserialize resource */
		bool Deserialize( const Path& InPath ) override;

		/* Update */
		void Tick();

		/* Render */
		void Render();

		/* Template spawn */
		template< typename T > FORCEINLINE void Spawn( const FVector3D& InPosition = FVector3D( 0.f, 0.f, 0.f ) )
		{
			TRefCountPtr< T >		newActor( new T() );
			newActor->SetPosition( InPosition );

			Spawn( FActorRef( static_cast< Actor* >( newActor.GetPtr() ) ) );
		}

		/* Spawn actor */
		void Spawn( FActorConstRef& InActor );

		/* Kill actor */
		void Kill( FActorConstRef& InActor );

		/* Kill all actors */
		void KillAllActors();

		/* Clear */
		void Clear();

		/* Get resource type */
		EResourceType GetType() const override;

		/* Operator = */
		FORCEINLINE World& operator=( const World& InRight )
		{
			Clear();

			spriteComponents = InRight.spriteComponents;
			actors = InRight.actors;
			return *this;
		}

	private:
		std::vector< FSpriteComponentRef >		spriteComponents;
		std::vector< FActorRef >				actors;
	};
}

#endif // !WORLD_H
