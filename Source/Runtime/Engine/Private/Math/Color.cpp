// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Math/Color.h"

/**
 * Constructor
 */
le::SColor::SColor() :
	r( 0 ),
	g( 0 ),
	b( 0 ),
	a( 0 )
{}

/**
 * Constructor
 */
le::SColor::SColor( uint8 InR, uint8 InG, uint8 InB, uint8 InA ) :
	r( InR ),
	g( InG ),
	b( InB ),
	a( InA )
{}
