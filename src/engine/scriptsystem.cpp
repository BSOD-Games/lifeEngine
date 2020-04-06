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
#include "scripts_api/engine/consolesystem.h"
#include "scripts_api/engine/inputsystem.h"
#include "scripts_api/engine/resourcesystem.h"
#include "scripts_api/engine/model.h"
#include "scripts_api/engine/camera.h"
#include "scripts_api/engine/material.h"
#include "scripts_api/engine/materialproxy.h"
#include "scripts_api/engine/materialproxyvar.h"

#include "scripts_api/studiorender/shaderparameter.h"
#include "scripts_api/studiorender/studiorenderpass.h"
#include "scripts_api/studiorender/studiorendertechnique.h"
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
	REGISTER_FUNCTION( ResourceSystem_UnloadTexture );
	REGISTER_FUNCTION( ResourceSystem_UnloadMaterial );
	REGISTER_FUNCTION( ResourceSystem_UnloadMesh );
	REGISTER_FUNCTION( ResourceSystem_UnloadFont );
	REGISTER_FUNCTION( ResourceSystem_UnloadTextures );
	REGISTER_FUNCTION( ResourceSystem_UnloadMaterials );
	REGISTER_FUNCTION( ResourceSystem_UnloadMeshes );
	REGISTER_FUNCTION( ResourceSystem_UnloadFonts );
	REGISTER_FUNCTION( ResourceSystem_UnloadAll );
	REGISTER_FUNCTION( ResourceSystem_GetTexture );
	REGISTER_FUNCTION( ResourceSystem_GetMaterial );
	REGISTER_FUNCTION( ResourceSystem_GetMesh );
	REGISTER_FUNCTION( ResourceSystem_GetFont );

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
	REGISTER_FUNCTION( Material_AddTechnique );
	REGISTER_FUNCTION( Material_GetTechniques );
	REGISTER_FUNCTION( Material_GetSurfaceName );
	REGISTER_FUNCTION( Material_SetSurfaceName );
	REGISTER_FUNCTION( Material_RemoveTechnique );
	REGISTER_FUNCTION( Material_GetCountTechniques );
	REGISTER_FUNCTION( Material_GetTechniqueByType );
	REGISTER_FUNCTION( Material_GetTechniqueByIndex );

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

	// Studiorender pass
	REGISTER_FUNCTION( StudioRenderPass_Create );
	REGISTER_FUNCTION( StudioRenderPass_Delete );
	REGISTER_FUNCTION( StudioRenderPass_Clear );
	REGISTER_FUNCTION( StudioRenderPass_IsBlend );
	REGISTER_FUNCTION( StudioRenderPass_AddProxy );
	REGISTER_FUNCTION( StudioRenderPass_GetProxy );
	REGISTER_FUNCTION( StudioRenderPass_GetProxes );
	REGISTER_FUNCTION( StudioRenderPass_SetShader );
	REGISTER_FUNCTION( StudioRenderPass_IsCullFace );
	REGISTER_FUNCTION( StudioRenderPass_EnableBlend );
	REGISTER_FUNCTION( StudioRenderPass_IsDepthTest );
	REGISTER_FUNCTION( StudioRenderPass_RemoveProxy );
	REGISTER_FUNCTION( StudioRenderPass_AddParameter );
	REGISTER_FUNCTION( StudioRenderPass_GetParameter );
	REGISTER_FUNCTION( StudioRenderPass_IsDepthWrite );
	REGISTER_FUNCTION( StudioRenderPass_FindParameter );
	REGISTER_FUNCTION( StudioRenderPass_GetNameShader );
	REGISTER_FUNCTION( StudioRenderPass_GetParameters );
	REGISTER_FUNCTION( StudioRenderPass_EnableCullFace );
	REGISTER_FUNCTION( StudioRenderPass_GetCountProxes );
	REGISTER_FUNCTION( StudioRenderPass_EnableDepthTest );
	REGISTER_FUNCTION( StudioRenderPass_GetCullFaceType );
	REGISTER_FUNCTION( StudioRenderPass_RemoveParameter );
	REGISTER_FUNCTION( StudioRenderPass_SetCullFaceType );
	REGISTER_FUNCTION( StudioRenderPass_EnableDepthWrite );
	REGISTER_FUNCTION( StudioRenderPass_GetCountParameters );

	// Studiorender technique
	REGISTER_FUNCTION( StudioRenderTechnique_Create );
	REGISTER_FUNCTION( StudioRenderTechnique_Delete );
	REGISTER_FUNCTION( StudioRenderTechnique_Clear );
	REGISTER_FUNCTION( StudioRenderTechnique_AddPass );
	REGISTER_FUNCTION( StudioRenderTechnique_GetPass );
	REGISTER_FUNCTION( StudioRenderTechnique_GetType );
	REGISTER_FUNCTION( StudioRenderTechnique_SetType );
	REGISTER_FUNCTION( StudioRenderTechnique_GetPasses );
	REGISTER_FUNCTION( StudioRenderTechnique_RemovePass );
	REGISTER_FUNCTION( StudioRenderTechnique_GetCountPasses );

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
