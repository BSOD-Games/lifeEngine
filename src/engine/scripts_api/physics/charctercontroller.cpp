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
#include "physics/icharactercontroller.h"
#include "physics/shapecapsuledescriptor.h"

namespace scripts_api
{
#include "charctercontroller.h"
}

#define CHARCTERCONTROLLER( Object )			( static_cast< le::ICharcterController* >( Object ) )

// ------------------------------------------------------------------------------------ //
// Charcter controller create
// ------------------------------------------------------------------------------------ //
scripts_api::charcterController_t scripts_api::CharcterController_Create()
{
	return le::g_physicsSystemFactory->Create( CHARCTERCONTROLLER_INTERFACE_VERSION );
}

// ------------------------------------------------------------------------------------ //
// Charcter controller delete
// ------------------------------------------------------------------------------------ //
void scripts_api::CharcterController_Delete( charcterController_t Object )
{
	if ( !Object ) return;

	if ( CHARCTERCONTROLLER( Object )->GetCountReferences() <= 1 )
		CHARCTERCONTROLLER( Object )->Release();
	else
		CHARCTERCONTROLLER( Object )->DecrementReference();

	Object = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Initialize
// ------------------------------------------------------------------------------------ //
void scripts_api::CharcterController_Initialize( charcterController_t Object, shapeCapsuleDescriptor_t ShapeCapsuleDescriptor )
{
	if ( !Object ) return;

	le::ShapeCapsuleDescriptor			shape = { ShapeCapsuleDescriptor.radius, ShapeCapsuleDescriptor.height };
	CHARCTERCONTROLLER( Object )->Initialize( shape );
}

// ------------------------------------------------------------------------------------ //
// Walk
// ------------------------------------------------------------------------------------ //
void scripts_api::CharcterController_Walk( charcterController_t Object, vec3f_t WalkDirection )
{
	if ( !Object ) return;
	CHARCTERCONTROLLER( Object )->Walk( { WalkDirection.x, WalkDirection.y, WalkDirection.z } );
}

// ------------------------------------------------------------------------------------ //
// Jump
// ------------------------------------------------------------------------------------ //
void scripts_api::CharcterController_Jump( charcterController_t Object, vec3f_t JumpForce )
{
	if ( !Object ) return;
	CHARCTERCONTROLLER( Object )->Jump( { JumpForce.x, JumpForce.y, JumpForce.z } );
}

// ------------------------------------------------------------------------------------ //
// Set velocity
// ------------------------------------------------------------------------------------ //
void scripts_api::CharcterController_SetVelocity( charcterController_t Object, vec3f_t Velocity )
{
	if ( !Object ) return;
	CHARCTERCONTROLLER( Object )->SetVelocity( { Velocity.x, Velocity.y, Velocity.z } );
}

// ------------------------------------------------------------------------------------ //
// Set position
// ------------------------------------------------------------------------------------ //
void scripts_api::CharcterController_SetPosition( charcterController_t Object, vec3f_t Position )
{
	if ( !Object ) return;
	CHARCTERCONTROLLER( Object )->SetPosition( { Position.x, Position.y, Position.z } );
}

// ------------------------------------------------------------------------------------ //
// Set rotation
// ------------------------------------------------------------------------------------ //
void scripts_api::CharcterController_SetRotation( charcterController_t Object, quat_t Rotation )
{
	if ( !Object ) return;
	CHARCTERCONTROLLER( Object )->SetRotation( { Rotation.w, Rotation.x, Rotation.y, Rotation.z } );
}

// ------------------------------------------------------------------------------------ //
// Can jump
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::CharcterController_CanJump( charcterController_t Object )
{
	if ( !Object ) return B_FALSE;
	return CHARCTERCONTROLLER( Object )->CanJump() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// On ground
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::CharcterController_OnGround( charcterController_t Object )
{
	if ( !Object ) return B_FALSE;
	return CHARCTERCONTROLLER( Object )->OnGround() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Get velocity
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::CharcterController_GetVelocity( charcterController_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&			vec3 = CHARCTERCONTROLLER( Object )->GetVelocity();
	return { vec3.x, vec3.y, vec3.z };
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::CharcterController_GetPosition( charcterController_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&			vec3 = CHARCTERCONTROLLER( Object )->GetPosition();
	return { vec3.x, vec3.y, vec3.z };
}

// ------------------------------------------------------------------------------------ //
// Get rotation
// ------------------------------------------------------------------------------------ //
scripts_api::quat_t scripts_api::CharcterController_GetRotation( charcterController_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f, 0.f };

	const le::Quaternion_t&			quat = CHARCTERCONTROLLER( Object )->GetRotation();
	return { quat.x, quat.y, quat.z, quat.w };
}
