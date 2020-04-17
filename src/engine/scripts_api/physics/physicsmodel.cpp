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
#include "physics/iphysicsmodel.h"

namespace scripts_api
{
#include "physicsmodel.h"
}

#define PHYSICSMODEL( Object )			( static_cast< le::IPhysicsModel* >( Object ) )

// ------------------------------------------------------------------------------------ //
// Create
// ------------------------------------------------------------------------------------ //
scripts_api::physicsModel_t scripts_api::PhysicsModel_Create()
{
	return le::g_physicsSystemFactory->Create( PHYSICSMODEL_INTERFACE_VERSION );
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsModel_Delete( physicsModel_t Object )
{
	if ( !Object ) return;

	if ( PHYSICSMODEL( Object )->GetCountReferences() <= 1 )
		PHYSICSMODEL( Object )->Release();
	else
		PHYSICSMODEL( Object )->DecrementReference();

	Object = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Initialize mesh
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsModel_InitializeMesh( physicsModel_t Object, vec3f_t* Verteces, uint32_t CountVerteces, uint32_t* Indeces, uint32_t CountIndeces )
{
	if ( !Object ) return;
	PHYSICSMODEL( Object )->InitializeMesh( ( le::Vector3D_t* ) Verteces, CountVerteces, Indeces, CountIndeces );
}

// ------------------------------------------------------------------------------------ //
// Clear mesh
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsModel_ClearMesh( physicsModel_t Object )
{
	if ( !Object ) return;
	PHYSICSMODEL( Object )->ClearMesh();
}

// ------------------------------------------------------------------------------------ //
// Is initialized mesh
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::PhysicsModel_IsInitializedMesh( physicsModel_t Object )
{
	if ( !Object ) return B_FALSE;
	return PHYSICSMODEL( Object )->IsInitializedMesh() ? B_TRUE : B_FALSE;
}
