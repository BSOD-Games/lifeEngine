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
#include "pointlight.h"
#include "spotlight.h"
#include "directionallight.h"

// ------------------------------------------------------------------------------------ //
// Создать объект
// ------------------------------------------------------------------------------------ //
void* le::StudioRenderFactory::Create( const char* NameInterface )
{
	if ( strcmp( NameInterface, GPUPROGRAM_INTERFACE_VERSION ) == 0 )					return new GPUProgram();
	else if ( strcmp( NameInterface, TEXTURE_INTERFACE_VERSION ) == 0 )					return new Texture();
	else if ( strcmp( NameInterface, MESH_INTERFACE_VERSION ) == 0 )					return new Mesh();
	else if ( strcmp( NameInterface, POINTLIGHT_INTERFACE_VERSION ) == 0 )				return new PointLight();
	else if ( strcmp( NameInterface, SPOTLIGHT_INTERFACE_VERSION ) == 0 )				return new SpotLight();
	else if ( strcmp( NameInterface, DIRECTIONALLIGHT_INTERFACE_VERSION ) == 0 )		return new DirectionalLight();

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