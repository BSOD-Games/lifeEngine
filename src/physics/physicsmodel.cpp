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

#include "engine/lifeengine.h"
#include "physicsmodel.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::PhysicsModel::PhysicsModel() :
	countReferences( 0 ),
	triangleMesh( nullptr ),
	convexTriangleMesh( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::PhysicsModel::~PhysicsModel()
{
	ClearMesh();
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::PhysicsModel::IncrementReference()
{
	++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::PhysicsModel::DecrementReference()
{
	--countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete object
// ------------------------------------------------------------------------------------ //
void le::PhysicsModel::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::PhysicsModel::GetCountReferences() const
{
	return countReferences;
}

// ------------------------------------------------------------------------------------ //
// Initialize mesh
// ------------------------------------------------------------------------------------ //
void le::PhysicsModel::InitializeMesh( le::Vector3D_t* Verteces, le::UInt32_t CountVerteces, le::UInt32_t* Indeces, le::UInt32_t CountIndeces )
{
	LIFEENGINE_ASSERT( Verteces && CountVerteces > 0 );

	if ( triangleMesh || convexTriangleMesh )
		ClearMesh();

	triangleMesh = new btTriangleMesh();

	// If mesh with indeces
	if ( Indeces && CountIndeces > 0)
		for ( UInt32_t index = 0, offset = 0, count = CountIndeces / 3; index < count; ++index, offset += 3 )
		{
			Vector3D_t&		vertex1 = Verteces[ Indeces[ offset ] ];
			Vector3D_t&		vertex2 = Verteces[ Indeces[ offset + 1 ] ];
			Vector3D_t&		vertex3 = Verteces[ Indeces[ offset + 2 ] ];

			triangleMesh->addTriangle( btVector3( vertex1.x, vertex1.y, vertex1.z ),
									   btVector3( vertex2.x, vertex2.y, vertex2.z),
									   btVector3( vertex3.x, vertex3.y, vertex3.z ) );
		}
	else
		for ( UInt32_t index = 0, offset = 0, count = CountVerteces / 3; index < count; ++index, offset += 3 )
		{
			Vector3D_t&		vertex1 = Verteces[ offset ];
			Vector3D_t&		vertex2 = Verteces[ offset + 1 ];
			Vector3D_t&		vertex3 = Verteces[ offset + 2 ];

			triangleMesh->addTriangle( btVector3( vertex1.x, vertex1.y, vertex1.z ),
									   btVector3( vertex2.x, vertex2.y, vertex2.z),
									   btVector3( vertex3.x, vertex3.y, vertex3.z ) );
		}

	convexTriangleMesh = new btConvexTriangleMeshShape( triangleMesh );
}

// ------------------------------------------------------------------------------------ //
// Clear mesh
// ------------------------------------------------------------------------------------ //
void le::PhysicsModel::ClearMesh()
{
	if ( triangleMesh )			delete triangleMesh;
	if ( convexTriangleMesh ) delete convexTriangleMesh;

	triangleMesh = nullptr;
	convexTriangleMesh = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Is initialized mesh
// ------------------------------------------------------------------------------------ //
bool le::PhysicsModel::IsInitializedMesh() const
{
	return triangleMesh && convexTriangleMesh;
}
