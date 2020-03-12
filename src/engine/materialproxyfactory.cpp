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

#include "engine/materialproxyfactory.h"
#include "engine/animatedtextureproxy.h"

// ------------------------------------------------------------------------------------ //
// Create proxy-material
// ------------------------------------------------------------------------------------ //
le::IMaterialProxy* le::MaterialProxyFactory::CreateProxy( const char* NameProxy )
{
    if ( strcmp( NameProxy, "AnimatedTexture" ) == 0 )              return new AnimatedTextureProxy();

    return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Delete proxy-material
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyFactory::DeleteProxy( IMaterialProxy* MaterialProxy )
{
    if ( !MaterialProxy ) return;
    delete MaterialProxy;
}
