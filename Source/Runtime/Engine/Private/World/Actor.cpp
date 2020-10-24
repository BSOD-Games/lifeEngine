// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "World/Actor.h"

/**
 * Constructor
 */
le::Actor::Actor() :
	isInitialized( false ),
	status( AS_Live )
{}

/**
 * Initialize
 */
void le::Actor::Initialize( World* InWorld, const std::vector< ActorVar >* InActorVars )
{
	isInitialized = true;
}

/**
 * Tick of actor
 */
void le::Actor::Tick()
{}

/**
 * Render actor
 */
void le::Actor::Render()
{}

/**
 * Is visible actor
 */
bool le::Actor::IsVisible( const CameraComponent& InCameraComponent ) const
{
	return true;
}

/**
 * Get component
 */
le::BaseComponent* le::Actor::GetComponent( const std::string& InTypeName ) const
{
	return nullptr;
}

