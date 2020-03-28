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

#include <engine/lifeengine.h>
#include "physics/shapeboxdescriptor.h"
#include "physics/shapecapsuledescriptor.h"
#include "physics/shapecylinderdescriptor.h"
#include "physics/shapemeshdescriptor.h"
#include "physics/shapespheredescriptor.h"
#include "physics/shapeconedescriptor.h"
#include "physics/shapeconvexhulldescriptor.h"
#include "physics/body.h"
#include "physics/collider.h"

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::Body::IncrementReference()
{
	++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::Body::DecrementReference()
{
	--countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::Body::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Body::GetCountReferences() const
{
	return countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::Body::Delete()
{
	if ( !isInitialize )	return;
	if ( rigidBody )		delete rigidBody;
	if ( motionState )		delete motionState;
	if ( collider )
	{
		if ( collider->GetCountShapes() <= 1 )
			collider->Release();
		else
			collider->DecrementReference();
	}

	transform = nullptr;
	rigidBody = nullptr;
	motionState = nullptr;
	collider = nullptr;
	isInitialize = false;
	isActive = false;
	isStatic = true;
	mass = 0.f;
	inertia = Vector3D_t( 0.f, 0.f, 0.f );	
}

// ------------------------------------------------------------------------------------ //
// Set position
// ------------------------------------------------------------------------------------ //
void le::Body::SetPosition( const le::Vector3D_t& Position )
{
	if ( !isInitialize ) return;
	transform->setOrigin( btVector3( Position.x, Position.y, Position.z ) );
}

// ------------------------------------------------------------------------------------ //
// Set rotation
// ------------------------------------------------------------------------------------ //
void le::Body::SetRotation( const le::Quaternion_t& Rotation )
{
	if ( !isInitialize ) return;
	transform->setRotation( btQuaternion( Rotation.x, Rotation.y, Rotation.z, Rotation.w ) );
}

// ------------------------------------------------------------------------------------ //
// Apply impulse
// ------------------------------------------------------------------------------------ //
void le::Body::ApplyImpulse( const le::Vector3D_t& Factor, const le::Vector3D_t& CenterBody )
{
	if ( !isInitialize ) return;

	if ( CenterBody == Vector3D_t( 0.f, 0.f, 0.f ) )
		rigidBody->applyCentralImpulse( btVector3( Factor.x, Factor.y, Factor.z ) );
	else
		rigidBody->applyImpulse( btVector3( Factor.x, Factor.y, Factor.z ), btVector3( CenterBody.x, CenterBody.y, CenterBody.z ) );
}

// ------------------------------------------------------------------------------------ //
// Apply force
// ------------------------------------------------------------------------------------ //
void le::Body::ApplyForce( const le::Vector3D_t& Factor, const le::Vector3D_t& CenterBody )
{
	if ( !isInitialize ) return;

	if ( CenterBody == Vector3D_t( 0.f, 0.f, 0.f ) )
		rigidBody->applyCentralForce( btVector3( Factor.x, Factor.y, Factor.z ) );
	else
		rigidBody->applyForce( btVector3( Factor.x, Factor.y, Factor.z ), btVector3( CenterBody.x, CenterBody.y, CenterBody.z ) );
}

// ------------------------------------------------------------------------------------ //
// Freeze rotation
// ------------------------------------------------------------------------------------ //
void le::Body::FreezeRotation( bool X, bool Y, bool Z )
{
	if ( !isInitialize ) return;

	btVector3		freezeAxis( 0.f, 0.f, 0.f );
	if ( X )		freezeAxis.setX( 1.f );
	if ( Y )		freezeAxis.setY( 1.f );
	if ( Z )		freezeAxis.setZ( 1.f );

	rigidBody->setAngularFactor( freezeAxis );
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::Body::GetPosition() const
{
	btVector3&			position = transform->getOrigin();
	return Vector3D_t( position.getX(), position.getY(), position.getZ() );
}

// ------------------------------------------------------------------------------------ //
// Get rotation
// ------------------------------------------------------------------------------------ //
le::Quaternion_t le::Body::GetRotation() const
{
	btQuaternion		rotation = transform->getRotation();
	return Quaternion_t( rotation.w(), rotation.x(), rotation.y(), rotation.z() );
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::Body::Body() :
	isStatic( true ),
	mass( 0.f ),
	inertia( 0.f, 0.f, 0.f ),
	isInitialize( false ),
	isActive( false ),
	rigidBody( nullptr ),
	transform( nullptr ),
	collider( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::Body::~Body()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Create body
// ------------------------------------------------------------------------------------ //
bool le::Body::Create( ICollider* Collider, float Mass, const Vector3D_t &Inertia, bool IsStatic )
{
	if ( isInitialize )		Delete();
	LIFEENGINE_ASSERT( Collider );

	btTransform			transform;
	btVector3			inertia( Inertia.x, Inertia.y, Inertia.z );
	transform.setIdentity();
	transform.setOrigin( btVector3( 0.f, 0.f, 0.f ) );

	collider = ( le::Collider* ) Collider;
	collider->IncrementReference();
	btCollisionShape*			shape = &collider->GetCollisionShape();

	if ( IsStatic )
	{
		mass = 0.f;
		inertia = btVector3( 0.f, 0.f, 0.f );
		this->inertia = Vector3D_t( 0.f, 0.f, 0.f );
	}
	else
	{
		mass = Mass;
		shape->calculateLocalInertia( mass, inertia );
		this->inertia = Vector3D_t( inertia.getX(), inertia.getY(), inertia.getZ() );
	}

	motionState = new btDefaultMotionState( transform );
	btRigidBody::btRigidBodyConstructionInfo			info( mass, motionState, shape, inertia );
	rigidBody = new btRigidBody( info );

	this->transform = &rigidBody->getWorldTransform();
	isStatic = IsStatic;
	isInitialize = true;
	isActive = true;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Activate body
// ------------------------------------------------------------------------------------ //
void le::Body::Activate()
{
	if ( !isInitialize ) return;
	rigidBody->activate();
}

// ------------------------------------------------------------------------------------ //
// Is static body
// ------------------------------------------------------------------------------------ //
bool le::Body::IsStatic() const
{
	return isStatic;
}

// ------------------------------------------------------------------------------------ //
// Get mass
// ------------------------------------------------------------------------------------ //
float le::Body::GetMass() const
{
	return mass;
}

// ------------------------------------------------------------------------------------ //
// Get inertia
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Body::GetInertia() const
{
	return inertia;
}

// ------------------------------------------------------------------------------------ //
// Get collider body
// ------------------------------------------------------------------------------------ //
le::ICollider* le::Body::GetCollider() const
{
	return ( ICollider* ) collider;
}
