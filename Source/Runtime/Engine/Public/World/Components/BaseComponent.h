// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include "Misc/EngineDefines.h"
#include "Misc/Object.h"
#include "Math/Math.h"
#include "World/Actor.h"

namespace le
{
	class BaseComponent : public Object
	{
	public:
		/* Destructor */
		virtual ~BaseComponent() {}
	};
}

#define DECLARE_COMPONENT( Class )		FORCEINLINE static std::string GetTypeName()		{ return #Class; }

#endif // !BASECOMPONENT_H
