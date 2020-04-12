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
#include "physics/ibody.h"

namespace scripts_api
{
#include "body.h"
}

#define BODY( Object )		( static_cast< le::IBody* >( Object ) )

// ------------------------------------------------------------------------------------ //
// Body create
// ------------------------------------------------------------------------------------ //
scripts_api::body_t scripts_api::Body_Create()
{
	return le::g_physicsSystemFactory->Create( BODY_INTERFACE_VERSION );
}

// ------------------------------------------------------------------------------------ //
// Body delete
// ------------------------------------------------------------------------------------ //
void scripts_api::Body_Delete( body_t Object )
{
	if ( !Object ) return;

	if ( BODY( Object )->GetCountReferences() <= 1 )
		BODY( Object )->Release();
	else
		BODY( Object )->DecrementReference();

	Object = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Body initialize
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::Body_Initialize( body_t Object, collider_t Collider, float Mass, vec3f_t Inertia, bool_t IsStatic )
{
	if ( !Object ) return B_FALSE;
	return BODY( Object )->Create( ( le::ICollider* ) Collider, Mass, { Inertia.x, Inertia.y, Inertia.z }, IsStatic == B_TRUE ? true : false ) ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Body activate
// ------------------------------------------------------------------------------------ //
void scripts_api::Body_Activate( body_t Object )
{
	if ( !Object ) return;
	BODY( Object )->Activate();
}

// ------------------------------------------------------------------------------------ //
// Apply impulse
// ------------------------------------------------------------------------------------ //
void scripts_api::Body_ApplyImpulse( body_t Object, vec3f_t Factor, vec3f_t CenterBody )
{
	if ( !Object ) return;
	BODY( Object )->ApplyImpulse( { Factor.x, Factor.y, Factor.z }, { CenterBody.x, CenterBody.y, CenterBody.z } );
}

// ------------------------------------------------------------------------------------ //
// Apply force
// ------------------------------------------------------------------------------------ //
void scripts_api::Body_ApplyForce( body_t Object, vec3f_t Factor, vec3f_t CenterBody )
{
	if ( !Object ) return;
	BODY( Object )->ApplyForce( { Factor.x, Factor.y, Factor.z }, { CenterBody.x, CenterBody.y, CenterBody.z } );
}

// ------------------------------------------------------------------------------------ //
// Freeze rotation
// ------------------------------------------------------------------------------------ //
void scripts_api::Body_FreezeRotation( body_t Object, bool_t X, bool_t Y, bool_t Z )
{
	if ( !Object ) return;
	BODY( Object )->FreezeRotation( X == B_TRUE ? true : false, Y == B_TRUE ? true : false, Z == B_TRUE ? true : false );
}

// ------------------------------------------------------------------------------------ //
// Set position
// ------------------------------------------------------------------------------------ //
void scripts_api::Body_SetPosition( body_t Object, vec3f_t Position )
{
	if ( !Object ) return;
	BODY( Object )->SetPosition( { Position.x, Position.y, Position.z } );
}

// ------------------------------------------------------------------------------------ //
// Set rotation
// ------------------------------------------------------------------------------------ //
void scripts_api::Body_SetRotation( body_t Object, quat_t Rotation )
{
	if ( !Object ) return;
	BODY( Object )->SetRotation( { Rotation.w, Rotation.x, Rotation.y, Rotation.z } );
}

// ------------------------------------------------------------------------------------ //
// Is static body
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::Body_IsStatic( body_t Object )
{
	if ( !Object ) return B_FALSE;
	return BODY( Object )->IsStatic() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Get collider
// ------------------------------------------------------------------------------------ //
scripts_api::collider_t scripts_api::Body_GetCollider( body_t Object )
{
	if ( !Object ) return nullptr;
	return BODY( Object )->GetCollider();
}

// ------------------------------------------------------------------------------------ //
// Get mass
// ------------------------------------------------------------------------------------ //
float scripts_api::Body_GetMass( body_t Object )
{
	if ( !Object ) return 0.f;
	return BODY( Object )->GetMass();
}

// ------------------------------------------------------------------------------------ //
// Get inertia
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Body_GetInertia( body_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&		vec3 = BODY( Object )->GetInertia();
	return { vec3.x, vec3.y, vec3.z };
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Body_GetPosition( body_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&		vec3 = BODY( Object )->GetPosition();
	return { vec3.x, vec3.y, vec3.z };
}

// ------------------------------------------------------------------------------------ //
// Get rotation
// ------------------------------------------------------------------------------------ //
scripts_api::quat_t scripts_api::Body_GetRotation( body_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f, 0.f };

	const le::Quaternion_t&		quat = BODY( Object )->GetRotation();
	return { quat.x, quat.y, quat.z, quat.w };
}
