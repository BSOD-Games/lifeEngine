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

// ------------------------------------------------------------------------------------ //
// Создать объект
// ------------------------------------------------------------------------------------ //
void* le::StudioRenderFactory::Create( const char* NameInterface )
{
	if ( strcmp( NameInterface, GPUPROGRAM_INTERFACE_VERSION ) == 0 )			return new GPUProgram();
	else if ( strcmp( NameInterface, TEXTURE_INTERFACE_VERSION ) == 0 )			return new Texture();

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