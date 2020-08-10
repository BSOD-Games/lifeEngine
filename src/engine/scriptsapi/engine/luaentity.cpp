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
#include "scriptsapi/mathlib/luavector3d.h"
#include "scriptsapi/engine/luacamera.h"
#include "scriptsapi/engine/luamodel.h"
#include "scriptsapi/engine/lualevel.h"
#include "scriptsapi/engine/luaentity.h"
#include "scriptsapi/studiorender/luastudiorender.h"

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::LUAEntity::IncrementReference()
{
	++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::LUAEntity::DecrementReference()
{
	--countReferences;
}

// ------------------------------------------------------------------------------------ //
// Release
// ------------------------------------------------------------------------------------ //
void le::LUAEntity::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get ñount references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::LUAEntity::GetCountReferences() const
{
	return countReferences;
}

// ------------------------------------------------------------------------------------ //
// Key value
// ------------------------------------------------------------------------------------ //
void le::LUAEntity::KeyValue( const char* Key, const char* Value )
{
	( *object )[ "KeyValue" ]( Key, Value );
}

// ------------------------------------------------------------------------------------ //
// Update
// ------------------------------------------------------------------------------------ //
void le::LUAEntity::Update()
{
	( *object )[ "Update" ]();
}

// ------------------------------------------------------------------------------------ //
// Render
// ------------------------------------------------------------------------------------ //
void le::LUAEntity::Render( IStudioRender* StudioRender )
{
	( *object )[ "Render" ]();
}

// ------------------------------------------------------------------------------------ //
// Set model
// ------------------------------------------------------------------------------------ //
void le::LUAEntity::SetModel( IModel* Model, IBody* Body )
{
	( *object )[ "SetModel" ]( LUAModel( Model ) );
}

// ------------------------------------------------------------------------------------ //
// Set position
// ------------------------------------------------------------------------------------ //
void le::LUAEntity::SetPosition( const Vector3D_t& Position )
{
	//( *object )[ "SetPosition" ]( Position );
}

// ------------------------------------------------------------------------------------ //
// Set level
// ------------------------------------------------------------------------------------ //
void le::LUAEntity::SetLevel( ILevel* Level )
{
	( *object )[ "SetLevel" ]( LUALevel( Level ) );
}

// ------------------------------------------------------------------------------------ //
// Is visible
// ------------------------------------------------------------------------------------ //
bool le::LUAEntity::IsVisible( ICamera* Camera ) const
{
	return true;// ( *object )[ "IsVisible" ]( LUACamera( Camera ) );
}

// ------------------------------------------------------------------------------------ //
// Get center
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::LUAEntity::GetCenter() const
{
	return Vector3D_t( /*( *object )[ "GetCenter" ]().cast<LUAVector3D>().GetHandle()*/ );
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::LUAEntity::GetPosition() const
{
	return q;
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAEntity::LUAEntity( luabridge::LuaRef& Object ) :
	countReferences( 0 ),
	object( nullptr )
{
	object = new luabridge::LuaRef( Object() );
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAEntity::LUAEntity( const LUAEntity& Copy ) :
	countReferences( 0 ),
	object( luabridge::LuaRef( *Copy.object ) )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::LUAEntity::~LUAEntity()
{
	if ( !object )			return;
	//delete object;
}
