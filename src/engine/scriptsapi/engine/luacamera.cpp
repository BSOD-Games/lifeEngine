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
#include "common/ray.h"
#include "engine/camera.h"
#include "scriptsapi/luaenum.h"
#include "scriptsapi/common/luaray.h"
#include "scriptsapi/mathlib/luavector2d.h"
#include "scriptsapi/mathlib/luavector3d.h"
#include "scriptsapi/engine/luacamera.h"

// ------------------------------------------------------------------------------------ //
// Register camera in LUA
// ------------------------------------------------------------------------------------ //
void le::LUACamera::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Reguster camera side move
	LuaEnum			luaCameraSideMove( LuaVM );
	luaCameraSideMove.AddValue( "Backward", CSM_BACKWARD ).
		AddValue( "Forward", CSM_FORWARD ).
		AddValue( "Left", CSM_LEFT ).
		AddValue( "Right", CSM_RIGHT ).
		AddValue( "None", CSM_NONE );

	luabridge::setGlobal( LuaVM, luaCameraSideMove.GetHandle(), "CameraSideMove" );

	// Register input system
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUACamera>( "Camera" ).
		addConstructor< void (*)() >().
		addFunction( "InitProjection_Perspective", &LUACamera::InitProjection_Perspective ).
		addFunction( "InitProjection_Ortho", &LUACamera::InitProjection_Ortho ).
		addFunction( "Move", ( void ( LUACamera::* )( const LUAVector3D& ) ) &LUACamera::Move ).
		addFunction( "Move", ( void ( LUACamera::* )( UInt32_t, float ) ) &LUACamera::Move ).
		addFunction( "Rotate", &LUACamera::Rotate ).
		addFunction( "RotateByMouse", &LUACamera::RotateByMouse ).
		addFunction( "ScreenToWorld", &LUACamera::ScreenToWorld ).
		addFunction( "WorldToScreen", &LUACamera::WorldToScreen ).
		addFunction( "SetPosition", &LUACamera::SetPosition ).
		addFunction( "SetRotation", &LUACamera::SetRotation ).
		addFunction( "SetTargetDirection", &LUACamera::SetTargetDirection ).
		addFunction( "SetUp", &LUACamera::SetUp ).
		addFunction( "IsVisible", ( bool ( LUACamera::* )( const LUAVector3D&, const LUAVector3D& ) ) &LUACamera::IsVisible ).
		addFunction( "IsVisible", ( bool ( LUACamera::* )( const LUAVector3D&, float ) ) &LUACamera::IsVisible ).
		addFunction( "GetNear", &LUACamera::GetNear ).
		addFunction( "GetFar", &LUACamera::GetFar ).
		addFunction( "GetPosition", &LUACamera::GetPosition ).
		addFunction( "GetUp", &LUACamera::GetUp ).
		addFunction( "GetRight", &LUACamera::GetRight ).
		addFunction( "GetDirectionMove", &LUACamera::GetDirectionMove ).
		addFunction( "GetEulerRotation", &LUACamera::GetEulerRotation ).
		addFunction( "GetTargetDirection", &LUACamera::GetTargetDirection ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUACamera::LUACamera() :
	camera( new Camera() )
{
	camera->IncrementReference();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUACamera::LUACamera( const LUACamera& Copy ) :
	camera( Copy.camera )
{
	if ( camera ) camera->IncrementReference();
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::LUACamera::~LUACamera()
{
	if ( !camera )			return;

	if ( camera->GetCountReferences() <= 1 )
		camera->Release();
	else
		camera->DecrementReference();
}

// ------------------------------------------------------------------------------------ //
// Init projection - Perspective
// ------------------------------------------------------------------------------------ //
void le::LUACamera::InitProjection_Perspective( float FOV, float Aspect, float Near, float Far )
{
	camera->InitProjection_Perspective( FOV, Aspect, Near, Far );
}

// ------------------------------------------------------------------------------------ //
// Init projection - Ortho
// ------------------------------------------------------------------------------------ //
void le::LUACamera::InitProjection_Ortho( float Left, float Right, float Bottom, float Top, float Near, float Far )
{
	camera->InitProjection_Ortho( Left, Right, Bottom, Top, Near, Far );
}

// ------------------------------------------------------------------------------------ //
// Move
// ------------------------------------------------------------------------------------ //
void le::LUACamera::Move( const LUAVector3D& FactorMove )
{
	camera->Move( FactorMove.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Move
// ------------------------------------------------------------------------------------ //
void le::LUACamera::Move( UInt32_t SideMove, float MoveSpeed )
{
	camera->Move( ( CAMERA_SIDE_MOVE ) SideMove, MoveSpeed );
}

// ------------------------------------------------------------------------------------ //
// Rotate
// ------------------------------------------------------------------------------------ //
void le::LUACamera::Rotate( const LUAVector3D& FactorRotate )
{
	camera->Rotate( FactorRotate.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Rotate by mouse
// ------------------------------------------------------------------------------------ //
void le::LUACamera::RotateByMouse( const LUAVector2D& MouseOffset, float MouseSensitivity, bool ConstrainYaw )
{
	camera->RotateByMouse( MouseOffset.GetHandle(), MouseSensitivity, ConstrainYaw );
}

// ------------------------------------------------------------------------------------ //
// Screen to world
// ------------------------------------------------------------------------------------ //
le::LUARay le::LUACamera::ScreenToWorld( const LUAVector2D& Coords, const LUAVector2D& ViewportSize )
{
	return LUARay( camera->ScreenToWorld( Coords.GetHandle(), ViewportSize.GetHandle() ) );
}

// ------------------------------------------------------------------------------------ //
// World to screen
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUACamera::WorldToScreen( const LUAVector3D& Coords, const LUAVector2D& ViewportSize )
{
	return LUAVector3D( camera->WorldToScreen( Coords.GetHandle(), ViewportSize.GetHandle() ) );
}

// ------------------------------------------------------------------------------------ //
// Set position
// ------------------------------------------------------------------------------------ //
void le::LUACamera::SetPosition( const LUAVector3D& Position )
{
	camera->SetPosition( Position.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set rotation
// ------------------------------------------------------------------------------------ //
void le::LUACamera::SetRotation( const LUAVector3D& Rotation )
{
	camera->SetRotation( Rotation.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set target direction
// ------------------------------------------------------------------------------------ //
void le::LUACamera::SetTargetDirection( const LUAVector3D& TargetDirection )
{
	camera->SetTargetDirection( TargetDirection.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set up
// ------------------------------------------------------------------------------------ //
void le::LUACamera::SetUp( const LUAVector3D& Up )
{
	camera->SetUp( Up.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Is visible
// ------------------------------------------------------------------------------------ //
bool le::LUACamera::IsVisible( const LUAVector3D& MinPosition, const LUAVector3D& MaxPosition )
{
	return camera->IsVisible( MinPosition.GetHandle(), MaxPosition.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Is visible
// ------------------------------------------------------------------------------------ //
bool le::LUACamera::IsVisible( const LUAVector3D& Position, float Radius )
{
	return camera->IsVisible( Position.GetHandle(), Radius );
}

// ------------------------------------------------------------------------------------ //
// Get near
// ------------------------------------------------------------------------------------ //
float le::LUACamera::GetNear() const
{
	return camera->GetNear();
}

// ------------------------------------------------------------------------------------ //
// Get far
// ------------------------------------------------------------------------------------ //
float le::LUACamera::GetFar() const
{
	return camera->GetFar();
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUACamera::GetPosition() const
{
	return LUAVector3D( camera->GetPosition() );
}

// ------------------------------------------------------------------------------------ //
// Get up
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUACamera::GetUp() const
{
	return LUAVector3D( camera->GetUp() );
}

// ------------------------------------------------------------------------------------ //
// Get right
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUACamera::GetRight() const
{
	return LUAVector3D( camera->GetRight() );
}

// ------------------------------------------------------------------------------------ //
// Get direction move
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUACamera::GetDirectionMove( UInt32_t SideMove ) const
{
	return LUAVector3D( camera->GetDirectionMove( ( CAMERA_SIDE_MOVE ) SideMove ) );
}

// ------------------------------------------------------------------------------------ //
// Get euler rotation
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUACamera::GetEulerRotation() const
{
	return LUAVector3D( camera->GetEulerRotation() );
}

// ------------------------------------------------------------------------------------ //
// Get target direction
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUACamera::GetTargetDirection() const
{
	return LUAVector3D( camera->GetTargetDirection() );
}
