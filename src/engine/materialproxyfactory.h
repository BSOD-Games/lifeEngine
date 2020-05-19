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

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class MaterialProxyFactory : public IMaterialProxyFactory
	{
	public:
		// IMaterialProxyFactory
		virtual void						Register( const char* Name, CreateMaterialProxyFn_t CreateMaterialProxy );
		virtual void						Unregister( const char* Name );
		virtual IMaterialProxy*				Create( const char* Name );

		virtual UInt32_t					GetCountMaterialProxes() const;
		virtual const char*					GetMaterialProxy( UInt32_t Index ) const;
		virtual const char**				GetMaterialProxes() const;

		// MaterialProxyFactory
		MaterialProxyFactory();
		~MaterialProxyFactory();

	private:
		std::vector< const char* >											nameMaterialProxes;
		std::unordered_map< std::string, CreateMaterialProxyFn_t >			materialProxes;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIALPROXY_FACTORY_H

