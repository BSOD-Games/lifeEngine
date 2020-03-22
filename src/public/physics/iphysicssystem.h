//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IPHYSICS_SYSTEM_H
#define IPHYSICS_SYSTEM_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IFactory;
	class ICamera;
    class IBody;

    //---------------------------------------------------------------------//

    class IPhysicsSystem
    {
    public:
        virtual ~IPhysicsSystem() {}
        virtual void                AddBody( IBody* Body ) = 0;
        virtual void                RemoveBody( IBody* Body ) = 0;
        virtual void                RemoveAllBodies() = 0;

		virtual void				SetDebugCamera( ICamera* Camera ) = 0;
        virtual void                SetGravity( const Vector3D_t& Gravity ) = 0;

        virtual const Vector3D_t&   GetGravity() const = 0;
        virtual IFactory*           GetFactory() const = 0;
        virtual UInt32_t            GetCountBodes() const = 0;
        virtual IBody*              GetBody( UInt32_t Index ) const = 0;
        virtual IBody**             GetBodies() const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IPHYSICS_SYSTEM_H
