// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef SETTINGSCONTEXT_H
#define SETTINGSCONTEXT_H

#include "Misc/Types.h"

namespace le
{
	struct SSettingsContext
	{
		enum EContextAttribute
		{
			CA_Default	= 0,
			CA_Core		= 1 << 0,
			CA_Debug	= 1 << 2
		};

		/* Constructor */
		SSettingsContext( uint32 InRedBits = 8, uint32 InGreenBits = 8, uint32 InBlueBits = 8, uint32 InAlphaBits = 8, uint32 InDepthBits = 8, uint32 InStencilBits = 0, uint32 InMajorVersion = 1, uint32 InMinorVersion = 1, uint32 InAttributeFlags = CA_Default );

		uint32			redBits;
		uint32			greenBits;
		uint32			blueBits;
		uint32			alphaBits;
		uint32			depthBits;
		uint32			stencilBits;
		uint32			majorVersion;
		uint32			minorVersion;
		uint32			attributeFlags;
	};
}

#endif // !SETTINGSCONTEXT_H
