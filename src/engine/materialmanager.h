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

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class MaterialManager : public IMaterialManager
    {
    public:
        // IMaterialManager
        virtual void                    RegisterProxy( IMaterialProxy* MaterialProxy );
        virtual void                    UnregisterProxy( IMaterialProxy* MaterialProxy );
        virtual void                    UnregisterProxy( UInt32_t Index );
        virtual void                    ClearProxes();

        virtual UInt32_t                GetCountProxes() const;
        virtual IMaterialProxy**        GetProxes() const;
        virtual IMaterialProxy*         GetProxy( UInt32_t Index ) const;

        // MaterialManager
        MaterialManager();
        ~MaterialManager();

        void                            Update( UInt32_t DeltaTime );

    private:
        std::vector< IMaterialProxy* >          proxes;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIAL_MANAGER_H
