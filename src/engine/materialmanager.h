//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include <vector>

#include "engine/imaterialmanager.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IEngine;
    class IFactory;

    //---------------------------------------------------------------------//

    class MaterialManager : public IMaterialManager
    {
    public:
        // IMaterialManager
        virtual void                            RegisterProxyFactory( IFactory* MaterialProxyFactory );
        virtual void                            UnregisterProxyFactory();
        virtual IMaterialProxy*                 CreateProxy( const char* NameProxy );
        virtual void                            RemoveProxy( IMaterialProxy* MaterialProxy );
        virtual void                            RemoveAllProxes();

        virtual UInt32_t                        GetCountProxes() const;
        virtual IMaterialProxy**                GetProxes() const;

        // MaterialManager
        MaterialManager();
        ~MaterialManager();

        void                                    UpdateProxes();

    private:
        IFactory*                               gameProxyFactory;
        std::vector< IMaterialProxy* >          proxes;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIAL_MANAGER_H
