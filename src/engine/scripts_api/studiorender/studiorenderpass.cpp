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
#include "studiorender/istudiorenderpass.h"

namespace scripts_api
{
#include "studiorenderpass.h"
}

#define STUDIORENDERPASS( Object )  static_cast< le::IStudioRenderPass* >( Object )

// ------------------------------------------------------------------------------------ //
// Add parameter
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderPass_AddParameter( studioRenderPass_t Object, shaderParameter_t Parameter )
{
	if ( !Object ) return;
	STUDIORENDERPASS( Object )->AddParameter( ( le::IShaderParameter* ) Parameter );
}

// ------------------------------------------------------------------------------------ //
// Remove parameter
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderPass_RemoveParameter( studioRenderPass_t Object, uint32_t Index )
{
	if ( !Object ) return;
	STUDIORENDERPASS( Object )->RemoveParameter( Index );
}

// ------------------------------------------------------------------------------------ //
// Add proxy
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderPass_AddProxy( studioRenderPass_t Object, materialProxy_t MaterialProxy )
{
	if ( !Object ) return;
	STUDIORENDERPASS( Object )->AddProxy( ( le::IMaterialProxy* ) MaterialProxy );
}

// ------------------------------------------------------------------------------------ //
// Remove proxy
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderPass_RemoveProxy( studioRenderPass_t Object, uint32_t Index )
{
	if ( !Object ) return;
	STUDIORENDERPASS( Object )->RemoveProxy( Index );
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderPass_Clear( studioRenderPass_t Object )
{
	if ( !Object ) return;
	STUDIORENDERPASS( Object )->Clear();
}

// ------------------------------------------------------------------------------------ //
// Enable depth test
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderPass_EnableDepthTest( studioRenderPass_t Object, bool_t Enable )
{
	if ( !Object ) return;
	STUDIORENDERPASS( Object )->EnableDepthTest( Enable == B_TRUE ? true : false );
}

// ------------------------------------------------------------------------------------ //
// Enable depth write
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderPass_EnableDepthWrite( studioRenderPass_t Object, bool_t Enable )
{
	if ( !Object ) return;
	STUDIORENDERPASS( Object )->EnableDepthWrite( Enable == B_TRUE ? true : false );
}

// ------------------------------------------------------------------------------------ //
// Enable blend
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderPass_EnableBlend( studioRenderPass_t Object, bool_t Enable )
{
	if ( !Object ) return;
	STUDIORENDERPASS( Object )->EnableBlend( Enable == B_TRUE ? true : false );
}

// ------------------------------------------------------------------------------------ //
// Enable cull face
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderPass_EnableCullFace( studioRenderPass_t Object, bool_t Enable )
{
	if ( !Object ) return;
	STUDIORENDERPASS( Object )->EnableCullFace( Enable == B_TRUE ? true : false );
}

// ------------------------------------------------------------------------------------ //
// Find parameter
// ------------------------------------------------------------------------------------ //
scripts_api::shaderParameter_t scripts_api::StudioRenderPass_FindParameter( studioRenderPass_t Object, const char* Name )
{
	if ( !Object ) return nullptr;
	return ( le::IShaderParameter* ) STUDIORENDERPASS( Object )->FindParameter( Name );
}

// ------------------------------------------------------------------------------------ //
// Set shader
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderPass_SetShader( studioRenderPass_t Object, const char* NameShader )
{
	if ( !Object ) return;
	STUDIORENDERPASS( Object )->SetShader( NameShader );
}

// ------------------------------------------------------------------------------------ //
// Set cull face type
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderPass_SetCullFaceType( studioRenderPass_t Object, cullFaceType_t CullFaceType )
{
	if ( !Object ) return;
	STUDIORENDERPASS( Object )->SetCullFaceType( ( le::CULLFACE_TYPE ) CullFaceType );
}

// ------------------------------------------------------------------------------------ //
// Is depth test
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::StudioRenderPass_IsDepthTest( studioRenderPass_t Object )
{
	if ( !Object ) return B_FALSE;
	return STUDIORENDERPASS( Object )->IsDepthTest() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Is depth write
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::StudioRenderPass_IsDepthWrite( studioRenderPass_t Object )
{
	if ( !Object ) return B_FALSE;
	return STUDIORENDERPASS( Object )->IsDepthWrite() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Is blend
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::StudioRenderPass_IsBlend( studioRenderPass_t Object )
{
	if ( !Object ) return B_FALSE;
	return STUDIORENDERPASS( Object )->IsBlend() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Is cull face
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::StudioRenderPass_IsCullFace( studioRenderPass_t Object )
{
	if ( !Object ) return B_FALSE;
	return STUDIORENDERPASS( Object )->IsCullFace() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Get cull face type
// ------------------------------------------------------------------------------------ //
scripts_api::cullFaceType_t scripts_api::StudioRenderPass_GetCullFaceType( studioRenderPass_t Object )
{
	if ( !Object ) return CT_BACK;
	return ( cullFaceType_t ) STUDIORENDERPASS( Object )->GetCullFaceType();
}

// ------------------------------------------------------------------------------------ //
// Get name shader
// ------------------------------------------------------------------------------------ //
const char* scripts_api::StudioRenderPass_GetNameShader( studioRenderPass_t Object )
{
	if ( !Object ) return nullptr;
	return STUDIORENDERPASS( Object )->GetNameShader();
}

// ------------------------------------------------------------------------------------ //
// Get count parameters
// ------------------------------------------------------------------------------------ //
uint32_t scripts_api::StudioRenderPass_GetCountParameters( studioRenderPass_t Object )
{
	if ( !Object ) return 0;
	return STUDIORENDERPASS( Object )->GetCountParameters();
}

// ------------------------------------------------------------------------------------ //
// Get shader parameters
// ------------------------------------------------------------------------------------ //
scripts_api::shaderParameter_t* scripts_api::StudioRenderPass_GetParameters( studioRenderPass_t Object )
{
	if ( !Object ) return nullptr;
	return ( shaderParameter_t* ) STUDIORENDERPASS( Object )->GetParameters();
}

// ------------------------------------------------------------------------------------ //
// Get shader parameter
// ------------------------------------------------------------------------------------ //
scripts_api::shaderParameter_t scripts_api::StudioRenderPass_GetParameter( studioRenderPass_t Object, uint32_t Index )
{
	if ( !Object ) return nullptr;
	return ( shaderParameter_t ) STUDIORENDERPASS( Object )->GetParameter( Index );
}

// ------------------------------------------------------------------------------------ //
// Get count proxes
// ------------------------------------------------------------------------------------ //
scripts_api::uint32_t scripts_api::StudioRenderPass_GetCountProxes( studioRenderPass_t Object )
{
	if ( !Object ) return 0;
	return STUDIORENDERPASS( Object )->GetCountProxes();
}

// ------------------------------------------------------------------------------------ //
// Clear proxes
// ------------------------------------------------------------------------------------ //
scripts_api::materialProxy_t* scripts_api::StudioRenderPass_GetProxes( studioRenderPass_t Object )
{
	if ( !Object ) return nullptr;
	return ( materialProxy_t* ) STUDIORENDERPASS( Object )->GetProxes();
}

// ------------------------------------------------------------------------------------ //
// Get proxy
// ------------------------------------------------------------------------------------ //
scripts_api::materialProxy_t scripts_api::StudioRenderPass_GetProxy( studioRenderPass_t Object, uint32_t Index )
{
	if ( !Object ) return nullptr;
	return ( materialProxy_t ) STUDIORENDERPASS( Object )->GetProxy( Index );
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderPass_Delete( studioRenderPass_t Object )
{
	if ( !Object ) return;

	le::IStudioRenderPass*			object = static_cast< le::IStudioRenderPass* >( Object );
	if ( object->GetCountReferences() <= 1 )
		object->Release();
	else
		object->DecrementReference();

	object = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Create
// ------------------------------------------------------------------------------------ //
scripts_api::studioRenderPass_t scripts_api::StudioRenderPass_Create()
{
	return le::g_studioRenderFactory->Create( PASS_INTERFACE_VERSION );
}
