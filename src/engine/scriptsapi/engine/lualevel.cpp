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
#include "engine/ilevel.h"
#include "engine/consolesystem.h"
#include "scriptsapi/engine/luacamera.h"
#include "scriptsapi/engine/lualevel.h"
#include "scriptsapi/engine/luaentity.h"

//---------------------------------------------------------------------//

le::LUALevel::EntityFactory			le::LUALevel::entityFactory;

//---------------------------------------------------------------------//

// ------------------------------------------------------------------------------------ //
// Register level in LUA
// ------------------------------------------------------------------------------------ //
void le::LUALevel::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Registern level
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUALevel>( "Level" ).
		addConstructor< void (*)() >().
		addFunction( "Update", &LUALevel::Update ).
		addFunction( "Render", &LUALevel::Render ).
		addFunction( "Clear", &LUALevel::Clear ).
		addFunction( "AddCamera", &LUALevel::AddCamera ).
		addFunction( "RemoveCamera", ( void ( LUALevel::* )( const LUACamera& ) ) &LUALevel::RemoveCamera ).
		addFunction( "RemoveCamera", ( void ( LUALevel::* )( UInt32_t ) ) &LUALevel::RemoveCamera ).
		addFunction( "IsLoaded", &LUALevel::IsLoaded ).
		addFunction( "GetNameFormat", &LUALevel::GetNameFormat ).
		addFunction( "GetCountCameras", &LUALevel::GetCountCameras ).
		addFunction( "GetCamera", &LUALevel::GetCamera ).
		addStaticFunction( "RegisterEntity", &LUALevel::RegisterEntity ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
void le::LUALevel::RegisterEntity( const char* Name, luabridge::LuaRef Class )
{
	entityFactory.Register( Name, Class );
	g_consoleSystem->PrintInfo( "Entity class [%s] registered", Name );
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUALevel::LUALevel() :
	object( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUALevel::LUALevel( ILevel* Level ) :
	object( Level )
{	
	if ( object )		object->IncrementReference();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUALevel::LUALevel( const LUALevel& Copy ) :
	object( Copy.GetLevel() )
{
	if ( object )		object->IncrementReference();
}

// ------------------------------------------------------------------------------------ //
// Descriptor
// ------------------------------------------------------------------------------------ //
le::LUALevel::~LUALevel()
{
	if ( !object )			return;

	if ( object->GetCountReferences() <= 1 )
		object->Release();
	else
		object->DecrementReference();
}

// ------------------------------------------------------------------------------------ //
// Update
// ------------------------------------------------------------------------------------ //
void le::LUALevel::Update()
{
	object->Update();
}

// ------------------------------------------------------------------------------------ //
// Render
// ------------------------------------------------------------------------------------ //
void le::LUALevel::Render()
{
	object->Render();
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void le::LUALevel::Clear()
{
	object->Clear();
}

// ------------------------------------------------------------------------------------ //
// Add camera
// ------------------------------------------------------------------------------------ //
void le::LUALevel::AddCamera( const LUACamera& Camera )
{
	object->AddCamera( Camera.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Remove camera
// ------------------------------------------------------------------------------------ //
void le::LUALevel::RemoveCamera( const LUACamera& Camera )
{
	object->RemoveCamera( Camera.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Remove camera
// ------------------------------------------------------------------------------------ //
void le::LUALevel::RemoveCamera( UInt32_t Index )
{
	object->RemoveCamera( Index );
}

// ------------------------------------------------------------------------------------ //
// Is loaded
// ------------------------------------------------------------------------------------ //
bool le::LUALevel::IsLoaded() const
{
	return object->IsLoaded();
}

// ------------------------------------------------------------------------------------ //
// Get name format
// ------------------------------------------------------------------------------------ //
const char* le::LUALevel::GetNameFormat() const
{
	return object->GetNameFormat();
}

// ------------------------------------------------------------------------------------ //
// Get count cameras
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::LUALevel::GetCountCameras() const
{
	return object->GetCountCameras();
}

// ------------------------------------------------------------------------------------ //
// Get camera
// ------------------------------------------------------------------------------------ //
le::LUACamera le::LUALevel::GetCamera( UInt32_t Index ) const
{
	return LUACamera( object->GetCamera( Index ) );
}

// ------------------------------------------------------------------------------------ //
// Create entity
// ------------------------------------------------------------------------------------ //
void* le::LUALevel::EntityFactory::Create( const char* NameInterface )
{
	auto itEntity = entites.find( NameInterface );
	if ( itEntity == entites.end() )		return nullptr;
	
	return new LUAEntity( *itEntity->second );
}

// ------------------------------------------------------------------------------------ //
// Delete entity
// ------------------------------------------------------------------------------------ //
void le::LUALevel::EntityFactory::Delete( void* Object )
{
	if ( !Object ) return;
	delete Object;
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::LUALevel::EntityFactory::~EntityFactory()
{
	for ( auto it = entites.begin(), itEnd = entites.end(); it != itEnd; ++it )
		delete it->second;
}

// ------------------------------------------------------------------------------------ //
// Register entity
// ------------------------------------------------------------------------------------ //
void le::LUALevel::EntityFactory::Register( std::string Name, luabridge::LuaRef Class )
{
	entites[ Name ] = new luabridge::LuaRef( Class );
}
