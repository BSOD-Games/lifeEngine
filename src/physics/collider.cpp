//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "physics/collider.h"
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
	ColliderMeshDescriptor*			colliderMeshDescriptor = new ColliderMeshDescriptor();
	colliderMeshDescriptor->triangleMesh = new btTriangleMesh();

	for ( UInt32_t index = 0; index < Shape.countIndeces; index += 3 )
	{
		glm::vec3&		vertex1 = Shape.verteces[ Shape.indeces[ index ] ];
		glm::vec3&		vertex2 = Shape.verteces[ Shape.indeces[ index + 1 ] ];
		glm::vec3&		vertex3 = Shape.verteces[ Shape.indeces[ index + 2 ] ];

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
	case ST_BOX:				delete static_cast< btBoxShape* >( shapeDescriptor.shape );				break;
	case ST_CONVEX_HULL:		delete static_cast< btConvexHullShape* >( shapeDescriptor.shape );		break;
	case ST_MESH:				delete static_cast< ColliderMeshDescriptor* >( shapeDescriptor.shape );	break;
	case ST_SPHERE:				delete static_cast< btSphereShape* >( shapeDescriptor.shape );			break;
	case ST_CAPSULE:			delete static_cast< btCapsuleShape* >( shapeDescriptor.shape );			break;
	case ST_CYLINDER:			delete static_cast< btCylinderShape* >( shapeDescriptor.shape );		break;
	case ST_CONE:				delete static_cast< btConeShape* >( shapeDescriptor.shape );			break;
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
		case ST_BOX:				delete static_cast< btBoxShape* >( shapeDescriptor.shape );				break;
		case ST_CONVEX_HULL:		delete static_cast< btConvexHullShape* >( shapeDescriptor.shape );		break;
		case ST_MESH:				delete static_cast< ColliderMeshDescriptor* >( shapeDescriptor.shape );	break;
		case ST_SPHERE:				delete static_cast< btSphereShape* >( shapeDescriptor.shape );			break;
		case ST_CAPSULE:			delete static_cast< btCapsuleShape* >( shapeDescriptor.shape );			break;
		case ST_CYLINDER:			delete static_cast< btCylinderShape* >( shapeDescriptor.shape );		break;
		case ST_CONE:				delete static_cast< btConeShape* >( shapeDescriptor.shape );			break;
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
// Scale
// ------------------------------------------------------------------------------------ //
void le::Collider::Scale( const le::Vector3D_t& FactorScale )
{
	scale += FactorScale;
	shape.setLocalScaling( btVector3( scale.x, scale.y, scale.z ) );
}

// ------------------------------------------------------------------------------------ //
// Set scale
// ------------------------------------------------------------------------------------ //
void le::Collider::SetScale( const le::Vector3D_t& Scale )
{
	scale = Scale;
	shape.setLocalScaling( btVector3( Scale.x, Scale.y, Scale.z ) );
}

// ------------------------------------------------------------------------------------ //
// Get scale
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Collider::GetScale() const
{
	return scale;
}
