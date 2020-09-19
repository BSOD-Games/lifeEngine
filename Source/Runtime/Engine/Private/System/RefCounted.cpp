// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "System/RefCounted.h"

/**
 * Constructor
 */
le::RefCounted::RefCounted() :
	countReferences( 0 )
{}

/**
 * Destructor
 */
le::RefCounted::~RefCounted()
{}

/**
 * Decrement reference count and delete self if no more references
 */
void le::RefCounted::ReleaseRef()
{
	if ( !countReferences )
	{
		delete this;
		return;
	}

	--countReferences;
}
