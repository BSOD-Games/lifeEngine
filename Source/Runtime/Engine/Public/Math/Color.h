// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef COLOR_H
#define COLOR_H

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"

namespace le
{
	struct SColor
	{
		// Constructor
		SColor();
		SColor( uint8 InR, uint8 InG, uint8 InB, uint8 InA );

		// Operator ==
		FORCEINLINE bool operator==( const SColor& InRight )
		{
			return r == InRight.r && g == InRight.g && b == InRight.b && a == InRight.a;
		}

		// Operator !=
		FORCEINLINE bool operator!=( const SColor& InRight )
		{
			return r != InRight.r && g != InRight.g && b != InRight.b && a != InRight.a;
		}

		// Operator +=
		FORCEINLINE void operator+=( const SColor& InRight )
		{
			r = r + InRight.r < 255 ? r + InRight.r : 255;
			g = g + InRight.g < 255 ? g + InRight.g : 255;
			b = b + InRight.b < 255 ? b + InRight.b : 255;
			a = a + InRight.a < 255 ? a + InRight.a : 255;
		}

		uint8		r;
		uint8		g;
		uint8		b;
		uint8		a;
	};
}

#endif // !COLOR_H
