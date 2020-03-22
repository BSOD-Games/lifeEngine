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

	struct ShapeBoxDescriptor;
	struct ShapeCylinderDescriptor;
	struct ShapeCapsuleDescriptor;
	struct ShapeSphereDescriptor;
	struct ShapeMeshDescriptor;
	struct ShapeConeDescriptor;

	//---------------------------------------------------------------------//

	enum BODY_TYPE
	{
		BT_STATIC,
		BT_DYNAMIC
	};

	//---------------------------------------------------------------------//

    class IBody : public IReferenceObject
    {
    public:
        virtual ~IBody() {}
		virtual bool                Initialize( const ShapeBoxDescriptor& ShapeBoxDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type = BT_STATIC ) = 0;
		virtual bool                Initialize( const ShapeCylinderDescriptor& ShapeCylinderDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type = BT_STATIC ) = 0;
		virtual bool                Initialize( const ShapeCapsuleDescriptor& ShapeCapsuleDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type = BT_STATIC ) = 0;
		virtual bool                Initialize( const ShapeSphereDescriptor& ShapeSphereDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type = BT_STATIC ) = 0;
		virtual bool                Initialize( const ShapeConeDescriptor& ShapeConeDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type = BT_STATIC ) = 0;
		virtual bool                Initialize( const ShapeMeshDescriptor& ShapeMeshDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type = BT_STATIC ) = 0;
		virtual void				Activate() = 0;
		virtual void                Delete() = 0;

        virtual void                SetPosition( const Vector3D_t& Position ) = 0;
        virtual void                ApplyImpulse( const Vector3D_t& Factor, const Vector3D_t& CenterBody = Vector3D_t( 0.f ) ) = 0;
        virtual void                ApplyForce( const Vector3D_t& Factor, const Vector3D_t& CenterBody = Vector3D_t( 0.f ) ) = 0;
        virtual void                FreezeRotation( bool X, bool Y, bool Z ) = 0;

		virtual BODY_TYPE           GetType() const = 0;
		virtual float               GetMass() const = 0;
		virtual const Vector3D_t&   GetInertia() const = 0;
		virtual Vector3D_t			GetPosition() const = 0;
		virtual Quaternion_t		GetRotation() const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define BODY_INTERFACE_VERSION "LE_Body001"

//---------------------------------------------------------------------//

#endif // !IBODY_H
