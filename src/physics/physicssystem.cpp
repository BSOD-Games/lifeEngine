//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "engine/lifeengine.h"
#include "engine/ifactory.h"
#include "engine/iconvar.h"
#include "engine/iengine.h"
#include "engine/iconsolesystem.h"
#include "engine/icamera.h"
#include "physics/physicssystem.h"
#include "physics/body.h"
#include "physics/charactercontroller.h"
#include "studiorender/istudiorender.h"
#include "global.h"

LIFEENGINE_PHYSICSSYSTEM_API( le::PhysicsSystem );

// ------------------------------------------------------------------------------------ //
// Initialize physics system
// ------------------------------------------------------------------------------------ //
bool le::PhysicsSystem::Initialize( le::IEngine* Engine )
{
    if ( isInitialize )         return true;

	g_engine =  Engine;
	g_consoleSystem = Engine->GetConsoleSystem();
	g_studioRender = Engine->GetStudioRender();

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher( collisionConfiguration );
    broadphase = new btDbvtBroadphase();
    constraintSolver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld( dispatcher, broadphase, constraintSolver, collisionConfiguration );
	broadphase->getOverlappingPairCache()->setInternalGhostPairCallback( new btGhostPairCallback() );

	IFactory*			consoleSystemFactory = g_consoleSystem->GetFactory();
	conVar_debug = ( IConVar* ) consoleSystemFactory->Create( CONVAR_INTERFACE_VERSION );
	if ( conVar_debug )
	{
		conVar_debug->Initialize( "phy_debug", "0", CVT_BOOL, "bool value for showing physics debug info", true, 0.f, true, 1.f, nullptr );
		conVar_debug->IncrementReference();
		g_consoleSystem->RegisterVar( conVar_debug );
	}

	dynamicsWorld->setDebugDrawer( &debugDrawer );
	debugDrawer.setDebugMode( btIDebugDraw::DBG_DrawWireframe );
    dynamicsWorld->setGravity( btVector3( gravity.x, gravity.y, gravity.z ) );
    isInitialize = true;
    return true;
}

// ------------------------------------------------------------------------------------ //
// Update physics system
// ------------------------------------------------------------------------------------ //
void le::PhysicsSystem::Update()
{
    if ( !isInitialize ) return;

	// TODO: move activation bodies to level manager
	for ( auto it = bodies.begin(), itEnd = bodies.end(); it != itEnd; ++it )
		if ( (*it)->GetType() == BT_DYNAMIC )
			(*it)->Activate();

	dynamicsWorld->stepSimulation( 1.f / 60.f );

	if ( debugCamera && conVar_debug && conVar_debug->GetValueBool() )
	{
		g_studioRender->BeginScene( debugCamera );
		dynamicsWorld->debugDrawWorld();
		g_studioRender->EndScene();
	}
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::PhysicsSystem::PhysicsSystem() :
    isInitialize( false ),
	gravity( 0.f, -9.8f, 0.f ),
    dynamicsWorld( nullptr ),
    dispatcher( nullptr ),
    broadphase( nullptr ),
    constraintSolver( nullptr ),
	collisionConfiguration( nullptr ),
	debugCamera( nullptr ),
	conVar_debug( nullptr )
{
	LIFEENGINE_ASSERT( !g_physicsSystem );
	g_physicsSystem = this;
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::PhysicsSystem::~PhysicsSystem()
{
    if ( isInitialize )
    {
        delete collisionConfiguration;
        delete dispatcher;
        delete broadphase;
        delete constraintSolver;
        delete dynamicsWorld;
    }

	if ( debugCamera )
	{
		if ( debugCamera->GetCountReferences() <= 1 )
			debugCamera->Release();
		else
			debugCamera->DecrementReference();
	}

	if ( conVar_debug )
	{
		g_consoleSystem->UnregisterVar( conVar_debug->GetName() );

		if ( conVar_debug->GetCountReferences() <= 1 )
			conVar_debug->Release();
		else
			conVar_debug->DecrementReference();
	}

	RemoveAllBodies();
}

// ------------------------------------------------------------------------------------ //
// Get factory
// ------------------------------------------------------------------------------------ //
le::IFactory* le::PhysicsSystem::GetFactory() const
{
    return ( IFactory* ) &factory;
}

// ------------------------------------------------------------------------------------ //
// Add body
// ------------------------------------------------------------------------------------ //
void le::PhysicsSystem::AddBody( le::IBody* Body )
{
    LIFEENGINE_ASSERT( Body );

	le::Body*		body = static_cast< le::Body* >( Body );
	body->IncrementReference();
	dynamicsWorld->addRigidBody( body->GetHandle() );
	bodies.push_back( body );
}

// ------------------------------------------------------------------------------------ //
// Remove body
// ------------------------------------------------------------------------------------ //
void le::PhysicsSystem::RemoveBody( le::IBody* Body )
{
    LIFEENGINE_ASSERT( Body );

    for ( auto it = bodies.begin(), itEnd = bodies.end(); it != itEnd; ++it )
        if ( (*it) == Body )
        {
			dynamicsWorld->removeRigidBody( (*it)->GetHandle() );

            if ( (*it)->GetCountReferences() <= 1 )
                (*it)->Release();
            else
                (*it)->DecrementReference();

            bodies.erase( it );
            return;
        }
}

// ------------------------------------------------------------------------------------ //
// Remove all bodies
// ------------------------------------------------------------------------------------ //
void le::PhysicsSystem::RemoveAllBodies()
{
    if ( bodies.empty() ) return;

    for ( auto it = bodies.begin(), itEnd = bodies.end(); it != itEnd; ++it )
	{
		dynamicsWorld->removeRigidBody( (*it)->GetHandle() );

        if ( (*it)->GetCountReferences() <= 1 )
            (*it)->Release();
        else
            (*it)->DecrementReference();
	}

	bodies.clear();
}

// ------------------------------------------------------------------------------------ //
// Set camera for render debug info
// ------------------------------------------------------------------------------------ //
void le::PhysicsSystem::SetDebugCamera( le::ICamera* Camera )
{
	if ( debugCamera )
	{
		if ( debugCamera->GetCountReferences() <= 1 )
			debugCamera->Release();
		else
			debugCamera->DecrementReference();
	}

	debugCamera = Camera;

	if ( debugCamera )
		debugCamera->IncrementReference();
}

// ------------------------------------------------------------------------------------ //
// Set gravity
// ------------------------------------------------------------------------------------ //
void le::PhysicsSystem::SetGravity( const le::Vector3D_t& Gravity )
{
    if ( !isInitialize ) return;

    gravity = Gravity;
    dynamicsWorld->setGravity( btVector3( Gravity.x, Gravity.y, Gravity.z ) );
}

// ------------------------------------------------------------------------------------ //
// Get gravity
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::PhysicsSystem::GetGravity() const
{
    return gravity;
}

// ------------------------------------------------------------------------------------ //
// Get count bodes
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::PhysicsSystem::GetCountBodes() const
{
    return bodies.size();
}

// ------------------------------------------------------------------------------------ //
// Get body
// ------------------------------------------------------------------------------------ //
le::IBody* le::PhysicsSystem::GetBody( le::UInt32_t Index ) const
{
    LIFEENGINE_ASSERT( Index < bodies.size() );
    return  ( IBody* ) bodies[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get bodies
// ------------------------------------------------------------------------------------ //
le::IBody** le::PhysicsSystem::GetBodies() const
{
    return ( IBody** ) bodies.data();
}

// ------------------------------------------------------------------------------------ //
// Add charcter controller
// ------------------------------------------------------------------------------------ //
void le::PhysicsSystem::AddCharcterController( le::ICharcterController* CharcterController )
{
	LIFEENGINE_ASSERT( CharcterController );

	le::CharcterController*		charcterController = static_cast< le::CharcterController* >( CharcterController );
	charcterController->IncrementReference();
	dynamicsWorld->addCollisionObject( charcterController->GetCollisionObject(), btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter );
	dynamicsWorld->addAction( charcterController->GetHandle() );
	charcterControllers.push_back( charcterController );
}

// ------------------------------------------------------------------------------------ //
// Remove charcter controller
// ------------------------------------------------------------------------------------ //
void le::PhysicsSystem::RemoveCharcterController( le::ICharcterController* CharcterController )
{
	LIFEENGINE_ASSERT( CharcterController );

	for ( auto it = charcterControllers.begin(), itEnd = charcterControllers.end(); it != itEnd; ++it )
		if ( (*it) == CharcterController )
		{
			dynamicsWorld->removeAction( (*it)->GetHandle() );
			dynamicsWorld->removeCollisionObject( (*it)->GetCollisionObject() );

			if ( (*it)->GetCountReferences() <= 1 )
				(*it)->Release();
			else
				(*it)->DecrementReference();

			charcterControllers.erase( it );
			return;
		}
}

// ------------------------------------------------------------------------------------ //
// Remove all charcter controllers
// ------------------------------------------------------------------------------------ //
void le::PhysicsSystem::RemoveAllCharcterControllers()
{
	if ( charcterControllers.empty() ) return;

	for ( auto it = charcterControllers.begin(), itEnd = charcterControllers.end(); it != itEnd; ++it )
	{
		dynamicsWorld->removeAction( (*it)->GetHandle() );
		dynamicsWorld->removeCollisionObject( (*it)->GetCollisionObject() );

		if ( (*it)->GetCountReferences() <= 1 )
			(*it)->Release();
		else
			(*it)->DecrementReference();
	}

	charcterControllers.clear();
}

// ------------------------------------------------------------------------------------ //
// Get count charcter controllers
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::PhysicsSystem::GetCountCharcterControllers() const
{
	return charcterControllers.size();
}

// ------------------------------------------------------------------------------------ //
// Get charcter controller
// ------------------------------------------------------------------------------------ //
le::ICharcterController* le::PhysicsSystem::GetCharcterController( le::UInt32_t Index ) const
{
	LIFEENGINE_ASSERT( Index < charcterControllers.size() );
	return  ( ICharcterController* ) charcterControllers[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get charcter controllers
// ------------------------------------------------------------------------------------ //
le::ICharcterController** le::PhysicsSystem::GetCharcterControllers() const
{
	return ( ICharcterController** ) charcterControllers.data();
}