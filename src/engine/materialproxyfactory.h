//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_PROXY_FACTORY_H
#define MATERIAL_PROXY_FACTORY_H

#include "engine/imaterialproxyfactory.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IMaterialProxy;

    //---------------------------------------------------------------------//

    class MaterialProxyFactory : public IMaterialProxyFactory
    {
    public:
        // IMaterialProxyFactory
        virtual IMaterialProxy*			CreateProxy( const char* NameProxy );
        virtual void                    DeleteProxy( IMaterialProxy* MaterialProxy );
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIAL_PROXY_FACTORY_H
