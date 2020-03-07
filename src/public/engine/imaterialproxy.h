//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIAL_PROXY_H
#define IMATERIAL_PROXY_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IMaterialProxyVar;

    //---------------------------------------------------------------------//

    class IMaterialProxy
    {
    public:
        virtual ~IMaterialProxy() {}
        virtual void                        AddVar( IMaterialProxyVar* MaterialProxyVar ) = 0;
        virtual void                        RemoveVar( UInt32_t Index ) = 0;
        virtual void                        Update( UInt32_t DeltaTime ) = 0;

        virtual const char*                 GetName() const = 0;
        virtual UInt32_t                    GetCountVars() const = 0;
        virtual IMaterialProxyVar**         GetVars() const = 0;
        virtual IMaterialProxyVar*          GetVar( UInt32_t Index ) const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIAL_PROXY_H
