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
void scripts_api::PhysicsModel_Initialize( physicsModel_t Object, vec3f_t* Verteces, uint32_t CountVerteces, uint32_t* Indeces, uint32_t CountIndeces, bool_t IsStatic )
{
	if ( !Object ) return;
	PHYSICSMODEL( Object )->Initialize( ( le::Vector3D_t* ) Verteces, CountVerteces, Indeces, CountIndeces, IsStatic == B_TRUE ? true : false );
}

// ------------------------------------------------------------------------------------ //
// Clear mesh
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsModel_Clear( physicsModel_t Object )
{
	if ( !Object ) return;
	PHYSICSMODEL( Object )->Clear();
}

// ------------------------------------------------------------------------------------ //
// Set masa
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsModel_SetMasa( physicsModel_t Object, float Masa )
{
	if ( !Object ) return;
	PHYSICSMODEL( Object )->SetMasa( Masa );
}

// ------------------------------------------------------------------------------------ //
// Set inertia
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsModel_SetInertia( physicsModel_t Object, vec3f_t Inertia )
{
	if ( !Object ) return;
	PHYSICSMODEL( Object )->SetInertia( le::Vector3D_t( Inertia.x, Inertia.y, Inertia.z ) );
}

// ------------------------------------------------------------------------------------ //
// Is static
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::PhysicsModel_IsStatic( physicsModel_t Object )
{
	if ( !Object ) return B_FALSE;
	return PHYSICSMODEL( Object )->IsStatic() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Is initialized mesh
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::PhysicsModel_IsInitialized( physicsModel_t Object )
{
	if ( !Object ) return B_FALSE;
	return PHYSICSMODEL( Object )->IsInitialized() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Get masa
// ------------------------------------------------------------------------------------ //
float scripts_api::PhysicsModel_GetMasa( physicsModel_t Object )
{
	if ( !Object ) return 0.f;
	return PHYSICSMODEL( Object )->GetMasa();
}

// ------------------------------------------------------------------------------------ //
// Get inertia
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::PhysicsModel_GetInertia( physicsModel_t Object )
{
	if ( !Object )	return { 0.f, 0.f, 0.f };
	
	le::Vector3D_t			inertia = PHYSICSMODEL( Object )->GetInertia();
	return { inertia.x, inertia.y, inertia.z };
}