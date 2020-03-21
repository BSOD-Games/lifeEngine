//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IPHYSICS_SYSTEM_INTERNAL_H
#define IPHYSICS_SYSTEM_INTERNAL_H

#include "physics/iphysicssystem.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IEngine;

    //---------------------------------------------------------------------//

    class IPhysicsSystemInternal : public IPhysicsSystem
    {
    public:
        virtual ~IPhysicsSystemInternal() {}
        virtual bool                Initialize( IEngine* Engine ) = 0;
        virtual void                Update() = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IPHYSICS_SYSTEM_INTERNAL_H
