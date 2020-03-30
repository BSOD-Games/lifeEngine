//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "common/types.h"
#include "engine/iconsolesystem.h"
#include "physics/debugdrawer.h"
#include "studiorender/istudiorender.h"
#include "global.h"

// ------------------------------------------------------------------------------------ //
// Draw line
// ------------------------------------------------------------------------------------ //
void le::DebugDrawer::drawLine( const btVector3& from, const btVector3& to, const btVector3& color )
{
	g_studioRender->SubmitDebugLine( Vector3D_t( from.getX(), from.getY(), from.getZ() ), Vector3D_t( to.getX(), to.getY(), to.getZ() ), Vector3D_t( color.getX(), color.getY(), color.getZ() ) );
}

// ------------------------------------------------------------------------------------ //
// Draw contact point
// ------------------------------------------------------------------------------------ //
void le::DebugDrawer::drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color )
{

}

// ------------------------------------------------------------------------------------ //
// Report error warning
// ------------------------------------------------------------------------------------ //
void le::DebugDrawer::reportErrorWarning( const char* warningString )
{
	g_consoleSystem->PrintWarning( warningString );
}

// ------------------------------------------------------------------------------------ //
// Draw 3D text
// ------------------------------------------------------------------------------------ //
void le::DebugDrawer::draw3dText( const btVector3& location, const char* textString )
{
}

// ------------------------------------------------------------------------------------ //
// Set debug mode
// ------------------------------------------------------------------------------------ //
void le::DebugDrawer::setDebugMode( int debugMode )
{
	mode = debugMode;
}

// ------------------------------------------------------------------------------------ //
// Get debug mode
// ------------------------------------------------------------------------------------ //
int le::DebugDrawer::getDebugMode() const
{
	return mode;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::DebugDrawer::DebugDrawer() :
	mode( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::DebugDrawer::~DebugDrawer()
{}
