// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IACTOR_H
#define IACTOR_H

#include "Misc/Object.h"

namespace le
{
	class IActor : public Object
	{
	public:
		/* Destructor */
		virtual ~IActor() {}

		/* Tick */
		virtual void Tick() = 0;

		/* Render */
		virtual void Render() = 0;
	};
}

#endif // !IACTOR_H
