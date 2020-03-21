//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PHYSICS_SYSTEM_FACTORY_H
#define PHYSICS_SYSTEM_FACTORY_H

#include "engine/ifactory.h"

namespace le
{
    //---------------------------------------------------------------------//

    class PhysicsSystemFactory : public IFactory
    {
    public:
        // IFactory
        virtual void*			Create( const char* NameInterface );
        virtual void			Delete( void* Object );
    };

    //---------------------------------------------------------------------//
}

#endif // !PHYSICS_SYSTEM_FACTORY_H

