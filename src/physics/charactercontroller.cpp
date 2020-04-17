//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "global.h"
#include "physics/physicssystem.h"
#include "physics/shapecapsuledescriptor.h"
#include "physics/charactercontroller.h"

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::CharcterController::IncrementReference()
{
	++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::CharcterController::DecrementReference()
{
	--countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete object
// ------------------------------------------------------------------------------------ //
void le::CharcterController::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::CharcterController::GetCountReferences() const
{
	return countReferences;
}

// ------------------------------------------------------------------------------------ //
// Initialize charcter controller
// ------------------------------------------------------------------------------------ //
void le::CharcterController::Initialize( const le::ShapeCapsuleDescriptor& ShapeCapsuleDescriptor )
{
	if ( isInitialize ) return;

	btTransform			transform;
	transform.setIdentity();
	transform.setOrigin( btVector3( 0.f, 0.f, 0.f ) );

	convexShape = new btCapsuleShapeZ( ShapeCapsuleDescriptor.radius, ShapeCapsuleDescriptor.height );
	ghostObject = new btPairCachingGhostObject();

	ghostObject->setCollisionShape( convexShape );
	ghostObject->setCollisionFlags( btCollisionObject::CF_CHARACTER_OBJECT );
	controller = new btKinematicCharacterController( ghostObject, convexShape, 0.05f, btVector3( 0.f, 1.f, 0.f ) );
controller->setFallSpeed(80);

	this->transform = &ghostObject->getWorldTransform();
	isInitialize = true;
}

// ------------------------------------------------------------------------------------ //
// Walk charcter
// ------------------------------------------------------------------------------------ //
void le::CharcterController::Walk( const le::Vector3D_t& WalkDirection )
{
	if ( !isInitialize ) return;
	controller->setWalkDirection( btVector3( WalkDirection.x, WalkDirection.y, WalkDirection.z ) );
}

// ------------------------------------------------------------------------------------ //
// Jump
// ------------------------------------------------------------------------------------ //
void le::CharcterController::Jump( const le::Vector3D_t& JumpForce )
{
	if ( !isInitialize ) return;
	controller->jump( btVector3( JumpForce.x, JumpForce.y, JumpForce.z ) );
}

// ------------------------------------------------------------------------------------ //
// Set velocity
// ------------------------------------------------------------------------------------ //
void le::CharcterController::SetVelocity( const le::Vector3D_t& Velocity )
{
	if ( !isInitialize ) return;
	controller->setLinearVelocity( btVector3( Velocity.x, Velocity.y, Velocity.z ) );
}

// ------------------------------------------------------------------------------------ //
// Can is jump
// ------------------------------------------------------------------------------------ //
bool le::CharcterController::CanJump() const
{
	if ( !isInitialize ) return false;
	return controller->canJump();
}

// ------------------------------------------------------------------------------------ //
// On ground
// ------------------------------------------------------------------------------------ //
bool le::CharcterController::OnGround() const
{
	if ( !isInitialize ) return false;
	return controller->onGround();
}

// ------------------------------------------------------------------------------------ //
// Get velocity
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::CharcterController::GetVelocity() const
{
	if ( !isInitialize ) return Vector3D_t( 0.f, 0.f, 0.f );

	btVector3		velocity = controller->getLinearVelocity();
	return Vector3D_t( velocity.getX(), velocity.getY(), velocity.getZ() );
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::CharcterController::CharcterController() :
	countReferences( 0 ),
	isInitialize( false ),
	convexShape( nullptr ),
	ghostObject( nullptr ),
	controller( nullptr ),
	transform( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::CharcterController::~CharcterController()
{
	if ( isInitialize )
	{
		delete  convexShape;
		delete ghostObject;
		delete controller;
	}
}

// ------------------------------------------------------------------------------------ //
// Set position
// ------------------------------------------------------------------------------------ //
void le::CharcterController::SetPosition( const le::Vector3D_t& Position )
{
	if ( !isInitialize ) return;
	transform->setOrigin( btVector3( Position.x, Position.y, Position.z ) );
}

// ------------------------------------------------------------------------------------ //
// Set rotation
// ------------------------------------------------------------------------------------ //
void le::CharcterController::SetRotation( const le::Quaternion_t& Rotation )
{
	if ( !isInitialize ) return;
	transform->setRotation( btQuaternion( Rotation.x, Rotation.y, Rotation.z, Rotation.w ) );
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::CharcterController::GetPosition() const
{
	btVector3&			position = transform->getOrigin();
	return Vector3D_t( position.getX(), position.getY(), position.getZ() );
}

// ------------------------------------------------------------------------------------ //
// Get rotation
// ------------------------------------------------------------------------------------ //
le::Quaternion_t le::CharcterController::GetRotation() const
{
	btQuaternion		rotation = transform->getRotation();
	return Quaternion_t( rotation.w(), rotation.x(), rotation.y(), rotation.z() );
}
