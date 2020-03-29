//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <vector>

#include "common/types.h"
#include "physics/iphysicssysteminternal.h"
#include "physicssystemfactory.h"
#include "debugdrawer.h"

//---------------------------------------------------------------------//

class btDynamicsWorld;
class btDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;
class btCollisionConfiguration;

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IEngine;
	class IConVar;
	class IStudioRender;
	class Body;
	class CharcterController;

    //---------------------------------------------------------------------//

    class PhysicsSystem : public IPhysicsSystemInternal
    {
    public:
        // IPhysicsSystem
		virtual void						AddBody( IBody* Body );
		virtual void						RemoveBody( IBody* Body );
		virtual void						RemoveAllBodies();
		virtual void						AddCharcterController( ICharcterController* CharcterController );
		virtual void						RemoveCharcterController( ICharcterController* CharcterController );
		virtual void						RemoveAllCharcterControllers();

		virtual void						SetDebugCamera( ICamera* Camera );
		virtual void						SetGravity( const Vector3D_t& Gravity );

		virtual const Vector3D_t&			GetGravity() const;
		virtual UInt32_t					GetCountBodes() const;
		virtual IBody*						GetBody( UInt32_t Index ) const;
		virtual IBody**						GetBodies() const;
		virtual IFactory*					GetFactory() const;
		virtual UInt32_t					GetCountCharcterControllers() const;
		virtual ICharcterController*		GetCharcterController( UInt32_t Index ) const;
		virtual ICharcterController**		GetCharcterControllers() const;

        // IPhysicsSystemInternal
		virtual bool						Initialize( IEngine* Engine );
		virtual void						Update();
		virtual void						DebugRender();

        // PhysicsSystem
        PhysicsSystem();
        ~PhysicsSystem();

    private:
		bool									isInitialize;

		UInt32_t								maxSubSteps;
		float									fixedTimeStep;

		btDynamicsWorld*						dynamicsWorld;
		btDispatcher*							dispatcher;
		btBroadphaseInterface*					broadphase;
		btConstraintSolver*						constraintSolver;
		btCollisionConfiguration*				collisionConfiguration;

		Vector3D_t								gravity;
		PhysicsSystemFactory					factory;
		DebugDrawer								debugDrawer;
		ICamera*								debugCamera;

		std::vector< Body* >					bodies;
		std::vector< CharcterController* >		charcterControllers;
	};

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IPHYSICS_SYSTEM_H
