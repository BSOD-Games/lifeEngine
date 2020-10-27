// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef OBJECT_H
#define OBJECT_H

#include "Misc/EngineDefines.h"
#include "Misc/RefCountPtr.h"

namespace le
{
	class Object
	{
	public:
		/* Destructor */
		virtual ~Object() {}

		/* Cast to type */
		template< typename T >
		FORCEINLINE static T* Cast( Object* InObject )
		{
			return dynamic_cast< T* >( InObject );
		}

		/* Cast to type */
		template< typename T >
		FORCEINLINE static TRefCountPtr< T > Cast( const TRefCountPtr< Object >& InObject )
		{
			return TRefCountPtr< T >( dynamic_cast< T* >( InObject.GetPtr() ) );
		}
	};
}

#endif // !OBJECT_H
