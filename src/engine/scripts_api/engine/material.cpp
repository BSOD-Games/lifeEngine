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
#include "studiorender/istudiorendertechnique.h"

namespace scripts_api
{
#include "material.h"
}

#define MATERIAL( Object )  static_cast< le::Material* >( Object )

// ------------------------------------------------------------------------------------ //
// Add technique
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_AddTechnique( material_t Object, studioRenderTechnique_t Tecnhique )
{
	if ( !Object ) return;
	MATERIAL( Object )->AddTechnique( ( le::IStudioRenderTechnique* ) Tecnhique );
}

// ------------------------------------------------------------------------------------ //
// Remove technique
// ------------------------------------------------------------------------------------ //
void scripts_api::Material_RemoveTechnique( material_t Object, uint32_t Index )
{
	if ( !Object ) return;
	MATERIAL( Object )->RemoveTechnique( Index );
}

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
	MATERIAL( Object )->GetSurfaceName();
}

// ------------------------------------------------------------------------------------ //
// Get count techniques
// ------------------------------------------------------------------------------------ //
scripts_api::uint32_t scripts_api::Material_GetCountTechniques( material_t Object )
{
	if ( !Object ) return 0;
	return MATERIAL( Object )->GetCountTechiques();
}

// ------------------------------------------------------------------------------------ //
// Get techniques
// ------------------------------------------------------------------------------------ //
scripts_api::studioRenderTechnique_t* scripts_api::Material_GetTechniques( material_t Object )
{
	if ( !Object ) return nullptr;
	return ( studioRenderTechnique_t* ) MATERIAL( Object )->GetTechiques();
}

// ------------------------------------------------------------------------------------ //
// Get technique by index
// ------------------------------------------------------------------------------------ //
scripts_api::studioRenderTechnique_t scripts_api::Material_GetTechniqueByIndex( material_t Object, uint32_t Index )
{
	if ( !Object ) return nullptr;
	return ( studioRenderTechnique_t ) MATERIAL( Object )->GetTechnique( Index );
}

// ------------------------------------------------------------------------------------ //
// Get technique by type
// ------------------------------------------------------------------------------------ //
scripts_api::studioRenderTechnique_t scripts_api::Material_GetTechniqueByType( material_t Object, renderTechniqueType_t Type )
{
	if ( !Object ) return nullptr;
	return ( studioRenderTechnique_t ) MATERIAL( Object )->GetTechnique( Type );
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
