//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIALPROXY_FACTORY_H
#define MATERIALPROXY_FACTORY_H

#include <string>
#include <vector>
#include <unordered_map>

#include "engine/imaterialproxyfactory.h"
#include "engine/materialproxydescriptor.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class MaterialProxyFactory : public IMaterialProxyFactory
	{
	public:
		// IMaterialProxyFactory
		virtual void								Register( const MaterialProxyDescriptor& MaterialProxyDescriptor );
		virtual void								Unregister( const char* Name );
		virtual IMaterialProxy*						Create( const char* Name );

		virtual UInt32_t							GetCountMaterialProxes() const;
		virtual MaterialProxyDescriptor				GetMaterialProxy( UInt32_t Index ) const;
		virtual MaterialProxyDescriptor*			GetMaterialProxes() const;

		// MaterialProxyFactory
		MaterialProxyFactory();
		~MaterialProxyFactory();

	private:
		std::vector< MaterialProxyDescriptor >								materialProxes;
		std::unordered_map< std::string, MaterialProxyDescriptor >			materialProxesFind;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIALPROXY_FACTORY_H

