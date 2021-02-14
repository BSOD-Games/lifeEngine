//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ICHARCTER_CONTROLLER_H
#define ICHARCTER_CONTROLLER_H

#include "common/types.h"
#include "engine/ireferenceobject.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct ShapeCapsuleDescriptor;

	//---------------------------------------------------------------------//

	class ICharcterController : public IReferenceObject
	{
	public:
		virtual ~ICharcterController() {}
		virtual void				Initialize( const ShapeCapsuleDescriptor& ShapeCapsuleDescriptor ) = 0;
		virtual void				Walk( const Vector3D_t& WalkDirection ) = 0;
		virtual void				Jump( const Vector3D_t& JumpForce ) = 0;

		virtual void				SetVelocity( const Vector3D_t& Velocity ) = 0;
		virtual void				SetPosition( const Vector3D_t& Position ) = 0;
		virtual void				SetRotation( const Quaternion_t& Rotation ) = 0;

		virtual bool				CanJump() const = 0;
		virtual bool				OnGround() const = 0;
		virtual Vector3D_t			GetVelocity() const = 0;
		virtual Vector3D_t			GetPosition() const = 0;
		virtual Quaternion_t		GetRotation() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define CHARCTERCONTROLLER_INTERFACE_VERSION "LE_CharcterController001"

//---------------------------------------------------------------------//

#endif // !ICHARCTER_CONTROLLER_H
