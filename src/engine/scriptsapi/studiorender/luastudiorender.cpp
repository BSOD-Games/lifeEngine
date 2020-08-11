//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#include "global.h"
#include "studiorender/istudiorender.h"
#include "scriptsapi/mathlib/luavector3d.h"
#include "scriptsapi/engine/luacamera.h"
#include "scriptsapi/engine/luamodel.h"
#include "scriptsapi/studiorender/luastudiorender.h"

// ------------------------------------------------------------------------------------ //
// Register studiorender in LUA
// ------------------------------------------------------------------------------------ //
void le::LUAStudioRender::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;
	
	// Registern studiorender system
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAStudioRender>( "StudioRender" ).
		addStaticFunction( "BeginScene", &LUAStudioRender::BeginScene ).
		addStaticFunction( "SubmitDebugLine", &LUAStudioRender::SubmitDebugLine ).
		addStaticFunction( "SubmitDebugPoint", &LUAStudioRender::SubmitDebugPoint ).
		addStaticFunction( "SubmitModel", ( void ( * ) ( const LUAModel& ) ) &LUAStudioRender::SubmitModel ).
		addStaticFunction( "SubmitModel", ( void ( * ) ( const LUAModel&, UInt32_t, UInt32_t ) ) &LUAStudioRender::SubmitModel ).
		addStaticFunction( "EndScene", &LUAStudioRender::EndScene ).
		addStaticFunction( "SetVerticalSyncEnabled", &LUAStudioRender::SetVerticalSyncEnabled ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Begin scene
// ------------------------------------------------------------------------------------ //
void le::LUAStudioRender::BeginScene( const LUACamera& Camera )
{
	g_studioRender->BeginScene( Camera.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Submit debug line
// ------------------------------------------------------------------------------------ //
void le::LUAStudioRender::SubmitDebugLine( const LUAVector3D& From, const LUAVector3D& To, const LUAVector3D& Color )
{
	g_studioRender->SubmitDebugLine( From.GetHandle(), To.GetHandle(), Color.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Submit debug point
// ------------------------------------------------------------------------------------ //
void le::LUAStudioRender::SubmitDebugPoint( const LUAVector3D& Position, const LUAVector3D& Color )
{
	g_studioRender->SubmitDebugPoint( Position.GetHandle(), Color.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Submit model
// ------------------------------------------------------------------------------------ //
void le::LUAStudioRender::SubmitModel( const LUAModel& Model )
{
	g_studioRender->SubmitModel( Model.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Submit model
// ------------------------------------------------------------------------------------ //
void le::LUAStudioRender::SubmitModel( const LUAModel& Model, UInt32_t StartSurface, UInt32_t CountSurface )
{
	g_studioRender->SubmitModel( Model.GetHandle(), StartSurface, CountSurface );
}

// ------------------------------------------------------------------------------------ //
// End scene
// ------------------------------------------------------------------------------------ //
void le::LUAStudioRender::EndScene()
{
	g_studioRender->EndScene();
}

// ------------------------------------------------------------------------------------ //
// Set vertical sync enabled
// ------------------------------------------------------------------------------------ //
void le::LUAStudioRender::SetVerticalSyncEnabled( bool IsEnabled )
{
	g_studioRender->SetVerticalSyncEnabled( IsEnabled );
}
