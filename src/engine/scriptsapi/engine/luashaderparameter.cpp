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
#include "engine/shaderparameter.h"
#include "scriptsapi/common/luacolor.h"
#include "scriptsapi/mathlib/luavector2d.h"
#include "scriptsapi/mathlib/luavector3d.h"
#include "scriptsapi/mathlib/luavector4d.h"
#include "scriptsapi/engine/luashaderparameter.h"

// ------------------------------------------------------------------------------------ //
// Register shader parameter in LUA
// ------------------------------------------------------------------------------------ //
void le::LUAShaderParameter::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Registern shader parameter
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAShaderParameter>( "ShaderParameter" ).
		addConstructor< void (*)() >().
		addFunction( "Clear", &LUAShaderParameter::Clear ).
		addFunction( "SetName", &LUAShaderParameter::SetName ).
		addFunction( "SetValueInt", &LUAShaderParameter::SetValueInt ).
		addFunction( "SetValueFloat", &LUAShaderParameter::SetValueFloat ).
		addFunction( "SetValueShaderFlag", &LUAShaderParameter::SetValueShaderFlag ).
		addFunction( "SetValueColor", &LUAShaderParameter::SetValueColor ).
		addFunction( "SetValueVector2D", &LUAShaderParameter::SetValueVector2D ).
		addFunction( "SetValueVector3D", &LUAShaderParameter::SetValueVector3D ).
		addFunction( "SetValueVector4D", &LUAShaderParameter::SetValueVector4D ).
		addFunction( "IsDefined", &LUAShaderParameter::IsDefined ).
		addFunction( "GetName", &LUAShaderParameter::GetName ).
		addFunction( "GetType", &LUAShaderParameter::GetType ).
		addFunction( "GetValueInt", &LUAShaderParameter::GetValueInt ).
		addFunction( "GetValueFloat", &LUAShaderParameter::GetValueFloat ).
		addFunction( "GetValueShaderFlag", &LUAShaderParameter::GetValueShaderFlag ).
		addFunction( "GetValueColor", &LUAShaderParameter::GetValueColor ).
		addFunction( "GetValueVector2D", &LUAShaderParameter::GetValueVector2D ).
		addFunction( "GetValueVector3D", &LUAShaderParameter::GetValueVector3D ).
		addFunction( "GetValueVector4D", &LUAShaderParameter::GetValueVector4D ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAShaderParameter::LUAShaderParameter() :
	object( new ShaderParameter() )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::LUAShaderParameter::~LUAShaderParameter()
{
	if ( object->GetCountReferences() == 0 )
		object->Release();
	else
		object->DecrementReference();
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void le::LUAShaderParameter::Clear()
{
	object->Clear();
}

// ------------------------------------------------------------------------------------ //
// Set name
// ------------------------------------------------------------------------------------ //
void le::LUAShaderParameter::SetName( const char* Name )
{
	object->SetName( Name );
}

// ------------------------------------------------------------------------------------ //
// Set value int
// ------------------------------------------------------------------------------------ //
void le::LUAShaderParameter::SetValueInt( int Value )
{
	object->SetValueInt( Value );
}

// ------------------------------------------------------------------------------------ //
// Set value float
// ------------------------------------------------------------------------------------ //
void le::LUAShaderParameter::SetValueFloat( float Value )
{
	object->SetValueFloat( Value );
}

// ------------------------------------------------------------------------------------ //
// Set value shader flag
// ------------------------------------------------------------------------------------ //
void le::LUAShaderParameter::SetValueShaderFlag( bool Value )
{
	object->SetValueShaderFlag( Value );
}

// ------------------------------------------------------------------------------------ //
// Set value vector 2d
// ------------------------------------------------------------------------------------ //
void le::LUAShaderParameter::SetValueVector2D( const LUAVector2D& Value )
{
	object->SetValueVector2D( Value.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set value vector 3d
// ------------------------------------------------------------------------------------ //
void le::LUAShaderParameter::SetValueVector3D( const LUAVector3D& Value )
{
	object->SetValueVector3D( Value.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set value vector 4d
// ------------------------------------------------------------------------------------ //
void le::LUAShaderParameter::SetValueVector4D( const LUAVector4D& Value )
{
	object->SetValueVector4D( Value.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set value color
// ------------------------------------------------------------------------------------ //
void le::LUAShaderParameter::SetValueColor( const LUAColor& Value )
{
	object->SetValueColor( Value.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Is defined
// ------------------------------------------------------------------------------------ //
bool le::LUAShaderParameter::IsDefined() const
{
	return object->IsDefined();
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::LUAShaderParameter::GetName() const
{
	return object->GetName();
}

// ------------------------------------------------------------------------------------ //
// Get type
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::LUAShaderParameter::GetType() const
{
	return object->GetType();
}

// ------------------------------------------------------------------------------------ //
// Get value int
// ------------------------------------------------------------------------------------ //
int le::LUAShaderParameter::GetValueInt() const
{
	return object->GetValueInt();
}

// ------------------------------------------------------------------------------------ //
// Get value float
// ------------------------------------------------------------------------------------ //
float le::LUAShaderParameter::GetValueFloat() const
{
	return object->GetValueFloat();
}

// ------------------------------------------------------------------------------------ //
// Get value shader flag
// ------------------------------------------------------------------------------------ //
bool le::LUAShaderParameter::GetValueShaderFlag() const
{
	return object->GetValueShaderFlag();
}

// ------------------------------------------------------------------------------------ //
// Get value vector 2d
// ------------------------------------------------------------------------------------ //
le::LUAVector2D le::LUAShaderParameter::GetValueVector2D() const
{
	return LUAVector2D( object->GetValueVector2D() );
}

// ------------------------------------------------------------------------------------ //
// Get value vector 3d
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUAShaderParameter::GetValueVector3D() const
{
	return LUAVector3D( object->GetValueVector3D() );
}

// ------------------------------------------------------------------------------------ //
// Get value vector 4d
// ------------------------------------------------------------------------------------ //
le::LUAVector4D le::LUAShaderParameter::GetValueVector4D() const
{
	return LUAVector4D( object->GetValueVector4D() );
}

// ------------------------------------------------------------------------------------ //
// Get value color
// ------------------------------------------------------------------------------------ //
le::LUAColor le::LUAShaderParameter::GetValueColor() const
{
	return LUAColor( object->GetValueColor() );
}
