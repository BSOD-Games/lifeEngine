//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "engine/lifeengine.h"
#include "engine/consolesystem.h"
#include "engine/scriptsystem.h"
#include "engine/script.h"

namespace scripts_api
{
#include "scripts/engine/consolesystem.h"
#include "scripts/engine/inputsystem.h"
#include "scripts/engine/resourcesystem.h"
#include "scripts/engine/model.h"
#include "scripts/engine/camera.h"
#include "scripts/engine/materialproxy.h"
#include "scripts/engine/materialproxyvar.h"
#include "scripts/engine/level.h"
#include "scripts/engine/sprite.h"
#include "scripts/engine/text.h"
#include "scripts/engine/shaderparameter.h"
#include "scripts/engine/material.h"

#include "scripts/physics/collider.h"
#include "scripts/physics/body.h"
#include "scripts/physics/charctercontroller.h"
#include "scripts/physics/physicssystem.h"
#include "scripts/physics/physicsmodel.h"
}

#define REGISTER_FUNCTION( Function )		( functions[ #Function ] = ( void* ) &Function )

// ------------------------------------------------------------------------------------ //
// Register function
// ------------------------------------------------------------------------------------ //
void le::ScriptSystem::RegisterFunction( const char* Name, void* Value )
{
	auto	itFunction = functions.find( Name );
	if ( itFunction != functions.end() )	return;

	functions[ Name ] = Value;
}

// ------------------------------------------------------------------------------------ //
// Register var
// ------------------------------------------------------------------------------------ //
void le::ScriptSystem::RegisterVar( const char* Name, void* Value )
{
	auto	itVar = vars.find( Name );
	if ( itVar != vars.end() )		return;

	vars[ Name ] = Value;
}

// ------------------------------------------------------------------------------------ //
// Unregister function
// ------------------------------------------------------------------------------------ //
void le::ScriptSystem::UnregisterFunction( const char* Name )
{
	auto	itFunction = functions.find( Name );
	if ( itFunction == functions.end() )	return;

	functions.erase( itFunction );
}

// ------------------------------------------------------------------------------------ //
// Unregister var
// ------------------------------------------------------------------------------------ //
void le::ScriptSystem::UnregisterVar( const char* Name )
{
	auto	itVar = vars.find( Name );
	if ( itVar == vars.end() )	return;

	vars.erase( itVar );
}

// ------------------------------------------------------------------------------------ //
// Update script
// ------------------------------------------------------------------------------------ //
le::IFactory* le::ScriptSystem::GetFactory() const
{
	return ( IFactory* ) &factory;
}

// ------------------------------------------------------------------------------------ //
// Update script
// ------------------------------------------------------------------------------------ //
bool le::ScriptSystem::Initialize( le::IEngine* Engine )
{
	using namespace scripts_api;

	// Console system
	REGISTER_FUNCTION( ConsoleSystem_Info );
	REGISTER_FUNCTION( ConsoleSystem_Error );
	REGISTER_FUNCTION( ConsoleSystem_Warning );
	REGISTER_FUNCTION( ConsoleSystem_Exec );

	// Input system
	REGISTER_FUNCTION( InputSystem_IsKeyUp );
	REGISTER_FUNCTION( InputSystem_IsKeyDown );
	REGISTER_FUNCTION( InputSystem_IsMouseKeyUp );
	REGISTER_FUNCTION( InputSystem_IsMouseKeyDown );
	REGISTER_FUNCTION( InputSystem_IsMouseWheel );
	REGISTER_FUNCTION( InputSystem_GetMouseOffset );
	REGISTER_FUNCTION( InputSystem_GetMousePosition );
	REGISTER_FUNCTION( InputSystem_GetMouseSensitivity );

	// Resource system
	REGISTER_FUNCTION( ResourceSystem_LoadTexture );
	REGISTER_FUNCTION( ResourceSystem_LoadMaterial );
	REGISTER_FUNCTION( ResourceSystem_LoadMesh );
	REGISTER_FUNCTION( ResourceSystem_LoadFont );
	REGISTER_FUNCTION( ResourceSystem_LoadPhysicsModel );
	REGISTER_FUNCTION( ResourceSystem_UnloadTexture );
	REGISTER_FUNCTION( ResourceSystem_UnloadMaterial );
	REGISTER_FUNCTION( ResourceSystem_UnloadMesh );
	REGISTER_FUNCTION( ResourceSystem_UnloadFont );
	REGISTER_FUNCTION( ResourceSystem_UnloadPhysicsModel );
	REGISTER_FUNCTION( ResourceSystem_UnloadTextures );
	REGISTER_FUNCTION( ResourceSystem_UnloadMaterials );
	REGISTER_FUNCTION( ResourceSystem_UnloadMeshes );
	REGISTER_FUNCTION( ResourceSystem_UnloadFonts );
	REGISTER_FUNCTION( ResourceSystem_UnloadPhysicsModels );
	REGISTER_FUNCTION( ResourceSystem_UnloadAll );
	REGISTER_FUNCTION( ResourceSystem_GetTexture );
	REGISTER_FUNCTION( ResourceSystem_GetMaterial );
	REGISTER_FUNCTION( ResourceSystem_GetMesh );
	REGISTER_FUNCTION( ResourceSystem_GetFont );
	REGISTER_FUNCTION( ResourceSystem_GetPhysicsModel );

	// Camera
	REGISTER_FUNCTION( Camera_Create );
	REGISTER_FUNCTION( Camera_Delete );
	REGISTER_FUNCTION( Camera_InitProjectionPerspective );
	REGISTER_FUNCTION( Camera_InitProjectionOrtho );
	REGISTER_FUNCTION( Camera_MoveByVector );
	REGISTER_FUNCTION( Camera_MoveBySide );
	REGISTER_FUNCTION( Camera_RotateByVector );
	REGISTER_FUNCTION( Camera_RotateByMouse );
	REGISTER_FUNCTION( Camera_ScreenToWorld );
	REGISTER_FUNCTION( Camera_WorldToScreen );
	REGISTER_FUNCTION( Camera_SetPosition );
	REGISTER_FUNCTION( Camera_SetRotation );
	REGISTER_FUNCTION( Camera_SetTargetDirection );
	REGISTER_FUNCTION( Camera_SetUp );
	REGISTER_FUNCTION( Camera_IsVisibleBBox );
	REGISTER_FUNCTION( Camera_IsVisibleSphere );
	REGISTER_FUNCTION( Camera_GetNear );
	REGISTER_FUNCTION( Camera_GetFar );
	REGISTER_FUNCTION( Camera_GetPosition );
	REGISTER_FUNCTION( Camera_GetUp );
	REGISTER_FUNCTION( Camera_GetRight );
	REGISTER_FUNCTION( Camera_GetDirectionMove );
	REGISTER_FUNCTION( Camera_GetQuatRotation );
	REGISTER_FUNCTION( Camera_GetEulerRotation );
	REGISTER_FUNCTION( Camera_GetTargetDirection );

	// Material
	REGISTER_FUNCTION( Material_Create );
	REGISTER_FUNCTION( Material_Delete );
	REGISTER_FUNCTION( Material_Clear );
	REGISTER_FUNCTION( Material_GetSurfaceName );
	REGISTER_FUNCTION( Material_SetSurfaceName );
	REGISTER_FUNCTION( Material_AddParameter );
	REGISTER_FUNCTION( Material_AddProxy );
	REGISTER_FUNCTION( Material_EnableBlend );
	REGISTER_FUNCTION( Material_EnableCullFace );
	REGISTER_FUNCTION( Material_EnableDepthTest );
	REGISTER_FUNCTION( Material_EnableDepthWrite );
	REGISTER_FUNCTION( Material_FindParameter );
	REGISTER_FUNCTION( Material_GetCountParameters );
	REGISTER_FUNCTION( Material_GetCountProxes );
	REGISTER_FUNCTION( Material_GetCullFaceType );
	REGISTER_FUNCTION( Material_GetParameter );
	REGISTER_FUNCTION( Material_GetParameters );
	REGISTER_FUNCTION( Material_GetProxes );
	REGISTER_FUNCTION( Material_GetProxy );
	REGISTER_FUNCTION( Material_IsBlend );
	REGISTER_FUNCTION( Material_IsCullFace );
	REGISTER_FUNCTION( Material_IsDepthTest );
	REGISTER_FUNCTION( Material_IsDepthWrite );
	REGISTER_FUNCTION( Material_RemoveParameter );
	REGISTER_FUNCTION( Material_RemoveProxy );
	REGISTER_FUNCTION( Material_SetCullFaceType );
	REGISTER_FUNCTION( Material_SetShader );

	// Material proxy
	REGISTER_FUNCTION( MaterialProxy_Create );
	REGISTER_FUNCTION( MaterialProxy_Delete );
	REGISTER_FUNCTION( MaterialProxy_GetVar );
	REGISTER_FUNCTION( MaterialProxy_SetVar );
	REGISTER_FUNCTION( MaterialProxy_Update );
	REGISTER_FUNCTION( MaterialProxy_GetName );
	REGISTER_FUNCTION( MaterialProxy_ClearVar );
	REGISTER_FUNCTION( MaterialProxy_NeadUpdate );
	REGISTER_FUNCTION( MaterialProxy_ClearAllVars );
	REGISTER_FUNCTION( MaterialProxy_IsNeadUpdate );

	// Material proxy var
	REGISTER_FUNCTION( MaterialProxyVar_Create );
	REGISTER_FUNCTION( MaterialProxyVar_Delete );
	REGISTER_FUNCTION( MaterialProxyVar_Clear );
	REGISTER_FUNCTION( MaterialProxyVar_GetName );
	REGISTER_FUNCTION( MaterialProxyVar_GetType );
	REGISTER_FUNCTION( MaterialProxyVar_SetName );
	REGISTER_FUNCTION( MaterialProxyVar_IsDefined );
	REGISTER_FUNCTION( MaterialProxyVar_GetValueInt );
	REGISTER_FUNCTION( MaterialProxyVar_SetValueInt );
	REGISTER_FUNCTION( MaterialProxyVar_GetValueBool );
	REGISTER_FUNCTION( MaterialProxyVar_GetValueVec2 );
	REGISTER_FUNCTION( MaterialProxyVar_GetValueVec3 );
	REGISTER_FUNCTION( MaterialProxyVar_GetValueVec4 );
	REGISTER_FUNCTION( MaterialProxyVar_SetValueBool );
	REGISTER_FUNCTION( MaterialProxyVar_SetValueVec2 );
	REGISTER_FUNCTION( MaterialProxyVar_SetValueVec3 );
	REGISTER_FUNCTION( MaterialProxyVar_SetValueVec4 );
	REGISTER_FUNCTION( MaterialProxyVar_GetValueFloat );
	REGISTER_FUNCTION( MaterialProxyVar_SetValueFloat );
	REGISTER_FUNCTION( MaterialProxyVar_GetValueShaderParameter );
	REGISTER_FUNCTION( MaterialProxyVar_SetValueShaderParameter );
	REGISTER_FUNCTION( MaterialProxyVar_GetValueArrayInt );
	REGISTER_FUNCTION( MaterialProxyVar_SetValueArrayInt );
	REGISTER_FUNCTION( MaterialProxyVar_GetValueArrayFloat );
	REGISTER_FUNCTION( MaterialProxyVar_SetValueArrayFloat );
	REGISTER_FUNCTION( MaterialProxyVar_GetValueArrayVec2 );
	REGISTER_FUNCTION( MaterialProxyVar_SetValueArrayVec2 );
	REGISTER_FUNCTION( MaterialProxyVar_GetValueArrayVec3 );
	REGISTER_FUNCTION( MaterialProxyVar_SetValueArrayVec3 );
	REGISTER_FUNCTION( MaterialProxyVar_GetValueArrayVec4 );
	REGISTER_FUNCTION( MaterialProxyVar_SetValueArrayVec4 );

	// Model
	REGISTER_FUNCTION( Model_Create );
	REGISTER_FUNCTION( Model_Delete );
	REGISTER_FUNCTION( Model_Move );
	REGISTER_FUNCTION( Model_Scale );
	REGISTER_FUNCTION( Model_GetScale );
	REGISTER_FUNCTION( Model_SetScale );
	REGISTER_FUNCTION( Model_QuatRotate );
	REGISTER_FUNCTION( Model_EulerRotate );
	REGISTER_FUNCTION( Model_GetPosition );
	REGISTER_FUNCTION( Model_GetRotation );
	REGISTER_FUNCTION( Model_SetPosition );
	REGISTER_FUNCTION( Model_SetQuatRotation );
	REGISTER_FUNCTION( Model_SetEulerRotation );
	REGISTER_FUNCTION( Model_SetMesh );
	REGISTER_FUNCTION( Model_SetStartFace );
	REGISTER_FUNCTION( Model_SetCountFace);
	REGISTER_FUNCTION( Model_GetMesh );
	REGISTER_FUNCTION( Model_GetStartFace );
	REGISTER_FUNCTION( Model_GetCountFace );

	// Shader parameter
	REGISTER_FUNCTION( ShaderParameter_Create );
	REGISTER_FUNCTION( ShaderParameter_Delete );
	REGISTER_FUNCTION( ShaderParameter_Clear );
	REGISTER_FUNCTION( ShaderParameter_GetName );
	REGISTER_FUNCTION( ShaderParameter_GetType );
	REGISTER_FUNCTION( ShaderParameter_SetName );
	REGISTER_FUNCTION( ShaderParameter_IsDefined );
	REGISTER_FUNCTION( ShaderParameter_GetValueInt );
	REGISTER_FUNCTION( ShaderParameter_SetValueInt );
	REGISTER_FUNCTION( ShaderParameter_GetValueMat4 );
	REGISTER_FUNCTION( ShaderParameter_GetValueVec2 );
	REGISTER_FUNCTION( ShaderParameter_GetValueVec3 );
	REGISTER_FUNCTION( ShaderParameter_GetValueVec4 );
	REGISTER_FUNCTION( ShaderParameter_SetValueMat4 );
	REGISTER_FUNCTION( ShaderParameter_SetValueVec2 );
	REGISTER_FUNCTION( ShaderParameter_SetValueVec3 );
	REGISTER_FUNCTION( ShaderParameter_SetValueVec4 );
	REGISTER_FUNCTION( ShaderParameter_GetValueFloat );
	REGISTER_FUNCTION( ShaderParameter_SetValueFloat );
	REGISTER_FUNCTION( ShaderParameter_GetValueTexture );
	REGISTER_FUNCTION( ShaderParameter_SetValueTexture );
	REGISTER_FUNCTION( ShaderParameter_GetValueShaderFlag );
	REGISTER_FUNCTION( ShaderParameter_SetValueShaderFlag );

	// Level
	REGISTER_FUNCTION( Level_Clear );
	REGISTER_FUNCTION( Level_IsLoaded );
	REGISTER_FUNCTION( Level_AddCamera );
	REGISTER_FUNCTION( Level_AddEntity );
	REGISTER_FUNCTION( Level_GetCamera );
	REGISTER_FUNCTION( Level_GetEntity );
	REGISTER_FUNCTION( Level_GetNameFormat );
	REGISTER_FUNCTION( Level_GetCountCameras );
	REGISTER_FUNCTION( Level_GetCountEntityes );
	REGISTER_FUNCTION( Level_RemoveCameraByPtr );
	REGISTER_FUNCTION( Level_RemoveEntityByPtr );
	REGISTER_FUNCTION( Level_RemoveCameraByIndex );
	REGISTER_FUNCTION( Level_RemoveEntityByIndex );

	// Sprite
	REGISTER_FUNCTION( Sprite_Create );
	REGISTER_FUNCTION( Sprite_Move );
	REGISTER_FUNCTION( Sprite_Scale );
	REGISTER_FUNCTION( Sprite_Delete );
	REGISTER_FUNCTION( Sprite_GetMax );
	REGISTER_FUNCTION( Sprite_GetMin );
	REGISTER_FUNCTION( Sprite_GetSize );
	REGISTER_FUNCTION( Sprite_GetType );
	REGISTER_FUNCTION( Sprite_SetSize );
	REGISTER_FUNCTION( Sprite_SetType );
	REGISTER_FUNCTION( Sprite_GetScale );
	REGISTER_FUNCTION( Sprite_IsCreate );
	REGISTER_FUNCTION( Sprite_SetScale );
	REGISTER_FUNCTION( Sprite_Initialize );
	REGISTER_FUNCTION( Sprite_QuatRotate );
	REGISTER_FUNCTION( Sprite_EulerRotate );
	REGISTER_FUNCTION( Sprite_GetMaterial );
	REGISTER_FUNCTION( Sprite_GetPosition );
	REGISTER_FUNCTION( Sprite_GetRotation );
	REGISTER_FUNCTION( Sprite_SetMaterial );
	REGISTER_FUNCTION( Sprite_SetPosition );
	REGISTER_FUNCTION( Sprite_SetQuatRotation );
	REGISTER_FUNCTION( Sprite_SetEulerRotation );

	// Text
	REGISTER_FUNCTION( Text_Create );
	REGISTER_FUNCTION( Text_Move );
	REGISTER_FUNCTION( Text_Scale );
	REGISTER_FUNCTION( Text_Delete );
	REGISTER_FUNCTION( Text_GetFont );
	REGISTER_FUNCTION( Text_GetText );
	REGISTER_FUNCTION( Text_SetFont );
	REGISTER_FUNCTION( Text_SetText );
	REGISTER_FUNCTION( Text_GetMaterial );
	REGISTER_FUNCTION( Text_GetScale );
	REGISTER_FUNCTION( Text_SetMaterial );
	REGISTER_FUNCTION( Text_SetScale );
	REGISTER_FUNCTION( Text_QuatRotate );
	REGISTER_FUNCTION( Text_EulerRotate );
	REGISTER_FUNCTION( Text_GetPosition );
	REGISTER_FUNCTION( Text_GetRotation );
	REGISTER_FUNCTION( Text_SetPosition );
	REGISTER_FUNCTION( Text_SetQuatRotation );
	REGISTER_FUNCTION( Text_GetCharacterSize );
	REGISTER_FUNCTION( Text_SetCharacterSize );
	REGISTER_FUNCTION( Text_SetEulerRotation );
	REGISTER_FUNCTION( Text_GetLineSpacingFactor );
	REGISTER_FUNCTION( Text_SetLineSpacingFactor );
	REGISTER_FUNCTION( Text_GetLetterSpacingFactor );
	REGISTER_FUNCTION( Text_SetLetterSpacingFactor );

	// Collider
	REGISTER_FUNCTION( Collider_AddBox );
	REGISTER_FUNCTION( Collider_Create );
	REGISTER_FUNCTION( Collider_Delete );
	REGISTER_FUNCTION( Collider_AddCone );
	REGISTER_FUNCTION( Collider_AddMesh );
	REGISTER_FUNCTION( Collider_GetAABB );
	REGISTER_FUNCTION( Collider_AddSphere );
	REGISTER_FUNCTION( Collider_AddCapsule );
	REGISTER_FUNCTION( Collider_AddCylinder );
	REGISTER_FUNCTION( Collider_RemoveShape );
	REGISTER_FUNCTION( Collider_AddConvexHull );
	REGISTER_FUNCTION( Collider_AddPhysicsModel );
	REGISTER_FUNCTION( Collider_GetCountShapes );
	REGISTER_FUNCTION( Collider_RemoveAllShapes );
	REGISTER_FUNCTION( Collider_Scale );
	REGISTER_FUNCTION( Collider_SetScale );
	REGISTER_FUNCTION( Collider_GetScale );

	// Body
	REGISTER_FUNCTION( Body_Create );
	REGISTER_FUNCTION( Body_Delete );
	REGISTER_FUNCTION( Body_GetMass );
	REGISTER_FUNCTION( Body_Activate );
	REGISTER_FUNCTION( Body_IsStatic );
	REGISTER_FUNCTION( Body_ApplyForce );
	REGISTER_FUNCTION( Body_GetInertia );
	REGISTER_FUNCTION( Body_Initialize );
	REGISTER_FUNCTION( Body_GetCollider );
	REGISTER_FUNCTION( Body_GetPosition );
	REGISTER_FUNCTION( Body_GetRotation );
	REGISTER_FUNCTION( Body_SetPosition );
	REGISTER_FUNCTION( Body_SetRotation );
	REGISTER_FUNCTION( Body_ApplyImpulse );
	REGISTER_FUNCTION( Body_FreezeRotation );

	// Charcter controller
	REGISTER_FUNCTION( CharcterController_Jump );
	REGISTER_FUNCTION( CharcterController_Walk );
	REGISTER_FUNCTION( CharcterController_Create );
	REGISTER_FUNCTION( CharcterController_Delete );
	REGISTER_FUNCTION( CharcterController_CanJump );
	REGISTER_FUNCTION( CharcterController_OnGround );
	REGISTER_FUNCTION( CharcterController_Initialize );
	REGISTER_FUNCTION( CharcterController_GetPosition );
	REGISTER_FUNCTION( CharcterController_GetRotation );
	REGISTER_FUNCTION( CharcterController_GetVelocity );
	REGISTER_FUNCTION( CharcterController_SetPosition );
	REGISTER_FUNCTION( CharcterController_SetRotation );
	REGISTER_FUNCTION( CharcterController_SetVelocity );

	// Physics system
	REGISTER_FUNCTION( PhysicsSystem_AddBody );
	REGISTER_FUNCTION( PhysicsSystem_GetBody );
	REGISTER_FUNCTION( PhysicsSystem_GetBodies );
	REGISTER_FUNCTION( PhysicsSystem_GetGravity );
	REGISTER_FUNCTION( PhysicsSystem_RemoveBody );
	REGISTER_FUNCTION( PhysicsSystem_SetGravity );
	REGISTER_FUNCTION( PhysicsSystem_GetCountBodes );
	REGISTER_FUNCTION( PhysicsSystem_SetDebugCamera );
	REGISTER_FUNCTION( PhysicsSystem_RemoveAllBodies );
	REGISTER_FUNCTION( PhysicsSystem_AddCharcterController );
	REGISTER_FUNCTION( PhysicsSystem_GetCharcterController );
	REGISTER_FUNCTION( PhysicsSystem_GetCharcterControllers );
	REGISTER_FUNCTION( PhysicsSystem_RemoveCharcterController );
	REGISTER_FUNCTION( PhysicsSystem_GetCountCharcterControllers );
	REGISTER_FUNCTION( PhysicsSystem_RemoveAllCharcterControllers );

	// Physics model
	REGISTER_FUNCTION( PhysicsModel_Create );
	REGISTER_FUNCTION( PhysicsModel_Delete );
	REGISTER_FUNCTION( PhysicsModel_ClearMesh );
	REGISTER_FUNCTION( PhysicsModel_InitializeMesh );
	REGISTER_FUNCTION( PhysicsModel_IsInitializedMesh );

	return true;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::ScriptSystem::ScriptSystem()
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::ScriptSystem::~ScriptSystem()
{}
