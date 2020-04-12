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
#include "physics/iphysicssystem.h"

namespace scripts_api
{
#include "physicssystem.h"
}

// ------------------------------------------------------------------------------------ //
// Add body
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsSystem_AddBody( body_t Body )
{
	le::g_physicsSystem->AddBody( ( le::IBody* ) Body );
}

// ------------------------------------------------------------------------------------ //
// Remove body
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsSystem_RemoveBody( body_t Body )
{
	le::g_physicsSystem->RemoveBody( ( le::IBody* ) Body );
}

// ------------------------------------------------------------------------------------ //
// Remove all bodies
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsSystem_RemoveAllBodies()
{
	le::g_physicsSystem->RemoveAllBodies();
}

// ------------------------------------------------------------------------------------ //
// Add charcter controller
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsSystem_AddCharcterController( charcterController_t CharcterController )
{
	le::g_physicsSystem->AddCharcterController( ( le::ICharcterController* ) CharcterController );
}

// ------------------------------------------------------------------------------------ //
// Remove charcter controller
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsSystem_RemoveCharcterController( charcterController_t CharcterController )
{
	le::g_physicsSystem->RemoveCharcterController( ( le::ICharcterController* ) CharcterController );
}

// ------------------------------------------------------------------------------------ //
// Remove all charcter controllers
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsSystem_RemoveAllCharcterControllers()
{
	le::g_physicsSystem->RemoveAllCharcterControllers();
}

// ------------------------------------------------------------------------------------ //
// Set debug camera
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsSystem_SetDebugCamera( camera_t Camera )
{
	le::g_physicsSystem->SetDebugCamera( ( le::ICamera* ) Camera );
}

// ------------------------------------------------------------------------------------ //
// Set gravity
// ------------------------------------------------------------------------------------ //
void scripts_api::PhysicsSystem_SetGravity( vec3f_t Gravity )
{
	le::g_physicsSystem->SetGravity( { Gravity.x, Gravity.y, Gravity.z } );
}

// ------------------------------------------------------------------------------------ //
// Get gravity
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::PhysicsSystem_GetGravity()
{
	const le::Vector3D_t&			gravity = le::g_physicsSystem->GetGravity();
	return { gravity.x, gravity.y, gravity.z };
}

// ------------------------------------------------------------------------------------ //
// Get count bodes
// ------------------------------------------------------------------------------------ //
uint32_t scripts_api::PhysicsSystem_GetCountBodes()
{
	return le::g_physicsSystem->GetCountBodes();
}

// ------------------------------------------------------------------------------------ //
// Get body
// ------------------------------------------------------------------------------------ //
scripts_api::body_t scripts_api::PhysicsSystem_GetBody( uint32_t Index )
{
	return le::g_physicsSystem->GetBody( Index );
}

// ------------------------------------------------------------------------------------ //
// Get bodies
// ------------------------------------------------------------------------------------ //
scripts_api::body_t* scripts_api::PhysicsSystem_GetBodies()
{
	return ( body_t* ) le::g_physicsSystem->GetBodies();
}

// ------------------------------------------------------------------------------------ //
// Get count charcter controller
// ------------------------------------------------------------------------------------ //
scripts_api::uint32_t scripts_api::PhysicsSystem_GetCountCharcterControllers()
{
	return le::g_physicsSystem->GetCountCharcterControllers();
}

// ------------------------------------------------------------------------------------ //
// Get character controller
// ------------------------------------------------------------------------------------ //
scripts_api::charcterController_t scripts_api::PhysicsSystem_GetCharcterController( uint32_t Index )
{
	return le::g_physicsSystem->GetCharcterController( Index );
}

// ------------------------------------------------------------------------------------ //
// Get character controllers
// ------------------------------------------------------------------------------------ //
scripts_api::charcterController_t* scripts_api::PhysicsSystem_GetCharcterControllers()
{
	return ( charcterController_t* ) le::g_physicsSystem->GetCharcterControllers();
}
