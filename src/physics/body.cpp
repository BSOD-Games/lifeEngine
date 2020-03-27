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

#include "physics/shapeboxdescriptor.h"
#include "physics/shapecapsuledescriptor.h"
#include "physics/shapecylinderdescriptor.h"
#include "physics/shapemeshdescriptor.h"
#include "physics/shapespheredescriptor.h"
#include "physics/shapeconedescriptor.h"
#include "physics/body.h"

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
	if ( shape )
	{
		switch ( shapeType )
		{
		case ST_BOX:				delete static_cast< btBoxShape* >( shape );				break;
		case ST_CONVEX_HULL:		delete static_cast< btConvexHullShape* >( shape );		break;
		case ST_SPHERE:				delete static_cast< btSphereShape* >( shape );			break;
		case ST_CAPSULE:			delete static_cast< btCapsuleShape* >( shape );			break;
		case ST_CYLINDER:			delete static_cast< btCylinderShape* >( shape );		break;
		case ST_CONE:				delete static_cast< btConeShape* >( shape );			break;
		}
	}

	transform = nullptr;
	rigidBody = nullptr;
	motionState = nullptr;
	shape = nullptr;
	isInitialize = false;
	isActive = false;
	mass = 0.f;
	inertia = Vector3D_t( 0.f, 0.f, 0.f );
	bodyType = BT_STATIC;
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
	bodyType( BT_STATIC ),
	mass( 0.f ),
	inertia( 0.f, 0.f, 0.f ),
	isInitialize( false ),
	isActive( false ),
	rigidBody( nullptr ),
	transform( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::Body::~Body()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Initialize
// ------------------------------------------------------------------------------------ //
bool le::Body::Initialize( const ShapeBoxDescriptor& ShapeBoxDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type )
{
	if ( isInitialize )		Delete();

	btTransform			transform;
	btVector3			inertia( Inertia.x, Inertia.y, Inertia.z );
	transform.setIdentity();
	transform.setOrigin( btVector3( 0.f, 0.f, 0.f ) );

	btBoxShape*			boxShape = new btBoxShape( btVector3( ShapeBoxDescriptor.width / 2.f, ShapeBoxDescriptor.height / 2.f, ShapeBoxDescriptor.depth / 2.f ) );
	if ( Type == BT_STATIC )
	{
		mass = 0.f;
		inertia = btVector3( 0.f, 0.f, 0.f );
		this->inertia = Vector3D_t( 0.f, 0.f, 0.f );
	}
	else
	{
		mass = Mass;
		boxShape->calculateLocalInertia( mass, inertia );
		this->inertia = Vector3D_t( inertia.getX(), inertia.getY(), inertia.getZ() );
	}

	motionState = new btDefaultMotionState( transform );
	btRigidBody::btRigidBodyConstructionInfo			info( mass, motionState, boxShape, inertia );
	rigidBody = new btRigidBody( info );

	this->transform = &rigidBody->getWorldTransform();
	bodyType = Type;
	shapeType = ST_BOX;
	shape = boxShape;
	isInitialize = true;
	isActive = true;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Initialize
// ------------------------------------------------------------------------------------ //
bool le::Body::Initialize( const ShapeCylinderDescriptor& ShapeCylinderDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type )
{
	if ( isInitialize )		Delete();

	btTransform			transform;
	btVector3			inertia( Inertia.x, Inertia.y, Inertia.z );
	transform.setIdentity();
	transform.setOrigin( btVector3( 0.f, 0.f, 0.f ) );

	btCylinderShape*			cylinderShape = new btCylinderShape( btVector3( ShapeCylinderDescriptor.radius / 2.f, ShapeCylinderDescriptor.height / 2.f, ShapeCylinderDescriptor.radius / 2.f ) );
	if ( Type == BT_STATIC )
	{
		mass = 0.f;
		inertia = btVector3( 0.f, 0.f, 0.f );
		this->inertia = Vector3D_t( 0.f, 0.f, 0.f );
	}
	else
	{
		mass = Mass;
		cylinderShape->calculateLocalInertia( mass, inertia );
		this->inertia = Vector3D_t( inertia.getX(), inertia.getY(), inertia.getZ() );
	}

	motionState = new btDefaultMotionState( transform );
	btRigidBody::btRigidBodyConstructionInfo			info( mass, motionState, cylinderShape, inertia );
	rigidBody = new btRigidBody( info );

	this->transform = &rigidBody->getWorldTransform();
	bodyType = Type;
	shapeType = ST_CYLINDER;
	shape = cylinderShape;
	isInitialize = true;
	isActive = true;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Initialize
// ------------------------------------------------------------------------------------ //
bool le::Body::Initialize( const ShapeCapsuleDescriptor& ShapeCapsuleDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type )
{
	if ( isInitialize )		Delete();

	btTransform			transform;
	btVector3			inertia( Inertia.x, Inertia.y, Inertia.z );
	transform.setIdentity();
	transform.setOrigin( btVector3( 0.f, 0.f, 0.f ) );

	btCapsuleShape*			capsuleShape = new btCapsuleShape( ShapeCapsuleDescriptor.radius, ShapeCapsuleDescriptor.height );
	if ( Type == BT_STATIC )
	{
		mass = 0.f;
		inertia = btVector3( 0.f, 0.f, 0.f );
		this->inertia = Vector3D_t( 0.f, 0.f, 0.f );
	}
	else
	{
		mass = Mass;
		capsuleShape->calculateLocalInertia( mass, inertia );
		this->inertia = Vector3D_t( inertia.getX(), inertia.getY(), inertia.getZ() );
	}

	motionState = new btDefaultMotionState( transform );
	btRigidBody::btRigidBodyConstructionInfo			info( mass, motionState, capsuleShape, inertia );
	rigidBody = new btRigidBody( info );

	this->transform = &rigidBody->getWorldTransform();
	bodyType = Type;
	shapeType = ST_CAPSULE;
	shape = capsuleShape;
	isInitialize = true;
	isActive = true;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Get rotation
// ------------------------------------------------------------------------------------ //
bool le::Body::Initialize( const ShapeSphereDescriptor& ShapeSphereDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type )
{
	if ( isInitialize )		Delete();

	btTransform			transform;
	btVector3			inertia( Inertia.x, Inertia.y, Inertia.z );
	transform.setIdentity();
	transform.setOrigin( btVector3( 0.f, 0.f, 0.f ) );

	btSphereShape*			sphereShape = new btSphereShape( ShapeSphereDescriptor.radius );
	if ( Type == BT_STATIC )
	{
		mass = 0.f;
		inertia = btVector3( 0.f, 0.f, 0.f );
		this->inertia = Vector3D_t( 0.f, 0.f, 0.f );
	}
	else
	{
		mass = Mass;
		sphereShape->calculateLocalInertia( mass, inertia );
		this->inertia = Vector3D_t( inertia.getX(), inertia.getY(), inertia.getZ() );
	}

	motionState = new btDefaultMotionState( transform );
	btRigidBody::btRigidBodyConstructionInfo			info( mass, motionState, sphereShape, inertia );
	rigidBody = new btRigidBody( info );

	this->transform = &rigidBody->getWorldTransform();
	bodyType = Type;
	shapeType = ST_SPHERE;
	shape = sphereShape;
	isInitialize = true;
	isActive = true;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Initialize
// ------------------------------------------------------------------------------------ //
bool le::Body::Initialize( const ShapeMeshDescriptor& ShapeMeshDescriptor, float Mass, const Vector3D_t& Inertia, BODY_TYPE Type )
{
	if ( isInitialize )		Delete();

	btTransform			transform;
	btVector3			inertia( Inertia.x, Inertia.y, Inertia.z );
	transform.setIdentity();
	transform.setOrigin( btVector3( 0.f, 0.f, 0.f ) );

	//btTriangleMesh* triangleMesh = new btTriangleMesh();
	/*for (int i=0; i < ShapeMeshDescriptor.countVerteces; i += 3 )
	   {
		   glm::vec3 v1 = ShapeMeshDescriptor.verteces[ i ];
		   glm::vec3 v2 = ShapeMeshDescriptor.verteces[ i + 1 ];
		   glm::vec3 v3 = ShapeMeshDescriptor.verteces[ i + 2 ];

		   btVector3 bv1 = btVector3(v1.x, v1.y, v1.z);
		   btVector3 bv2 = btVector3(v2.x, v2.y, v2.z);
		   btVector3 bv3 = btVector3(v3.x, v3.y, v3.z);

		   triangleMesh->addTriangle(bv1, bv2, bv3);
	   }
	btCollisionShape* meshShape = new btBvhTriangleMeshShape(triangleMesh ,false);*/

	btConvexHullShape* meshShape = new btConvexHullShape( &ShapeMeshDescriptor.verteces[ 0 ].x, ShapeMeshDescriptor.countVerteces, sizeof( Vector3D_t ) );
	meshShape->optimizeConvexHull();

	if ( Type == BT_STATIC )
	{
		mass = 0.f;
		inertia = btVector3( 0.f, 0.f, 0.f );
		this->inertia = Vector3D_t( 0.f, 0.f, 0.f );
	}
	else
	{
		mass = Mass;
		meshShape->calculateLocalInertia( mass, inertia );
		this->inertia = Vector3D_t( inertia.getX(), inertia.getY(), inertia.getZ() );
	}

	motionState = new btDefaultMotionState( transform );
	btRigidBody::btRigidBodyConstructionInfo			info( mass, motionState, meshShape, inertia );
	rigidBody = new btRigidBody( info );

	this->transform = &rigidBody->getWorldTransform();
	bodyType = Type;
	shapeType = ST_CONVEX_HULL;
	shape = meshShape;
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
// Get type
// ------------------------------------------------------------------------------------ //
le::BODY_TYPE le::Body::GetType() const
{
	return bodyType;
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
// Initialize
// ------------------------------------------------------------------------------------ //
bool le::Body::Initialize( const le::ShapeConeDescriptor& ShapeConeDescriptor, float Mass, const le::Vector3D_t& Inertia, le::BODY_TYPE Type )
{
	if ( isInitialize )		Delete();

	btTransform			transform;
	btVector3			inertia( Inertia.x, Inertia.y, Inertia.z );
	transform.setIdentity();
	transform.setOrigin( btVector3( 0.f, 0.f, 0.f ) );

	btConeShape*			coneSphere = new btConeShape( ShapeConeDescriptor.radius, ShapeConeDescriptor.height );
	if ( Type == BT_STATIC )
	{
		mass = 0.f;
		inertia = btVector3( 0.f, 0.f, 0.f );
		this->inertia = Vector3D_t( 0.f, 0.f, 0.f );
	}
	else
	{
		mass = Mass;
		coneSphere->calculateLocalInertia( mass, inertia );
		this->inertia = Vector3D_t( inertia.getX(), inertia.getY(), inertia.getZ() );
	}

	motionState = new btDefaultMotionState( transform );
	btRigidBody::btRigidBodyConstructionInfo			info( mass, motionState, coneSphere, inertia );
	rigidBody = new btRigidBody( info );

	this->transform = &rigidBody->getWorldTransform();
	bodyType = Type;
	shapeType = ST_CONE;
	shape = coneSphere;
	isInitialize = true;
	isActive = true;
	return true;
}
