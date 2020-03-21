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
        virtual void                        SetVar( IMaterialProxyVar* MaterialProxyVar ) = 0;
        virtual void                        ClearVar( const char* NameVar ) = 0;
        virtual void                        ClearAllVars() = 0;
        virtual void                        OnApply( double DeltaTime ) = 0;

        virtual const char*                 GetName() const = 0;
        virtual IMaterialProxyVar*          GetVar( const char* NameVar ) const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIAL_PROXY_H
