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
#include "engine/model.h"
#include "scriptsapi/mathlib/luavector3d.h"
#include "scriptsapi/mathlib/luaquaternion.h"
#include "scriptsapi/studiorender/luamesh.h"
#include "scriptsapi/engine/luamodel.h"

// ------------------------------------------------------------------------------------ //
// Register model in LUA
// ------------------------------------------------------------------------------------ //
void le::LUAModel::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Registern input system
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAModel>( "Model" ).
		addConstructor< void (*)() >().
		addFunction( "Move", &LUAModel::Move ).
		addFunction( "Rotate", ( void ( LUAModel::* ) ( const LUAVector3D& ) ) &LUAModel::Rotate ).
		//addFunction( "Rotate", ( void ( LUAModel::* ) ( const LUAQuaternion& ) ) &LUAModel::Rotate ).
		addFunction( "Scale", &LUAModel::Scale ).
		addFunction( "SetPosition", &LUAModel::SetPosition ).
		addFunction( "SetRotation", ( void ( LUAModel::* )( const LUAVector3D& ) ) &LUAModel::SetRotation ).
		addFunction( "SetRotation", ( void ( LUAModel::* )( const LUAQuaternion& ) ) &LUAModel::SetRotation ).
		addFunction( "SetScale", &LUAModel::SetScale ).
		addFunction( "SetMesh", &LUAModel::SetMesh ).
		addFunction( "SetMin", &LUAModel::SetMin ).
		addFunction( "SetMax", &LUAModel::SetMax ).
		addFunction( "SetStartFace", &LUAModel::SetStartFace ).
		addFunction( "SetCountFace", &LUAModel::SetCountFace ).
		addFunction( "GetPosition", &LUAModel::GetPosition ).
		addFunction( "GetScale", &LUAModel::GetScale ).
		addFunction( "GetMesh", &LUAModel::GetMesh ).
		addFunction( "GetMin", &LUAModel::GetMin ).
		addFunction( "GetMax", &LUAModel::GetMax ).
		addFunction( "GetStartFace", &LUAModel::GetStartFace ).
		addFunction( "GetCountFace", &LUAModel::GetCountFace ).
		addFunction( "GetRotation", &LUAModel::GetRotation ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAModel::LUAModel() :
	object( new Model() )
{
	object->IncrementReference();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAModel::LUAModel( IModel* Model ) :
	object( Model )
{
	if ( object ) object->IncrementReference();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAModel::LUAModel( const LUAModel& Copy ) :
	object( Copy.object )
{	
	if ( object ) object->IncrementReference();
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::LUAModel::~LUAModel()
{
	if ( !object )		return;

	if ( object->GetCountReferences() <= 1 )
		object->Release();
	else
		object->DecrementReference();
}

// ------------------------------------------------------------------------------------ //
// Move
// ------------------------------------------------------------------------------------ //
void le::LUAModel::Move( const LUAVector3D& FactorMove )
{
	object->Move( FactorMove.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Rotate
// ------------------------------------------------------------------------------------ //
void le::LUAModel::Rotate( const LUAVector3D& FactorRotate )
{
	object->Rotate( FactorRotate.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Rotate
// ------------------------------------------------------------------------------------ //
void le::LUAModel::Rotate( const LUAQuaternion& FactorRotate )
{
	object->Rotate( FactorRotate.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Scale
// ------------------------------------------------------------------------------------ //
void le::LUAModel::Scale( const LUAVector3D& FactorScale )
{
	object->Scale( FactorScale.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set position
// ------------------------------------------------------------------------------------ //
void le::LUAModel::SetPosition( const LUAVector3D& Position )
{
	object->SetPosition( Position.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set rotation
// ------------------------------------------------------------------------------------ //
void le::LUAModel::SetRotation( const LUAVector3D& Rotation )
{
	object->SetRotation( Rotation.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set rotation
// ------------------------------------------------------------------------------------ //
void le::LUAModel::SetRotation( const LUAQuaternion& Rotation )
{
	object->SetRotation( Rotation.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set scale
// ------------------------------------------------------------------------------------ //
void le::LUAModel::SetScale( const LUAVector3D& Scale )
{
	object->SetScale( Scale.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set mesh
// ------------------------------------------------------------------------------------ //
void le::LUAModel::SetMesh( const LUAMesh& Mesh )
{
	object->SetMesh( Mesh.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set min
// ------------------------------------------------------------------------------------ //
void le::LUAModel::SetMin( const LUAVector3D& MinPosition )
{
	object->SetMin( MinPosition.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set max
// ------------------------------------------------------------------------------------ //
void le::LUAModel::SetMax( const LUAVector3D& MaxPosition )
{
	object->SetMax( MaxPosition.GetHandle() );
}

// ------------------------------------------------------------------------------------ //
// Set start face
// ------------------------------------------------------------------------------------ //
void le::LUAModel::SetStartFace( UInt32_t StartFace )
{
	object->SetStartFace( StartFace );
}

// ------------------------------------------------------------------------------------ //
// Set count face
// ------------------------------------------------------------------------------------ //
void le::LUAModel::SetCountFace( UInt32_t CountFace )
{
	object->SetCountFace( CountFace );
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUAModel::GetPosition() const
{
	return LUAVector3D( object->GetPosition() );
}

// ------------------------------------------------------------------------------------ //
// Get rotation
// ------------------------------------------------------------------------------------ //
le::LUAQuaternion le::LUAModel::GetRotation() const
{
	return LUAQuaternion( object->GetRotation() );
}

// ------------------------------------------------------------------------------------ //
// Get scale
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUAModel::GetScale() const
{
	return LUAVector3D( object->GetScale() );
}

// ------------------------------------------------------------------------------------ //
// Get mesh
// ------------------------------------------------------------------------------------ //
le::LUAMesh le::LUAModel::GetMesh()
{
	return LUAMesh( object->GetMesh() );
}

// ------------------------------------------------------------------------------------ //
// Get min
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUAModel::GetMin()
{
	return LUAVector3D( object->GetMin() );
}

// ------------------------------------------------------------------------------------ //
// Get max
// ------------------------------------------------------------------------------------ //
le::LUAVector3D le::LUAModel::GetMax()
{
	return LUAVector3D( object->GetMax() );
}

// ------------------------------------------------------------------------------------ //
// Get start face
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::LUAModel::GetStartFace() const
{
	return object->GetStartFace();
}

// ------------------------------------------------------------------------------------ //
// Get count face
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::LUAModel::GetCountFace() const
{
	return object->GetCountFace();
}
