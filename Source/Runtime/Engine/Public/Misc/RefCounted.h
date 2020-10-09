// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef REFCOUNTED_H
#define REFCOUNTED_H

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"

namespace le
{
	class RefCounted
	{
	public:
		/* Constructor */
		RefCounted();

		/* Destructor */
		virtual ~RefCounted();

		/* Increment reference count */
		FORCEINLINE void AddRef()
		{
			++countReferences;
		}

		/* Decrement reference count and delete self if no more references */
		void ReleaseRef();

		/* Get reference count */
		FORCEINLINE uint32 GetRefCount() const
		{
			return countReferences;
		}

	private:
		uint32		countReferences;
	};
}

#endif // !REFCOUNTED_H
