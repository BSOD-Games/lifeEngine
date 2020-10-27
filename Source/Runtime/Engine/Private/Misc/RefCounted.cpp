// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Misc/EngineDefines.h"
#include "Misc/RefCounted.h"

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
{
	LIFEENGINE_ASSERT( !countReferences );
}

/**
 * Decrement reference count and delete self if no more references
 */
void le::RefCounted::ReleaseRef()
{	 
	if ( !countReferences || !--countReferences )
		delete this;
}
