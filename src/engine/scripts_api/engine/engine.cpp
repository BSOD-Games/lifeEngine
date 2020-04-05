//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/ifactory.h"
#include "engine/global.h"
#include "engine/material.h"
#include "engine/imaterialproxy.h"
#include "engine/materialproxyvar.h"
#include "engine/model.h"
#include "studiorender/ishaderparameter.h"
#include "studiorender/istudiorenderpass.h"
#include "studiorender/istudiorendertechnique.h"

namespace scripts_api
{
#include "engine.h"
}

// ------------------------------------------------------------------------------------ //
// Create material
// ------------------------------------------------------------------------------------ //
scripts_api::material_t scripts_api::Engine_CreateMaterial()
{
	le::Material*		material = ( le::Material* ) le::g_engineFactory->Create( MATERIAL_INTERFACE_VERSION );
	material->IncrementReference();

	return material;
}

// ------------------------------------------------------------------------------------ //
// Create material proxy
// ------------------------------------------------------------------------------------ //
scripts_api::materialProxy_t scripts_api::Engine_CreateMaterialProxy( const char* Name )
{
	le::IMaterialProxy*		materialProxy = ( le::IMaterialProxy* ) le::g_engineFactory->Create( Name );
	materialProxy->IncrementReference();

	return materialProxy;
}

// ------------------------------------------------------------------------------------ //
// Create material proxy var
// ------------------------------------------------------------------------------------ //
scripts_api::materialProxyVar_t scripts_api::Engine_CreateMaterialProxyVar()
{
	le::MaterialProxyVar*		materialProxyVar = ( le::MaterialProxyVar* ) le::g_engineFactory->Create( MATERIALPROXYVAR_INTERFACE_VERSION );
	materialProxyVar->IncrementReference();

	return materialProxyVar;
}

// ------------------------------------------------------------------------------------ //
// Create shader parameter
// ------------------------------------------------------------------------------------ //
scripts_api::shaderParameter_t scripts_api::Engine_CreateShaderParameter()
{
	le::IShaderParameter*			shaderParameter = ( le::IShaderParameter* ) le::g_studioRenderFactory->Create( SHADERPARAMETER_INTERFACE_VERSION );
	shaderParameter->IncrementReference();

	return shaderParameter;
}

// ------------------------------------------------------------------------------------ //
// Create studiorender pass
// ------------------------------------------------------------------------------------ //
scripts_api::studioRenderPass_t scripts_api::Engine_CreateStudioRenderPass()
{
	le::IStudioRenderPass*			pass = ( le::IStudioRenderPass* ) le::g_studioRenderFactory->Create( PASS_INTERFACE_VERSION );
	pass->IncrementReference();

	return pass;
}

// ------------------------------------------------------------------------------------ //
// Create studiorender technique
// ------------------------------------------------------------------------------------ //
scripts_api::studioRenderTechnique_t scripts_api::Engine_CreateStudioRenderTechnique()
{
	le::IStudioRenderTechnique*		techniqut = ( le::IStudioRenderTechnique* ) le::g_studioRenderFactory->Create( TECHNIQUE_INTERFACE_VERSION );
	techniqut->IncrementReference();

	return techniqut;
}

// ------------------------------------------------------------------------------------ //
// Create model
// ------------------------------------------------------------------------------------ //
scripts_api::model_t scripts_api::Engine_CreateModel()
{
	le::Model*			model = ( le::Model* ) le::g_engineFactory->Create( MODEL_INTERFACE_VERSION );
	model->IncrementReference();

	return model;
}
