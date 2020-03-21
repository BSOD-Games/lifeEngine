//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIAL_MANAGER_H
#define IMATERIAL_MANAGER_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IMaterialProxy;
    class IFactory;

    //---------------------------------------------------------------------//

    class IMaterialManager
    {
    public:
        virtual ~IMaterialManager() {}
        virtual void                            RegisterProxyFactory( IFactory* MaterialProxyFactory ) = 0;
        virtual void                            UnregisterProxyFactory() = 0;
        virtual IMaterialProxy*                 CreateProxy( const char* NameProxy ) = 0;
        virtual void                            RemoveProxy( IMaterialProxy* MaterialProxy ) = 0;
        virtual void                            RemoveAllProxes() = 0;

        virtual UInt32_t                        GetCountProxes() const = 0;
        virtual IMaterialProxy**                GetProxes() const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIAL_MANAGER_H
