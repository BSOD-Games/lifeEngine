// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef OBJECT_H
#define OBJECT_H

#include "RefCounted.h"

namespace le
{
	class Object : public RefCounted
	{
	public:
		/* Destructor */
		virtual ~Object() {}

		/* Cast to type */
		template< typename T >
		static T* Cast( Object* InObject )
		{
			return dynamic_cast< T* >( InObject );
		}
	};
}

#endif // !OBJECT_H
