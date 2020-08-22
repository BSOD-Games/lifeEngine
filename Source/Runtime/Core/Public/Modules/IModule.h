// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IMODULE_H
#define IMODULE_H

#include "../Misc/CoreDefines.h"

namespace le
{
	class ICore;

	class IModule
	{
	public:
		/* Destructor IModule */
		virtual ~IModule() {}

		/* Initialize module */
		virtual bool STDCALL Initialize( ICore* Core ) = 0;
		
		/* Deinitialize module */
		virtual void STDCALL Deinitialize() = 0;
	};
}

#endif // !IMODULE_H
