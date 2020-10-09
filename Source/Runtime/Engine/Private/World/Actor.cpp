// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "World/Actor.h"

/**
 * Constructor
 */
le::Actor::Actor() :
	isInitialized( false ),
	position( 0.f, 0.f, 0.f )
{}

/**
 * Initialize
 */
void le::Actor::Initialize()
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


