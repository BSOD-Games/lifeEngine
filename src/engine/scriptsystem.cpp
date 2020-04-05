//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/lifeengine.h"
#include "engine/consolesystem.h"
#include "engine/scriptsystem.h"
#include "engine/script.h"

#include "global.h"
#include "scripts_api/engine/consolesystem.h"
#include "scripts_api/engine/inputsystem.h"
#include "scripts_api/engine/resourcesystem.h"
#include "scripts_api/engine/model.h"

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

	// Model
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
