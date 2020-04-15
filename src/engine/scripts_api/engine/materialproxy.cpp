//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/materialmanager.h"
#include "engine/global.h"
#include "engine/ifactory.h"
#include "engine/imaterialproxy.h"
#include "engine/materialproxyvar.h"

namespace scripts_api
{
#include "materialproxy.h"
}

// ------------------------------------------------------------------------------------ //
// Update
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxy_Update( materialProxy_t Object )
{
	if ( !Object ) return;
	static_cast< le::IMaterialProxy* >( Object )->Update();
}

// ------------------------------------------------------------------------------------ //
// Nead update
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxy_NeadUpdate( materialProxy_t Object )
{
	if ( !Object ) return;
	static_cast< le::IMaterialProxy* >( Object )->NeadUpdate();
}

// ------------------------------------------------------------------------------------ //
// Clear var
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxy_ClearVar( materialProxy_t Object, const char* Name )
{
	if ( !Object ) return;
	static_cast< le::IMaterialProxy* >( Object )->ClearVar( Name );
}

// ------------------------------------------------------------------------------------ //
// Clear all vars
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxy_ClearAllVars( materialProxy_t Object )
{
	if ( !Object ) return;
	static_cast< le::IMaterialProxy* >( Object )->ClearAllVars();
}

// ------------------------------------------------------------------------------------ //
// Set var
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxy_SetVar( materialProxy_t Object, materialProxyVar_t MaterialProxyVar )
{
	if ( !Object ) return;
	static_cast< le::IMaterialProxy* >( Object )->SetVar( ( le::MaterialProxyVar* ) MaterialProxyVar );
}

// ------------------------------------------------------------------------------------ //
// Is nead update
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::MaterialProxy_IsNeadUpdate( materialProxy_t Object )
{
	if ( !Object ) return B_FALSE;
	return static_cast< le::IMaterialProxy* >( Object )->IsNeadUpdate() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* scripts_api::MaterialProxy_GetName( materialProxy_t Object )
{
	if ( !Object ) return nullptr;
	return static_cast< le::IMaterialProxy* >( Object )->GetName();
}

// ------------------------------------------------------------------------------------ //
// Get var
// ------------------------------------------------------------------------------------ //
scripts_api::materialProxyVar_t scripts_api::MaterialProxy_GetVar( materialProxy_t Object, const char* Name )
{
	if ( !Object ) return nullptr;
	return static_cast< le::IMaterialProxy* >( Object )->GetVar( Name );
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxy_Delete( materialProxy_t Object )
{
	if ( !Object ) return;

	le::IMaterialProxy*		object = static_cast< le::IMaterialProxy* >( Object );
	le::g_materialManager->RemoveProxy( object );

	if ( object->GetCountReferences() <= 1 )
		object->Release();
	else
		object->DecrementReference();

	object = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Create
// ------------------------------------------------------------------------------------ //
scripts_api::materialProxy_t scripts_api::MaterialProxy_Create( const char* Name )
{
	return le::g_materialManager->CreateProxy( Name );
}
