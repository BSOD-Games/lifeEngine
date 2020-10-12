// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

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
{}

/**
 * Decrement reference count and delete self if no more references
 */
void le::RefCounted::ReleaseRef()
{
	// TODO: [yehor.pohuliaka] - Fix this when the deleting object is not a pointer
	//if ( !countReferences || !--countReferences )		
	//	delete this;
}
