//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef DEBUG_DRAWER_H
#define DEBUG_DRAWER_H

#include <btBulletDynamicsCommon.h>

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ICamera;

	//---------------------------------------------------------------------//

	class DebugDrawer : public btIDebugDraw
	{
	public:
		// btIDebugDraw
		virtual void			drawLine( const btVector3& from, const btVector3& to, const btVector3& color );
		virtual void			drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color );
		virtual void			reportErrorWarning( const char* warningString );
		virtual void			draw3dText( const btVector3& location, const char* textString );

		virtual void			setDebugMode(int debugMode);

		virtual int				getDebugMode() const;

		// DebugDrawer
		DebugDrawer();
		~DebugDrawer();

	private:
		int							mode;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // DEBUG_DRAWER_H
