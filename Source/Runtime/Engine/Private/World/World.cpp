// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Misc/EngineDefines.h"
#include "World/IActor.h"
#include "World/World.h"

/**
 * Constructor
 */
le::World::World()
{}

/**
 * Destructor
 */
le::World::~World()
{
	KillAllActors();
}

/**
 * Spawn actor
 */
void le::World::Spawn( IActor* InActor )
{
	LIFEENGINE_ASSERT( InActor );

	InActor->AddRef();
	actors.push_back( InActor );
}

/**
 * Kill actor
 */
void le::World::Kill( IActor* InActor )
{
	LIFEENGINE_ASSERT( InActor );
	for ( uint32 index = 0, count = static_cast< uint32 >( actors.size() ); index < count; ++index )
	{
		IActor*		actor = actors[ index ];
		
		actor->ReleaseRef();
		actors.erase( index + actors.begin() );
		return;
	}
}

/**
 * Kill all actors
 */
void le::World::KillAllActors()
{
	for ( uint32 index = 0, count = static_cast< uint32 >( actors.size() ); index < count; ++index )
		actors[ index ]->ReleaseRef();

	actors.clear();
}

/**
 * Tick
 */
void le::World::Tick()
{
	for ( uint32 index = 0, count = static_cast< uint32 >( actors.size() ); index < count; ++index )
		actors[ index ]->Tick();
}

/**
 * Render
 */
void le::World::Render()
{
	for ( uint32 index = 0, count = static_cast< uint32 >( actors.size() ); index < count; ++index )
		actors[ index ]->Render();
}
