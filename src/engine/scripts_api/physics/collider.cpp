//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/global.h"
#include "engine/ifactory.h"
#include "physics/icollider.h"
#include "physics/iphysicsmodel.h"
#include "physics/shapeboxdescriptor.h"
#include "physics/shapecylinderdescriptor.h"
#include "physics/shapecapsuledescriptor.h"
#include "physics/shapeconedescriptor.h"
#include "physics/shapeconvexhulldescriptor.h"
#include "physics/shapemeshdescriptor.h"
#include "physics/shapespheredescriptor.h"
#include "mathlib/gtc/type_ptr.hpp"

namespace scripts_api
{
#include "collider.h"
}

#define COLLIDER( Object )		( static_cast< le::ICollider* >( Object ) )

// ------------------------------------------------------------------------------------ //
// Collider create
// ------------------------------------------------------------------------------------ //
scripts_api::collider_t scripts_api::Collider_Create()
{
	return le::g_physicsSystemFactory->Create( COLLIDER_INTERFACE_VERSION );
}

// ------------------------------------------------------------------------------------ //
// Collider delete
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_Delete( collider_t Object )
{
	if ( !Object ) return;

	if ( COLLIDER( Object )->GetCountReferences() <= 1 )
		COLLIDER( Object )->Release();
	else
		COLLIDER( Object )->DecrementReference();

	Object = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Add box
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_AddBox( collider_t Object, shapeBoxDescriptor_t Shape, mat4_t LocalTransformation )
{
	if ( !Object ) return;

	le::ShapeBoxDescriptor			shape = { Shape.width, Shape.height, Shape.depth };
	COLLIDER( Object )->AddShape( shape, glm::make_mat4( LocalTransformation.matrix ) );
}

// ------------------------------------------------------------------------------------ //
// Add cylinder
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_AddCylinder( collider_t Object, shapeCylinderDescriptor_t Shape, mat4_t LocalTransformation )
{
	if ( !Object ) return;

	le::ShapeCylinderDescriptor		shape = { Shape.radius, Shape.height };
	COLLIDER( Object )->AddShape( shape, glm::make_mat4( LocalTransformation.matrix ) );
}

// ------------------------------------------------------------------------------------ //
// Add capsule
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_AddCapsule( collider_t Object, shapeCapsuleDescriptor_t Shape, mat4_t LocalTransformation )
{
	if ( !Object ) return;

	le::ShapeCapsuleDescriptor		shape = { Shape.radius, Shape.height };
	COLLIDER( Object )->AddShape( shape, glm::make_mat4( LocalTransformation.matrix ) );
}

// ------------------------------------------------------------------------------------ //
// Add sphere
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_AddSphere( collider_t Object, shapeSphereDescriptor_t Shape, mat4_t LocalTransformation )
{
	if ( !Object ) return;

	le::ShapeSphereDescriptor		shape = { Shape.radius };
	COLLIDER( Object )->AddShape( shape, glm::make_mat4( LocalTransformation.matrix ) );
}

// ------------------------------------------------------------------------------------ //
// Add mesh
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_AddMesh( collider_t Object, shapeMeshDescriptor_t Shape, mat4_t LocalTransformation )
{
	if ( !Object ) return;

	le::ShapeMeshDescriptor			shape = { Shape.countVerteces, ( le::Vector3D_t* ) Shape.verteces, Shape.countIndeces, Shape.indeces };
	COLLIDER( Object )->AddShape( shape, glm::mat4(1.f) );
}

// ------------------------------------------------------------------------------------ //
// Add cone
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_AddCone( collider_t Object, shapeConeDescriptor_t Shape, mat4_t LocalTransformation )
{
	if ( !Object ) return;

	le::ShapeConeDescriptor			shape = { Shape.radius, Shape.height };
	COLLIDER( Object )->AddShape( shape, glm::make_mat4( LocalTransformation.matrix ) );
}

// ------------------------------------------------------------------------------------ //
// Add convex hull
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_AddConvexHull( collider_t Object, shapeConvexHullDescriptor_t Shape, mat4_t LocalTransformation )
{
	if ( !Object ) return;

	le::ShapeConvexHullDescriptor		shape = { Shape.countVerteces, ( le::Vector3D_t* ) Shape.verteces };
	COLLIDER( Object )->AddShape( shape, glm::make_mat4( LocalTransformation.matrix ) );
}

// ------------------------------------------------------------------------------------ //
// Remove shape
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_RemoveShape( collider_t Object, uint32_t Index )
{
	if ( !Object ) return;
	COLLIDER( Object )->RemoveShape( Index );
}

// ------------------------------------------------------------------------------------ //
// Remove all shapes
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_RemoveAllShapes(collider_t Object)
{
	if ( !Object ) return;
	COLLIDER( Object )->RemoveAllShapes();
}

// ------------------------------------------------------------------------------------ //
// Get AABB
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_GetAABB( collider_t Object, vec3f_t* Min, vec3f_t* Max )
{
	if ( !Object )
	{
		if ( Min ) *Min = { 0.f, 0.f, 0.f };
		if ( Max ) *Max = { 0.f, 0.f, 0.f };
		return;
	}

	le::Vector3D_t		min, max;
	COLLIDER( Object )->GetAABB( min, max );

	if ( Min ) *Min = { min.x, min.y, min.z };
	if ( Max ) *Max = { max.x, max.y, max.z };
}

// ------------------------------------------------------------------------------------ //
// Get count shapes
// ------------------------------------------------------------------------------------ //
scripts_api::uint32_t scripts_api::Collider_GetCountShapes( collider_t Object )
{
	if ( !Object ) return 0;
	return COLLIDER( Object )->GetCountShapes();
}

// ------------------------------------------------------------------------------------ //
// Scale
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_Scale( collider_t Object, vec3f_t FactorScale )
{
	if ( !Object ) return;
	COLLIDER( Object )->Scale( { FactorScale.x, FactorScale.y, FactorScale.z } );
}

// ------------------------------------------------------------------------------------ //
// Set scale
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_SetScale( collider_t Object, vec3f_t Scale )
{
	if ( !Object ) return;
	COLLIDER( Object )->SetScale( { Scale.x, Scale.y, Scale.z } );
}

// ------------------------------------------------------------------------------------ //
// Get scale
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Collider_GetScale( collider_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&			vec3 = COLLIDER( Object )->GetScale();
	return { vec3.x, vec3.y, vec3.z };
}

// ------------------------------------------------------------------------------------ //
// Add physics model
// ------------------------------------------------------------------------------------ //
void scripts_api::Collider_AddPhysicsModel( collider_t Object, physicsModel_t PhysicsModel, mat4_t LocalTransformation )
{
	if ( !Object ) return;
	COLLIDER( Object )->AddShape( ( le::IPhysicsModel* ) PhysicsModel, glm::make_mat4( LocalTransformation.matrix ) );
}
