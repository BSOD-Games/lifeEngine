// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "World/Components/SpriteComponent.h"
#include "World/Actor.h"
#include "Resources/Resource.h"

namespace le
{
	class Actor;

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

		/* Clear */
		void Clear();

		/* Get resource type */
		EResourceType GetType() const override;

		/* Operator = */
		FORCEINLINE World& operator=( const World& InRight )
		{
			Clear();

			spriteComponents = InRight.spriteComponents;
			for ( uint32 index = 0, count = static_cast< uint32 >( InRight.actors.size() ); index < count; ++index )
			{
				Actor*		actor = InRight.actors[ index ];
				actor->AddRef();
				actors.push_back( actor );
			}

			return *this;
		}

	private:
		std::vector< SpriteComponent >			spriteComponents;
		std::vector< Actor* >					actors;
	};
}

#endif // !WORLD_H
