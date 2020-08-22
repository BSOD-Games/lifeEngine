// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef CORE_H
#define CORE_H

#include "Misc/CoreDefines.h"
#include "Modules/IObject.h"

namespace le
{
	class Core
	{
	public:
		/* Constructor Core */
		CORE_API Core();

		/* Destructor Core */
		CORE_API ~Core();

		/* Initialize engine */
		CORE_API bool Initialize();

		/* Register interface */
		CORE_API void RegisterInterface( const SGuid& InGuid, CreateObjectFn InCreateObjectFn );

		/* Unregister interface */
		CORE_API void UnregisterInterface( const SGuid& InGuid );

		/* Create object */
		CORE_API void* CreateObject( const SGuid& InGuid );

		/* Create object */
		template< typename T >
		T* CreateObject( const SGuid& InGuid )
		{
			return static_cast< T* >( CreateObject( InGuid ) );
		}
	};
}

#endif // !CORE_H
