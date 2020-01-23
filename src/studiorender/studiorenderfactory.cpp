//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "studiorenderfactory.h"
#include "gpuprogram.h"
#include "texture.h"
#include "mesh.h"
#include "studiorendertechnique.h"
#include "studiorenderpass.h"
#include "shaderparameter.h"

// ------------------------------------------------------------------------------------ //
// Создать объект
// ------------------------------------------------------------------------------------ //
void* le::StudioRenderFactory::Create( const char* NameInterface )
{
	if ( strcmp( NameInterface, GPUPROGRAM_INTERFACE_VERSION ) == 0 )			return new GPUProgram();
	else if ( strcmp( NameInterface, TEXTURE_INTERFACE_VERSION ) == 0 )			return new Texture();
	else if ( strcmp( NameInterface, MESH_INTERFACE_VERSION ) == 0 )			return new Mesh();
	else if ( strcmp( NameInterface, TECHNIQUE_INTERFACE_VERSION ) == 0 )		return new StudioRenderTechnique();
	else if ( strcmp( NameInterface, PASS_INTERFACE_VERSION ) == 0 )			return new StudioRenderPass();
	else if ( strcmp( NameInterface, SHADERPARAMETER_INTERFACE_VERSION ) == 0 )	return new ShaderParameter();

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Удалить объект
// ------------------------------------------------------------------------------------ //
void le::StudioRenderFactory::Delete( void* Object )
{
	if ( !Object ) return;
	delete Object;
}