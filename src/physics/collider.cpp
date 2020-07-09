//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/lifeengine.h"
#include "physics/collider.h"
#include "physics/physicsmodel.h"
#include "physics/shapeboxdescriptor.h"
#include "physics/shapecapsuledescriptor.h"
#include "physics/shapecylinderdescriptor.h"
#include "physics/shapemeshdescriptor.h"
#include "physics/shapespheredescriptor.h"
#include "physics/shapeconedescriptor.h"
#include "physics/shapeconvexhulldescriptor.h"
#include "mathlib/gtc/type_ptr.hpp"

#include "global.h"
#include "engine/iconsolesystem.h"
#include "physics/physicssystem.h"

// ------------------------------------------------------------------------------------ //
// Add shape
// ------------------------------------------------------------------------------------ //
void le::Collider::AddShape( const le::ShapeBoxDescriptor& Shape, const le::Matrix4x4_t& LocalTransormation )
{
	ShapeDescriptor			shapeDescriptor;
	shapeDescriptor.type = ST_BOX;
	shapeDescriptor.shape = new btBoxShape( btVector3( Shape.width / 2.f, Shape.height / 2.f, Shape.depth / 2.f ) );

	btTransform		transform;
	transform.setIdentity();
	transform.setFromOpenGLMatrix( glm::value_ptr( LocalTransormation ) );
	shape.addChildShape( transform, ( btCollisionShape* ) shapeDescriptor.shape );
}

// ------------------------------------------------------------------------------------ //
// Add shape
// ------------------------------------------------------------------------------------ //
void le::Collider::AddShape( const le::ShapeCylinderDescriptor& Shape, const le::Matrix4x4_t& LocalTransormation )
{
	ShapeDescriptor			shapeDescriptor;
	shapeDescriptor.type = ST_CYLINDER;
	shapeDescriptor.shape = new btCylinderShape( btVector3( Shape.radius / 2.f, Shape.height / 2.f, Shape.radius / 2.f ) );

	btTransform		transform;
	transform.setIdentity();
	transform.setFromOpenGLMatrix( glm::value_ptr( LocalTransormation ) );
	shape.addChildShape( transform, ( btCollisionShape* ) shapeDescriptor.shape );
}

// ------------------------------------------------------------------------------------ //
// Add shape
// ------------------------------------------------------------------------------------ //
void le::Collider::AddShape( const le::ShapeCapsuleDescriptor& Shape, const le::Matrix4x4_t& LocalTransormation)
{
	ShapeDescriptor			shapeDescriptor;
	shapeDescriptor.type = ST_CAPSULE;
	shapeDescriptor.shape =  new btCapsuleShape( Shape.radius, Shape.height );

	btTransform		transform;
	transform.setIdentity();
	transform.setFromOpenGLMatrix( glm::value_ptr( LocalTransormation ) );
	shape.addChildShape( transform, ( btCollisionShape* ) shapeDescriptor.shape );
}

// ------------------------------------------------------------------------------------ //
// Add shape
// ------------------------------------------------------------------------------------ //
void le::Collider::AddShape( const le::ShapeSphereDescriptor& Shape, const le::Matrix4x4_t& LocalTransormation )
{
	ShapeDescriptor			shapeDescriptor;
	shapeDescriptor.type = ST_SPHERE;
	shapeDescriptor.shape = new btSphereShape( Shape.radius );

	btTransform		transform;
	transform.setIdentity();
	transform.setFromOpenGLMatrix( glm::value_ptr( LocalTransormation ) );
	shape.addChildShape( transform, ( btCollisionShape* ) shapeDescriptor.shape );
}

// ------------------------------------------------------------------------------------ //
// Add shape
// ------------------------------------------------------------------------------------ //
void le::Collider::AddShape( const le::ShapeMeshDescriptor& Shape, const le::Matrix4x4_t& LocalTransormation )
{
	LIFEENGINE_ASSERT( Shape.verteces && Shape.countVerteces > 0 );

	ColliderMeshDescriptor*			colliderMeshDescriptor = new ColliderMeshDescriptor();
	colliderMeshDescriptor->triangleMesh = new btTriangleMesh();

	// If mesh with indeces
	if ( Shape.indeces && Shape.countIndeces > 0)
		for ( UInt32_t index = 0, offset = 0, count = Shape.countIndeces / 3; index < count; ++index, offset += 3 )
		{
			Vector3D_t&		vertex1 = Shape.verteces[ Shape.indeces[ offset ] ];
			Vector3D_t&		vertex2 = Shape.verteces[ Shape.indeces[ offset + 1 ] ];
			Vector3D_t&		vertex3 = Shape.verteces[ Shape.indeces[ offset + 2 ] ];

			colliderMeshDescriptor->triangleMesh->addTriangle( btVector3( vertex1.x, vertex1.y, vertex1.z ),
															   btVector3( vertex2.x, vertex2.y, vertex2.z),
															   btVector3( vertex3.x, vertex3.y, vertex3.z ) );
		}
	else
		for ( UInt32_t index = 0, offset = 0, count = Shape.countVerteces / 3; index < count; ++index, offset += 3 )
		{
			Vector3D_t&		vertex1 = Shape.verteces[ offset ];
			Vector3D_t&		vertex2 = Shape.verteces[ offset + 1 ];
			Vector3D_t&		vertex3 = Shape.verteces[ offset + 2 ];

			colliderMeshDescriptor->triangleMesh->addTriangle( btVector3( vertex1.x, vertex1.y, vertex1.z ),
															   btVector3( vertex2.x, vertex2.y, vertex2.z),
															   btVector3( vertex3.x, vertex3.y, vertex3.z ) );
		}

	colliderMeshDescriptor->shape = new btBvhTriangleMeshShape( colliderMeshDescriptor->triangleMesh, true );

	ShapeDescriptor			shapeDescriptor;
	shapeDescriptor.type = ST_MESH;
	shapeDescriptor.shape = &colliderMeshDescriptor;

	btTransform		transform;
	transform.setIdentity();
	transform.setFromOpenGLMatrix( glm::value_ptr( LocalTransormation ) );
	shape.addChildShape( transform, colliderMeshDescriptor->shape );
}

// ------------------------------------------------------------------------------------ //
// Add shape
// ------------------------------------------------------------------------------------ //
void le::Collider::AddShape( const le::ShapeConeDescriptor& Shape, const le::Matrix4x4_t& LocalTransormation )
{
	ShapeDescriptor			shapeDescriptor;
	shapeDescriptor.type = ST_CONE;
	shapeDescriptor.shape = new btConeShape( Shape.radius, Shape.height );

	btTransform		transform;
	transform.setIdentity();
	transform.setFromOpenGLMatrix( glm::value_ptr( LocalTransormation ) );
	shape.addChildShape( transform, ( btCollisionShape* ) shapeDescriptor.shape );
}

// ------------------------------------------------------------------------------------ //
// Add shape
// ------------------------------------------------------------------------------------ //
void le::Collider::AddShape( const le::ShapeConvexHullDescriptor& Shape, const le::Matrix4x4_t& LocalTransormation )
{
	ShapeDescriptor			shapeDescriptor;
	shapeDescriptor.type = ST_CONVEX_HULL;
	shapeDescriptor.shape = new btConvexHullShape( &Shape.verteces[ 0 ].x, Shape.countVerteces, sizeof( Vector3D_t ) );
	static_cast< btConvexHullShape* >( shapeDescriptor.shape )->optimizeConvexHull();

	btTransform		transform;
	transform.setIdentity();
	transform.setFromOpenGLMatrix( glm::value_ptr( LocalTransormation ) );
	shape.addChildShape( transform, ( btCollisionShape* ) shapeDescriptor.shape );
}

// ------------------------------------------------------------------------------------ //
// Remove shape
// ------------------------------------------------------------------------------------ //
void le::Collider::RemoveShape( le::UInt32_t Index )
{
	if ( Index >= shapesDescriptors.size() ) return;

	ShapeDescriptor            shapeDescriptor = shapesDescriptors[ Index ];
	switch ( shapeDescriptor.type )
	{
	case ST_BOX:				delete static_cast< btBoxShape* >( shapeDescriptor.shape );						break;
	case ST_CONVEX_HULL:		delete static_cast< btConvexHullShape* >( shapeDescriptor.shape );				break;
	case ST_MESH:				delete static_cast< ColliderMeshDescriptor* >( shapeDescriptor.shape );			break;
	case ST_PHYSICSMODEL:
	{
		PhysicsModel*			physicsModel = static_cast< PhysicsModel* >( shapeDescriptor.shape );
		if ( physicsModel->GetCountReferences() <= 0 )
			physicsModel->Release();
		else
			physicsModel->DecrementReference();
		break;
	}
	case ST_SPHERE:				delete static_cast< btSphereShape* >( shapeDescriptor.shape );					break;
	case ST_CAPSULE:			delete static_cast< btCapsuleShape* >( shapeDescriptor.shape );					break;
	case ST_CYLINDER:			delete static_cast< btCylinderShape* >( shapeDescriptor.shape );				break;
	case ST_CONE:				delete static_cast< btConeShape* >( shapeDescriptor.shape );					break;
	}

	shape.removeChildShapeByIndex( Index );
	shapesDescriptors.erase( shapesDescriptors.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Remove all shapes
// ------------------------------------------------------------------------------------ //
void le::Collider::RemoveAllShapes()
{
	for ( UInt32_t index = 0, count = shapesDescriptors.size(); index < count; ++index )
	{
		ShapeDescriptor            shapeDescriptor = shapesDescriptors[ index ];
		switch ( shapeDescriptor.type )
		{
		case ST_BOX:				delete static_cast< btBoxShape* >( shapeDescriptor.shape );						break;
		case ST_CONVEX_HULL:		delete static_cast< btConvexHullShape* >( shapeDescriptor.shape );				break;
		case ST_MESH:				delete static_cast< ColliderMeshDescriptor* >( shapeDescriptor.shape );			break;
		case ST_PHYSICSMODEL:
		{
			PhysicsModel*			physicsModel = static_cast< PhysicsModel* >( shapeDescriptor.shape );
			if ( physicsModel->GetCountReferences() <= 1 )
				physicsModel->Release();
			else
				physicsModel->DecrementReference();
			break;
		}
		case ST_SPHERE:				delete static_cast< btSphereShape* >( shapeDescriptor.shape );					break;
		case ST_CAPSULE:			delete static_cast< btCapsuleShape* >( shapeDescriptor.shape );					break;
		case ST_CYLINDER:			delete static_cast< btCylinderShape* >( shapeDescriptor.shape );				break;
		case ST_CONE:				delete static_cast< btConeShape* >( shapeDescriptor.shape );					break;
		}

		shape.removeChildShapeByIndex( index );
	}

	shapesDescriptors.clear();
}

// ------------------------------------------------------------------------------------ //
// Get AABB collider
// ------------------------------------------------------------------------------------ //
void le::Collider::GetAABB( Vector3D_t& Min, Vector3D_t& Max ) const
{
	btTransform			transformation;
	btVector3			min, max;
	shape.getAabb( transformation, min, max );

	Min.x = min.getX();
	Min.y = min.getY();
	Min.z = min.getZ();

	Max.x = max.getX();
	Max.y = max.getY();
	Max.z = max.getZ();
}

// ------------------------------------------------------------------------------------ //
// Get count shapes
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Collider::GetCountShapes() const
{
	return shape.getNumChildShapes();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::Collider::Collider() :
	isNeedUpdateScale( false ),
	countReferences( 0 ),
	scale( 1.f, 1.f, 1.f )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::Collider::~Collider()
{
	RemoveAllShapes();
}

// ------------------------------------------------------------------------------------ //
// Register body on update this collider
// ------------------------------------------------------------------------------------ //
void le::Collider::RegisterBody( le::Body* Body )
{
	LIFEENGINE_ASSERT( Body );
	bodies.push_back( Body );
}

// ------------------------------------------------------------------------------------ //
// Unregister body from update this collider
// ------------------------------------------------------------------------------------ //
void le::Collider::UnregisterBody( le::Body* Body )
{
	if ( bodies.empty() ) return;

	for ( UInt32_t index = 0, count = bodies.size(); index < count; ++index )
		if ( bodies[ index ] == Body )
		{
			bodies.erase( bodies.begin() + index );
			return;
		}
}

// ------------------------------------------------------------------------------------ //
// Update collider
// ------------------------------------------------------------------------------------ //
void le::Collider::Update()
{
	if ( !isNeedUpdateScale ) return;	
	shape.setLocalScaling( btVector3( scale.x, scale.y, scale.z ) );

	for ( UInt32_t index = 0, count = bodies.size(); index < count; ++index )
		bodies[ index ]->UpdateLocalInertia();

	isNeedUpdateScale = false;
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::Collider::IncrementReference()
{
	++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::Collider::DecrementReference()
{
	--countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::Collider::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Collider::GetCountReferences() const
{
	return countReferences;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::Collider::ColliderMeshDescriptor::ColliderMeshDescriptor() :
	triangleMesh( nullptr ),
	shape( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::Collider::ColliderMeshDescriptor::~ColliderMeshDescriptor()
{
	if ( triangleMesh )		delete triangleMesh;
	if ( shape )			delete shape;
}

// ------------------------------------------------------------------------------------ //
// Need update scale
// ------------------------------------------------------------------------------------ //
void le::Collider::NeedUpdateScale()
{
	if ( isNeedUpdateScale ) return;

	g_physicsSystem->UpdateCollider( this );
	isNeedUpdateScale = true;
}

// ------------------------------------------------------------------------------------ //
// Scale
// ------------------------------------------------------------------------------------ //
void le::Collider::Scale( const le::Vector3D_t& FactorScale )
{
	scale += FactorScale;
	NeedUpdateScale();
}

// ------------------------------------------------------------------------------------ //
// Set scale
// ------------------------------------------------------------------------------------ //
void le::Collider::SetScale( const le::Vector3D_t& Scale )
{
	scale = Scale;
	NeedUpdateScale();
}

// ------------------------------------------------------------------------------------ //
// Get scale
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Collider::GetScale() const
{
	return scale;
}

// ------------------------------------------------------------------------------------ //
// Add shape
// ------------------------------------------------------------------------------------ //
void le::Collider::AddShape( le::IPhysicsModel* PhysicsModel, const le::Matrix4x4_t& LocalTransormation )
{
	LIFEENGINE_ASSERT( PhysicsModel );
	if ( !PhysicsModel->IsInitializedMesh() ) return;

	le::PhysicsModel*		physicsModel = static_cast< le::PhysicsModel* >( PhysicsModel );
	ShapeDescriptor			shapeDescriptor;
	shapeDescriptor.type = ST_PHYSICSMODEL;
	shapeDescriptor.shape = &physicsModel;

	btTransform		transform;
	transform.setIdentity();
	transform.setFromOpenGLMatrix( glm::value_ptr( LocalTransormation ) );
	shape.addChildShape( transform, ( btCollisionShape* ) physicsModel->GetMesh() );
	physicsModel->IncrementReference();
}
