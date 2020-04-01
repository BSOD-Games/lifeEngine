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
#include "scripts_api/consolesystem.h"
#include "scripts_api/inputsystem.h"
#include "scripts_api/model.h"

// ------------------------------------------------------------------------------------ //
// Register symbol
// ------------------------------------------------------------------------------------ //
void le::ScriptSystem::RegisterSymbol( const char* Name, void* Value )
{
	auto	itSymbol = symbols.find( Name );
	if ( itSymbol != symbols.end() )
		return;

	symbols[ Name ] = Value;
}

// ------------------------------------------------------------------------------------ //
// Add script
// ------------------------------------------------------------------------------------ //
void le::ScriptSystem::AddScript( le::IScript* Script )
{
	LIFEENGINE_ASSERT( Script );
	le::Script*		script = static_cast< le::Script* >( Script );

	script->IncrementReference();
	scripts.push_back( script );
}

// ------------------------------------------------------------------------------------ //
// Update script
// ------------------------------------------------------------------------------------ //
void le::ScriptSystem::RemoveScript( le::UInt32_t Index )
{
	if ( Index >= scripts.size() ) return;
	Script*         script = scripts[ Index ];

	if ( script->GetCountReferences() <= 1 )
		script->Release();
	else
		script->DecrementReference();

	scripts.erase( scripts.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Update script
// ------------------------------------------------------------------------------------ //
void le::ScriptSystem::RemoveScript( le::IScript* Script )
{
	if ( scripts.empty() ) return;

	for ( UInt32_t index = 0, count = scripts.size(); index < count; ++index )
		if ( scripts[ index ] == Script )
		{
			if ( Script->GetCountReferences() <= 1 )
				Script->Release();
			else
				Script->DecrementReference();

			scripts.erase( scripts.begin() + index );
			break;
		}
}

// ------------------------------------------------------------------------------------ //
// Update script
// ------------------------------------------------------------------------------------ //
void le::ScriptSystem::RemoveAllScripts()
{
	if ( scripts.empty() ) return;

	for ( UInt32_t index = 0, count = scripts.size(); index < count; ++index )
	{
		Script*			script = scripts[ index ];
		if ( script->GetCountReferences() <= 1 )
			script->Release();
		else
			script->DecrementReference();
	}

	scripts.clear();
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
le::UInt32_t le::ScriptSystem::GetCountScripts() const
{
	return scripts.size();
}

// ------------------------------------------------------------------------------------ //
// Update script
// ------------------------------------------------------------------------------------ //
le::IScript* le::ScriptSystem::GetScript( le::UInt32_t Index ) const
{
	if ( Index >= scripts.size() ) return nullptr;
	return scripts[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Update script
// ------------------------------------------------------------------------------------ //
le::IScript** le::ScriptSystem::GetScrips() const
{
	return ( IScript** ) scripts.data();
}

// ------------------------------------------------------------------------------------ //
// Update script
// ------------------------------------------------------------------------------------ //
bool le::ScriptSystem::Initialize( le::IEngine* Engine )
{
	RegisterConsoleSystem();
	RegisterInputSystem();
	return true;
}

// ------------------------------------------------------------------------------------ //
// Update scripts
// ------------------------------------------------------------------------------------ //
void le::ScriptSystem::Update()
{
	if ( scripts.empty() ) return;
	for ( UInt32_t index = 0, count = scripts.size(); index < count; ++index )
		scripts[ index ]->Update();
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
{
	RemoveAllScripts();
}

// ------------------------------------------------------------------------------------ //
// Register console system
// ------------------------------------------------------------------------------------ //
void le::ScriptSystem::RegisterConsoleSystem()
{
	symbols[ "Console_Info" ] = ( void* ) &Console_Info;
	symbols[ "Console_Error" ] = ( void* ) &Console_Error;
	symbols[ "Console_Warning" ] = ( void* ) &Console_Warning;
}

// ------------------------------------------------------------------------------------ //
// Register input system
// ------------------------------------------------------------------------------------ //
void le::ScriptSystem::RegisterInputSystem()
{
	symbols[ "InputSystem_IsKeyUp" ] = ( void* ) &InputSystem_IsKeyUp;
	symbols[ "InputSystem_IsKeyDown" ] = ( void* ) &InputSystem_IsKeyDown;
	symbols[ "InputSystem_IsMouseKeyUp" ] = ( void* ) &InputSystem_IsMouseKeyUp;
	symbols[ "InputSystem_IsMouseKeyDown" ] = ( void* ) &InputSystem_IsMouseKeyDown;
	symbols[ "InputSystem_IsMouseWheel" ] = ( void* ) &InputSystem_IsMouseWheel;

	symbols[ "Model_Move" ] = ( void* ) &Model_Move;
	symbols[ "Model_Scale" ] = ( void* ) &Model_Scale;
	symbols[ "Model_GetScale" ] = ( void* ) &Model_GetScale;
	symbols[ "Model_SetScale" ] = ( void* ) &Model_SetScale;
	symbols[ "Model_QuatRotate" ] = ( void* ) &Model_QuatRotate;
	symbols[ "Model_EulerRotate" ] = ( void* ) &Model_EulerRotate;
	symbols[ "Model_GetPosition" ] = ( void* ) &Model_GetPosition;
	symbols[ "Model_GetRotation" ] = ( void* ) &Model_GetRotation;
	symbols[ "Model_SetPosition" ] = ( void* ) &Model_SetPosition;
	symbols[ "Model_SetQuatRotation" ] = ( void* ) &Model_SetQuatRotation;
	symbols[ "Model_SetEulerRotation" ] = ( void* ) &Model_SetEulerRotation;
}
