//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/global.h"
#include "engine/ifactory.h"
#include "engine/material.h"

namespace scripts_api
{
#include "material.h"
}

#define MATERIAL( Object )  static_cast< le::Material* >( Object )

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_Clear( material_t Object )
{
	if ( !Object ) return;
	MATERIAL( Object )->Clear();
}

// ------------------------------------------------------------------------------------ //
// Set surface name
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_SetSurfaceName( material_t Object, const char* Name )
{
	if ( !Object ) return;
	MATERIAL( Object )->SetSurfaceName( Name );
}

// ------------------------------------------------------------------------------------ //
// Get surface name
// ------------------------------------------------------------------------------------ //
const char* scripts_api::Material_GetSurfaceName( material_t Object )
{
	if ( !Object ) return nullptr;
	return MATERIAL( Object )->GetSurfaceName();
}

// ------------------------------------------------------------------------------------ //
// Delete material
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_Delete( material_t Object )
{
	if ( !Object ) return;

	le::Material*		material = static_cast< le::Material* >( Object );
	if ( material->GetCountReferences() <= 1 )
		material->Release();
	else
		material->DecrementReference();

	Object = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Create
// ------------------------------------------------------------------------------------ //
scripts_api::material_t scripts_api::Material_Create()
{
	return le::g_engineFactory->Create( MATERIAL_INTERFACE_VERSION );
}

// ------------------------------------------------------------------------------------ //
// Add parameter
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_AddParameter( material_t Object, shaderParameter_t Parameter )
{
	if ( !Object ) return;
	MATERIAL( Object )->AddParameter( ( le::IShaderParameter* ) Parameter );
}

// ------------------------------------------------------------------------------------ //
// Remove parameter
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_RemoveParameter( material_t Object, uint32_t Index )
{
	if ( !Object ) return;
	MATERIAL( Object )->RemoveParameter( Index );
}

// ------------------------------------------------------------------------------------ //
// Add proxy
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_AddProxy( material_t Object, materialProxy_t MaterialProxy )
{
	if ( !Object ) return;
	MATERIAL( Object )->AddProxy( ( le::IMaterialProxy* ) MaterialProxy );
}

// ------------------------------------------------------------------------------------ //
// Remove proxy
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_RemoveProxy( material_t Object, uint32_t Index )
{
	if ( !Object ) return;
	MATERIAL( Object )->RemoveProxy( Index );
}

// ------------------------------------------------------------------------------------ //
// Enable depth test
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_EnableDepthTest( material_t Object, bool_t Enable )
{
	if ( !Object ) return;
	MATERIAL( Object )->EnableDepthTest( Enable == B_TRUE ? true : false );
}

// ------------------------------------------------------------------------------------ //
// Enable depth write
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_EnableDepthWrite( material_t Object, bool_t Enable )
{
	if ( !Object ) return;
	MATERIAL( Object )->EnableDepthWrite( Enable == B_TRUE ? true : false );
}

// ------------------------------------------------------------------------------------ //
// Enable blend
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_EnableBlend( material_t Object, bool_t Enable )
{
	if ( !Object ) return;
	MATERIAL( Object )->EnableBlend( Enable == B_TRUE ? true : false );
}

// ------------------------------------------------------------------------------------ //
// Enable cull face
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_EnableCullFace( material_t Object, bool_t Enable )
{
	if ( !Object ) return;
	MATERIAL( Object )->EnableCullFace( Enable == B_TRUE ? true : false );
}

// ------------------------------------------------------------------------------------ //
// Find parameter
// ------------------------------------------------------------------------------------ //
scripts_api::shaderParameter_t scripts_api::Material_FindParameter( material_t Object, const char * Name )
{
	if ( !Object ) return nullptr;
	return ( le::IShaderParameter* ) MATERIAL( Object )->FindParameter( Name );
}

// ------------------------------------------------------------------------------------ //
// Set shader
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_SetShader( material_t Object, const char * NameShader )
{
	if ( !Object ) return;
	MATERIAL( Object )->SetShader( NameShader );
}

// ------------------------------------------------------------------------------------ //
// Set cull face type
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_SetCullFaceType( material_t Object, cullFaceType_t CullFaceType )
{
	if ( !Object ) return;
	MATERIAL( Object )->SetCullFaceType( ( le::CULLFACE_TYPE ) CullFaceType );
}

// ------------------------------------------------------------------------------------ //
// Is depth test
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::Material_IsDepthTest( material_t Object )
{
	if ( !Object ) return B_FALSE;
	return MATERIAL( Object )->IsDepthTest() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Is depth write
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::Material_IsDepthWrite( material_t Object )
{
	if ( !Object ) return B_FALSE;
	return MATERIAL( Object )->IsDepthWrite() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Is blend
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::Material_IsBlend( material_t Object )
{
	if ( !Object ) return B_FALSE;
	return MATERIAL( Object )->IsBlend() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Is cull face
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::Material_IsCullFace( material_t Object )
{
	if ( !Object ) return B_FALSE;
	return MATERIAL( Object )->IsCullFace() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Get cull face type
// ------------------------------------------------------------------------------------ //
scripts_api::cullFaceType_t scripts_api::Material_GetCullFaceType( material_t Object )
{
	if ( !Object ) return CT_BACK;
	return ( cullFaceType_t ) MATERIAL( Object )->GetCullFaceType();
}

// ------------------------------------------------------------------------------------ //
// Get count parameters
// ------------------------------------------------------------------------------------ //
scripts_api::uint32_t scripts_api::Material_GetCountParameters( material_t Object )
{
	if ( !Object ) return 0;
	return MATERIAL( Object )->GetCountParameters();
}

// ------------------------------------------------------------------------------------ //
// Get parameters
// ------------------------------------------------------------------------------------ //
scripts_api::shaderParameter_t* scripts_api::Material_GetParameters( material_t Object )
{
	if ( !Object ) return nullptr;
	return ( shaderParameter_t* ) MATERIAL( Object )->GetParameters();
}

// ------------------------------------------------------------------------------------ //
// Get parameter
// ------------------------------------------------------------------------------------ //
scripts_api::shaderParameter_t scripts_api::Material_GetParameter( material_t Object, uint32_t Index )
{
	if ( !Object ) return nullptr;
	return ( shaderParameter_t ) MATERIAL( Object )->GetParameter( Index );
}

// ------------------------------------------------------------------------------------ //
// Get count proxes
// ------------------------------------------------------------------------------------ //
scripts_api::uint32_t scripts_api::Material_GetCountProxes( material_t Object )
{
	if ( !Object ) return 0;
	return MATERIAL( Object )->GetCountProxes();
}

// ------------------------------------------------------------------------------------ //
// Get proxes
// ------------------------------------------------------------------------------------ //
scripts_api::materialProxy_t* scripts_api::Material_GetProxes( material_t Object )
{
	if ( !Object ) return nullptr;
	return ( materialProxy_t* ) MATERIAL( Object )->GetProxes();
}

// ------------------------------------------------------------------------------------ //
// Get proxy
// ------------------------------------------------------------------------------------ //
scripts_api::materialProxy_t scripts_api::Material_GetProxy( material_t Object, uint32_t Index )
{
	if ( !Object ) return nullptr;
	return ( materialProxy_t ) MATERIAL( Object )->GetProxy( Index );
}
