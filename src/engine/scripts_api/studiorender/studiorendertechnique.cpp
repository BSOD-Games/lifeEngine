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
#include "studiorender/istudiorendertechnique.h"

namespace scripts_api
{
#include "studiorendertechnique.h"
}

#define STUDIORENDERTECHNIQUE( Object )  static_cast< le::IStudioRenderTechnique* >( Object )

// ------------------------------------------------------------------------------------ //
// Add pass
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderTechnique_AddPass( studioRenderTechnique_t Object, studioRenderPass_t Pass )
{
	if ( !Object ) return;
	STUDIORENDERTECHNIQUE( Object )->AddPass( ( le::IStudioRenderPass* ) Pass );
}

// ------------------------------------------------------------------------------------ //
// Remove pass
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderTechnique_RemovePass( studioRenderTechnique_t Object, uint32_t Index )
{
	if ( !Object ) return;
	STUDIORENDERTECHNIQUE( Object )->RemovePass( Index );
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderTechnique_Clear( studioRenderTechnique_t Object )
{
	if ( !Object ) return;
	STUDIORENDERTECHNIQUE( Object )->Clear();
}

// ------------------------------------------------------------------------------------ //
// Set type
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderTechnique_SetType( studioRenderTechnique_t Object, renderTechniqueType_t Technique )
{
	if ( !Object ) return;
	STUDIORENDERTECHNIQUE( Object )->SetType( ( le::RENDER_TECHNIQUE ) Technique );
}

// ------------------------------------------------------------------------------------ //
// Get type
// ------------------------------------------------------------------------------------ //
scripts_api::renderTechniqueType_t scripts_api::StudioRenderTechnique_GetType( studioRenderTechnique_t Object )
{
	if ( !Object ) return RT_UNKNOW;
	return ( renderTechniqueType_t ) STUDIORENDERTECHNIQUE( Object )->GetType();
}

// ------------------------------------------------------------------------------------ //
// Get count passes
// ------------------------------------------------------------------------------------ //
uint32_t scripts_api::StudioRenderTechnique_GetCountPasses( studioRenderTechnique_t Object )
{
	if ( !Object ) return 0;
	return STUDIORENDERTECHNIQUE( Object )->GetCountPasses();
}

// ------------------------------------------------------------------------------------ //
// Get passes
// ------------------------------------------------------------------------------------ //
scripts_api::studioRenderPass_t* scripts_api::StudioRenderTechnique_GetPasses( studioRenderTechnique_t Object )
{
	if ( !Object ) return nullptr;
	return ( studioRenderPass_t* ) STUDIORENDERTECHNIQUE( Object )->GetPasses();
}

// ------------------------------------------------------------------------------------ //
// Get pass
// ------------------------------------------------------------------------------------ //
scripts_api::studioRenderPass_t scripts_api::StudioRenderTechnique_GetPass( studioRenderTechnique_t Object, uint32_t Index )
{
	if ( !Object ) return nullptr;
	return ( studioRenderPass_t ) STUDIORENDERTECHNIQUE( Object )->GetPass( Index );
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void scripts_api::StudioRenderTechnique_Delete( studioRenderTechnique_t Object )
{
	if ( !Object ) return;

	le::IStudioRenderTechnique*			object = static_cast< le::IStudioRenderTechnique* >( Object );
	if ( object->GetCountReferences() <= 1 )
		object->Release();
	else
		object->DecrementReference();

	object = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Create
// ------------------------------------------------------------------------------------ //
scripts_api::studioRenderTechnique_t scripts_api::StudioRenderTechnique_Create()
{
	return le::g_studioRenderFactory->Create( TECHNIQUE_INTERFACE_VERSION );
}
