//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIALPROXYFACTORY_H
#define IMATERIALPROXYFACTORY_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IMaterialProxy;
	typedef IMaterialProxy*					( *CreateMaterialProxyFn_t )( );

	//---------------------------------------------------------------------//

	class IMaterialProxyFactory
	{
	public:
        virtual ~IMaterialProxyFactory() {}
		virtual void					Register( const char* Name, CreateMaterialProxyFn_t CreateMaterialProxy ) = 0;
		virtual void					Unregister( const char* Name ) = 0;
		virtual IMaterialProxy*			Create( const char* Name ) = 0;

		virtual UInt32_t				GetCountMaterialProxes() const = 0;
		virtual const char*				GetMaterialProxy( UInt32_t Index ) const = 0;
		virtual const char**			GetMaterialProxes() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIALPROXYFACTORY_H

