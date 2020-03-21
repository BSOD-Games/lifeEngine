//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef BODY_H
#define BODY_H

#include "physics/ibody.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class Body : public IBody
    {
    public:
        // IReferenceObject
        virtual void                    IncrementReference();
        virtual void                    DecrementReference();
        virtual void                    Release();
        virtual UInt32_t                GetCountReferences() const;

        // IBody
        virtual void                    Delete();
        virtual void                    SetPosition(const Vector3D_t &Position);
        virtual void                    ApplyImpulse(const Vector3D_t &Factor, const Vector3D_t &CenterBody);
        virtual void                    ApplyForce(const Vector3D_t &Factor, const Vector3D_t &CenterBody);
        virtual void                    FreezeRotation(bool X, bool Y, bool Z);
        virtual const Vector3D_t&       GetPosition() const;
        virtual const Quaternion_t&     GetRotation() const;

    private:
        UInt32_t        countReferences;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // IBODY_H
