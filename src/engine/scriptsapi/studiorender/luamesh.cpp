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
#include "studiorender/imesh.h"
#include "scriptsapi/studiorender/luamesh.h"

// ------------------------------------------------------------------------------------ //
// Register mesh in LUA
// ------------------------------------------------------------------------------------ //
void le::LUAMesh::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Registern input system
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAMesh>( "Mesh" ).
		addConstructor< void (*)() >().
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAMesh::LUAMesh() :
	object( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAMesh::LUAMesh( IMesh* Object ) :
	object( Object )
{
	if ( object ) object->IncrementReference();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAMesh::LUAMesh( const LUAMesh& Copy ) :
	object( Copy.object )
{
	if ( object ) object->IncrementReference();
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::LUAMesh::~LUAMesh()
{
	if ( !object )			return;

	if ( object->GetCountReferences() == 0 )
		object->Release();
	else
		object->DecrementReference();
}