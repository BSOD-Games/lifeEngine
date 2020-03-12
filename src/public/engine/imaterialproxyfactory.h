//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIAL_PROXY_FACTORY_H
#define IMATERIAL_PROXY_FACTORY_H

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IMaterialProxy;

    //---------------------------------------------------------------------//

    class IMaterialProxyFactory
    {
    public:
        virtual ~IMaterialProxyFactory() {}
        virtual IMaterialProxy*			CreateProxy( const char* NameProxy ) = 0;
        virtual void                    DeleteProxy( IMaterialProxy* MaterialProxy ) = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIAL_PROXY_FACTORY_H
