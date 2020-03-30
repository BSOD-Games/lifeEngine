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

#include <vector>
#include <btBulletDynamicsCommon.h>
#include "physics/ibody.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

	class Collider;

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
		virtual bool					Create( ICollider* Collider, float Mass, const Vector3D_t& Inertia, bool IsStatic = true );
		virtual void					Activate();
		virtual void                    Delete();

		virtual void                    ApplyImpulse( const Vector3D_t &Factor, const Vector3D_t &CenterBody );
		virtual void                    ApplyForce( const Vector3D_t &Factor, const Vector3D_t &CenterBody );
		virtual void                    FreezeRotation( bool X, bool Y, bool Z );

		virtual void                    SetPosition( const Vector3D_t &Position );
		virtual void					SetRotation( const Quaternion_t& Rotation );

		virtual bool					IsStatic() const;
		virtual ICollider*				GetCollider() const;
		virtual float                   GetMass() const;
		virtual const Vector3D_t&       GetInertia() const;
		virtual Vector3D_t				GetPosition() const;
		virtual Quaternion_t			GetRotation() const;

		// Body
		Body();
		~Body();

		btRigidBody*					GetHandle() const		{ return rigidBody; }

    private:
		bool			isInitialize;
		bool			isActive;
		bool			isStatic;

		float           mass;
        UInt32_t        countReferences;
		Vector3D_t      inertia;

		Collider*		collider;
		btTransform*	transform;
		btMotionState*	motionState;
		btRigidBody*	rigidBody;
	};

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // IBODY_H
