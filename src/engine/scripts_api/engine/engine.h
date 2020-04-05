//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_ENGINE_H
#define API_ENGINE_H

// Engine
#include "material.h"
#include "materialproxy.h"
#include "materialproxyvar.h"
#include "model.h"

// StudioRender
#include "../studiorender/shaderparameter.h"
#include "../studiorender/studiorenderpass.h"
#include "../studiorender/studiorendertechnique.h"

//---------------------------------------------------------------------//

/* Create material */
extern material_t						Engine_CreateMaterial();

/* Create material proxy */
extern materialProxy_t					Engine_CreateMaterialProxy( const char* Name );

/* Create material proxy var */
extern materialProxyVar_t				Engine_CreateMaterialProxyVar();

/* Create shader parameter */
extern shaderParameter_t				Engine_CreateShaderParameter();

/* Create studiorender pass */
extern studioRenderPass_t				Engine_CreateStudioRenderPass();

/* Create studiorender technique */
extern studioRenderTechnique_t			Engine_CreateStudioRenderTechnique();

/* Create model */
extern model_t							Engine_CreateModel();

//---------------------------------------------------------------------//

#endif // API_ENGINE_H
