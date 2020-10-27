// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Logging/LogMacros.h"
#include "World/Actor.h"
#include "World/ActorFactory.h"

/**
 * Register actor
 */
void le::ActorFactory::Register( const std::string& InName, FCreateActorFn InCreateActorFunction )
{
	actors[ InName ] = InCreateActorFunction;
	LIFEENGINE_LOG_INFO( "Engine", "Actor [%s] registered", InName.c_str() );
}

/**
 * Create actor
 */
le::FActorRef le::ActorFactory::Create( const std::string& InName ) const
{
	auto		it = actors.find( InName );
	if ( it == actors.end() )
	{
		LIFEENGINE_LOG_INFO( "Engine", "Actor [%s] not founded", InName.c_str() );
		return nullptr;
	}

	LIFEENGINE_ASSERT( it->second );
	return it->second();
}
