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
	isStatic( false ),
	masa( 0.f ),
	inertia( 0.f, 0.f, 0.f ),
	countReferences( 0 ),
	mesh( nullptr ),
	shape( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::PhysicsModel::~PhysicsModel()
{
	Clear();
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
// Initialize
// ------------------------------------------------------------------------------------ //
void le::PhysicsModel::Initialize( le::Vector3D_t* Verteces, le::UInt32_t CountVerteces, le::UInt32_t* Indeces, le::UInt32_t CountIndeces, bool IsStatic )
{
	if ( !Verteces || CountVerteces <= 0 )		return;
	if ( mesh || shape )						Clear();

	// If mesh with indeces
	if ( Indeces && CountIndeces > 0 )
	{
		// Copy verteces and indeces
		verteces.resize( CountVerteces );
		indeces.resize( CountIndeces );
		memcpy( verteces.data(), Verteces, CountVerteces * sizeof( Vector3D_t ) );
		memcpy( indeces.data(), Indeces, CountIndeces * sizeof( UInt32_t ) );
	}
	else
	{
		// Generate indeced mesh
		for ( UInt32_t index = 0; index < CountVerteces; ++index )
		{
			auto		it = find( verteces.begin(), verteces.end(), Verteces[ index ] );
			
			// Look for the vertex index in the shared vertex buffer,
			// if not found, add the vertex to the buffer,
			// and then write its index
			if ( it == verteces.end() )
			{
				indeces.push_back( verteces.size() );
				verteces.push_back( Verteces[ index ] );
			}
			else
				indeces.push_back( it - verteces.begin() );
		}
	}

	// Make collision shape
	mesh = new btTriangleIndexVertexArray();
	btIndexedMesh		indexedMesh;
	indexedMesh.m_numTriangles = indeces.size() / 3;
	indexedMesh.m_numVertices = verteces.size();
	indexedMesh.m_vertexBase = ( unsigned char* ) verteces.data();
	indexedMesh.m_vertexStride = sizeof( Vector3D_t );
	indexedMesh.m_vertexType = PHY_FLOAT;
	indexedMesh.m_triangleIndexBase = ( unsigned char* ) indeces.data();
	indexedMesh.m_triangleIndexStride = 3 * sizeof( UInt32_t );

	mesh->addIndexedMesh( indexedMesh );

	if ( IsStatic )		shape = new btBvhTriangleMeshShape( mesh, true );
	else				shape = new btConvexTriangleMeshShape( mesh, true );
	
	isStatic = IsStatic;
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void le::PhysicsModel::Clear()
{
	if ( mesh )		delete mesh;
	if ( shape )	delete shape;

	isStatic = false;
	masa = 0.f;
	inertia = le::Vector3D_t( 0.f, 0.f, 0.f );
	mesh = nullptr;
	shape = nullptr;

	verteces.clear();
	indeces.clear();
}

// ------------------------------------------------------------------------------------ //
// Set mass
// ------------------------------------------------------------------------------------ //
void le::PhysicsModel::SetMasa( float Masa )
{
	masa = Masa;
}

// ------------------------------------------------------------------------------------ //
// Set inertia
// ------------------------------------------------------------------------------------ //
void le::PhysicsModel::SetInertia( const le::Vector3D_t& Inertia )
{
	inertia = Inertia;
}

// ------------------------------------------------------------------------------------ //
// Is static
// ------------------------------------------------------------------------------------ //
bool le::PhysicsModel::IsStatic() const
{
	return isStatic;
}

// ------------------------------------------------------------------------------------ //
// Is initialized
// ------------------------------------------------------------------------------------ //
bool le::PhysicsModel::IsInitialized() const
{
	return mesh && shape;
}

// ------------------------------------------------------------------------------------ //
// Get masa
// ------------------------------------------------------------------------------------ //
float le::PhysicsModel::GetMasa() const
{
	return masa;
}

// ------------------------------------------------------------------------------------ //
// Get inertia
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::PhysicsModel::GetInertia() const
{
	return inertia;
}
