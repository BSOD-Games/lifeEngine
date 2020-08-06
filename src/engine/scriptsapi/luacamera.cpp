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
#include "engine/lifeengine.h"
#include "engine/engine.h"
#include "engine/camera.h"
#include "scriptsapi/luacamera.h"

#define REGISTER_ENUM_VALUE( LuaVM, Value ) \
	lua_pushliteral( ( LuaVM ), #Value ); \
	lua_pushinteger( ( LuaVM ), ( Value ) ); \
	lua_settable( ( LuaVM ), -3 )

// ------------------------------------------------------------------------------------ //
// Register window
// ------------------------------------------------------------------------------------ //
void le::LUACamera::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;
	
	LIFEENGINE_ASSERT( g_engine );
	g_window = g_engine->GetWindow();

	// Register metatable for enum CAMERA_SIDE_MOVE
	lua_newtable( LuaVM );

	// Register enum values
	REGISTER_ENUM_VALUE( LuaVM, CSM_NONE );
	REGISTER_ENUM_VALUE( LuaVM, CSM_FORWARD );
	REGISTER_ENUM_VALUE( LuaVM, CSM_BACKWARD );
	REGISTER_ENUM_VALUE( LuaVM, CSM_LEFT );
	REGISTER_ENUM_VALUE( LuaVM, CSM_RIGHT );

	// Set metatable to global value
	lua_setglobal( LuaVM, "CameraSideMove" );

	// Register window
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<Camera>( "Camera" ).
		addConstructor< void (*)() >().
		addFunction( "InitProjection_Perspective", &Camera::InitProjection_Perspective ).
		addFunction( "InitProjection_Ortho", &Camera::InitProjection_Ortho ).
		addFunction( "Move", ( void ( Camera::* )( const Vector3D_t& ) ) &Camera::Move ).
		addFunction( "Move", ( void ( Camera::* )( CAMERA_SIDE_MOVE, float ) ) &Camera::Move ).
		addFunction( "Rotate", &Camera::Rotate ).
		addFunction( "RotateByMouse", &Camera::RotateByMouse ).
		addFunction( "ScreenToWorld", &Camera::ScreenToWorld ).
		addFunction( "WorldToScreen", &Camera::WorldToScreen ).
		addFunction( "SetPosition", &Camera::SetPosition ).
		addFunction( "SetRotation", &Camera::SetRotation ).
		addFunction( "SetTargetDirection", &Camera::SetTargetDirection ).
		addFunction( "SetUp", &Camera::SetUp ).
		addFunction( "IsVisible", ( bool (Camera::*)( const Vector3D_t&, const Vector3D_t& ) ) &Camera::IsVisible ).
		addFunction( "IsVisible", ( bool ( Camera::* )( const Vector3DInt_t&, const Vector3DInt_t& ) ) &Camera::IsVisible ).
		addFunction( "IsVisible", ( bool ( Camera::* )( const Vector3DInt_t&, float ) ) &Camera::IsVisible ).
		addFunction( "GetNear", &Camera::GetNear ).
		addFunction( "GetFar", &Camera::GetFar ).
		addFunction( "GetPosition", &Camera::GetPosition ).
		addFunction( "GetUp", &Camera::GetUp ).
		addFunction( "GetRight", &Camera::GetRight ).
		addFunction( "GetDirectionMove", &Camera::GetDirectionMove ).
		addFunction( "GetEulerRotation", &Camera::GetEulerRotation ).
		addFunction( "GetTargetDirection", &Camera::GetTargetDirection ).
		endClass();
}