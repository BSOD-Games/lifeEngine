// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IOBJECT_H
#define IOBJECT_H

#include "../Misc/CoreDefines.h"
#include "../Misc/Types.h"

namespace le
{
	struct SGuid;
	typedef void*				( *CreateObjectFn )( );

	class IObject
	{
	public:
		/* Descturctor IObject */
		virtual ~IObject() {}

		/* Add reference */
		virtual void STDCALL AddRef() = 0;

		/* Release */
		virtual void STDCALL Release() = 0;

		/* Get GUID of object */
		virtual const SGuid& STDCALL GetGuid() const = 0;
	};	
}

#endif // !IOBJECT_H