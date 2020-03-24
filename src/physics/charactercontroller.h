//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef CHARCTER_CONTROLLER_H
#define CHARCTER_CONTROLLER_H

#include "physics/icharactercontroller.h"

//---------------------------------------------------------------------//

class btTransform;
class btConvexShape;
class btPairCachingGhostObject;
class btKinematicCharacterController;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class CharcterController : public ICharcterController
	{
	public:
		// IReferenceObject
		virtual void				IncrementReference();
		virtual void				DecrementReference();
		virtual void				Release();

		virtual UInt32_t			GetCountReferences() const;

		// ICharcterController
		virtual void				Initialize( const ShapeCapsuleDescriptor& ShapeCapsuleDescriptor );
		virtual void				Walk( const Vector3D_t& WalkDirection );
		virtual void				Jump( const Vector3D_t& JumpForce );

		virtual void				SetVelocity( const Vector3D_t& Velocity );
		virtual void				SetPosition( const Vector3D_t& Position );
		virtual void				SetRotation( const Quaternion_t& Rotation );

		virtual bool				CanJump() const;
		virtual bool				OnGround() const;
		virtual Vector3D_t			GetVelocity() const;
		virtual Vector3D_t			GetPosition() const;
		virtual Quaternion_t		GetRotation() const;

		// CharcterController
		CharcterController();
		~CharcterController();

		inline btPairCachingGhostObject*			GetCollisionObject() const	{ return ghostObject; }
		inline btKinematicCharacterController*		GetHandle() const			{ return controller; }

	private:
		bool								isInitialize;
		le::UInt32_t						countReferences;

		btTransform*						transform;
		btConvexShape*						convexShape;
		btPairCachingGhostObject*			ghostObject;
		btKinematicCharacterController*		controller;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !CHARCTER_CONTROLLER_H
