//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IBODY_H
#define IBODY_H

#include "common/types.h"
#include "engine/ireferenceobject.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IBody : public IReferenceObject
    {
    public:
        virtual ~IBody() {}
        virtual void                Delete() = 0;

        virtual void                SetPosition( const Vector3D_t& Position ) = 0;
        virtual void                ApplyImpulse( const Vector3D_t& Factor, const Vector3D_t& CenterBody = Vector3D_t( 0.f ) ) = 0;
        virtual void                ApplyForce( const Vector3D_t& Factor, const Vector3D_t& CenterBody = Vector3D_t( 0.f ) ) = 0;
        virtual void                FreezeRotation( bool X, bool Y, bool Z ) = 0;

        virtual const Vector3D_t&   GetPosition() const = 0;
        virtual const Quaternion_t& GetRotation() const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define BODY_INTERFACE_VERSION "LE_Body001"

//---------------------------------------------------------------------//

#endif // !IBODY_H
