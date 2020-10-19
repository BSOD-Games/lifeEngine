// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "World/PlayerController.h"

/**
 * Constructor 
 */
le::PlayerController::PlayerController() :
	actor( nullptr )
{}

/**
 * Destructor
 */
le::PlayerController::~PlayerController()
{
	if ( actor )	actor->ReleaseRef();
}

/**
 * Initialize controller
 */
void le::PlayerController::Initialize()
{}

/**
 * Update controller
 */
void le::PlayerController::Tick()
{}
