//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include <vector>

#include "engine/imaterialmanager.h"
#include "engine/materialproxyfactory.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class MaterialManager : public IMaterialManager
    {
    public:
        // IMaterialManager
        virtual void                    RegisterGameProxyFactory( IMaterialProxyFactory* MaterialProxyFactory );
        virtual void                    UnregisterGameProxyFactory();
        virtual IMaterialProxy*         CreateProxy( const char* NameProxy );
        virtual void                    DeleteProxy( IMaterialProxy* MaterialProxy );
        virtual void                    DeleteProxy( UInt32_t Index );
        virtual void                    DeleteAllProxes();

        virtual UInt32_t                GetCountProxes() const;
        virtual IMaterialProxy**        GetProxes() const;
        virtual IMaterialProxy*         GetProxy( UInt32_t Index ) const;

        // MaterialManager
        MaterialManager();
        ~MaterialManager();

        void                            Update( UInt32_t DeltaTime );

    private:
        MaterialProxyFactory                    proxyFactory;
        IMaterialProxyFactory*                  gameProxyFactory;
        std::vector< IMaterialProxy* >          proxes;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIAL_MANAGER_H
