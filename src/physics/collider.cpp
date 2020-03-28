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
	// Warning: this pointer is not deleted when the object is destroyed!
	btTriangleMesh*				triangleMesh = new btTriangleMesh();
	for ( UInt32_t index = 0; index < Shape.countVerteces; index += 3 )
	{
		glm::vec3&		vertex1 = Shape.verteces[ index ];
		glm::vec3&		vertex2 = Shape.verteces[ index + 1 ];
		glm::vec3&		vertex3 = Shape.verteces[ index + 2 ];

		// TODO: Add indeces to collision mesh
		triangleMesh->addTriangle( btVector3( vertex1.x, vertex1.y, vertex1.z ),
								   btVector3( vertex2.x, vertex2.y, vertex2.z),
								   btVector3( vertex3.x, vertex3.y, vertex3.z ) );
	}

	ShapeDescriptor			shapeDescriptor;
	shapeDescriptor.type = ST_MESH;
	shapeDescriptor.shape = new btBvhTriangleMeshShape( triangleMesh, false );

	btTransform		transform;
	transform.setIdentity();
	transform.setFromOpenGLMatrix( glm::value_ptr( LocalTransormation ) );
	shape.addChildShape( transform, ( btCollisionShape* ) shapeDescriptor.shape );
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
	case ST_MESH:				delete static_cast< btBvhTriangleMeshShape* >( shapeDescriptor.shape );	break;
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
		case ST_MESH:				delete static_cast< btBvhTriangleMeshShape* >( shapeDescriptor.shape );	break;
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
	countReferences( 0 )
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
