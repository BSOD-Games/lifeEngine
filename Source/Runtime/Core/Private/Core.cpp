// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Core.h"

/**
 * Constructor core
 */
le::Core::Core()
{}

/**
 * Destructor core
 */
le::Core::~Core()
{}

/**
 * Initialize
 */
bool le::Core::Initialize()
{
	return true;
}

/**
 * Register interface
 */
void le::Core::RegisterInterface( const SGuid& InGuid, CreateObjectFn InCreateObjectFn )
{
}

/**
 * Unregister interface
 */
void le::Core::UnregisterInterface( const SGuid& InGuid )
{
}

/**
 * Create object
 */
void* le::Core::CreateObject( const SGuid& InGuid )
{
	return nullptr;
}