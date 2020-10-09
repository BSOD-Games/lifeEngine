// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "SettingsContext.h"

/**
 * Constructor
 */
le::SSettingsContext::SSettingsContext( uint32 InRedBits, uint32 InGreenBits, uint32 InBlueBits, uint32 InAlphaBits, uint32 InDepthBits, uint32 InStencilBits, uint32 InMajorVersion, uint32 InMinorVersion, uint32 InAttributeFlags ) :
	redBits( InRedBits ),
	greenBits( InGreenBits ),
	blueBits( InBlueBits ),
	alphaBits( InAlphaBits ),
	depthBits( InDepthBits ),
	stencilBits( InStencilBits ),
	majorVersion( InMajorVersion ),
	minorVersion( InMinorVersion ),
	attributeFlags( InAttributeFlags )
{}