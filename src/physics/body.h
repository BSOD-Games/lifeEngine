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

	enum SHAPE_TYPE
	{
		ST_BOX,
		ST_CYLINDER,
		ST_CAPSULE,
		ST_SPHERE,
		ST_MESH,
		ST_CONE
	};

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
		virtual bool                    Initialize( const ShapeBoxDescriptor& ShapeBoxDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type );
		virtual bool                    Initialize( const ShapeCylinderDescriptor& ShapeCylinderDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type );
		virtual bool                    Initialize( const ShapeCapsuleDescriptor& ShapeCapsuleDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type );
		virtual bool                    Initialize( const ShapeSphereDescriptor& ShapeSphereDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type );
		virtual bool					Initialize( const ShapeConeDescriptor& ShapeConeDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type );
		virtual bool                    Initialize( const ShapeMeshDescriptor& ShapeMeshDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type );
		virtual void					Activate();
		virtual void                    Delete();

        virtual void                    SetPosition(const Vector3D_t &Position);

        virtual void                    ApplyImpulse(const Vector3D_t &Factor, const Vector3D_t &CenterBody);
        virtual void                    ApplyForce(const Vector3D_t &Factor, const Vector3D_t &CenterBody);
        virtual void                    FreezeRotation(bool X, bool Y, bool Z);

		virtual BODY_TYPE               GetType() const;
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

		BODY_TYPE       bodyType;
		SHAPE_TYPE		shapeType;

		float           mass;
        UInt32_t        countReferences;
		Vector3D_t      inertia;

		std::vector< float >		verteces;
		std::vector< int >			indeces;

		void*			shape;
		btTransform*	transform;
		btMotionState*	motionState;
		btRigidBody*	rigidBody;
	};

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // IBODY_H
