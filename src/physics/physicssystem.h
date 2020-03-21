//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "physics/iphysicssysteminternal.h"
#include "physicssystemfactory.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IEngine;

    //---------------------------------------------------------------------//

    class PhysicsSystem : public IPhysicsSystemInternal
    {
    public:
        // IPhysicsSystem
        virtual IFactory*           GetFactory() const;

        // IPhysicsSystemInternal
        virtual bool                Initialize( IEngine* Engine );
        virtual void                Update();

        // PhysicsSystem
        PhysicsSystem();
        ~PhysicsSystem();

    private:
        PhysicsSystemFactory        factory;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IPHYSICS_SYSTEM_H
