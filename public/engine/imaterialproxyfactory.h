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

	struct MaterialProxyDescriptor;
	class IMaterialProxy;

	//---------------------------------------------------------------------//

	class IMaterialProxyFactory
	{
	public:
        virtual ~IMaterialProxyFactory() {}
		virtual void								Register( const MaterialProxyDescriptor& MaterialProxyDescriptor ) = 0;
		virtual void								Unregister( const char* Name ) = 0;
		virtual IMaterialProxy*						Create( const char* Name ) = 0;

		virtual UInt32_t							GetCountMaterialProxes() const = 0;
		virtual MaterialProxyDescriptor				GetMaterialProxy( UInt32_t Index ) const = 0;
		virtual MaterialProxyDescriptor*			GetMaterialProxes() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIALPROXYFACTORY_H

