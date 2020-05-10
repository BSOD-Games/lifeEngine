//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <exception>
#include <string>
#include <SDL2/SDL.h>

#include "common/gameinfo.h"
#include "engine/lifeengine.h"
#include "engine/iengine.h"
#include "engine/iconsolesystem.h"
#include "studiorender/ishader.h"

#include "global.h"
#include "shadermanager.h"
#include "lightmappedgeneric.h"
#include "unlitgeneric.h"
#include "spritegeneric.h"
#include "textgeneric.h"
#include "testshader.h"

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ShaderManager::ShaderManager()
{
	shaders.push_back( new LightmappedGeneric() );
	shaders.push_back( new UnlitGeneric() );
	shaders.push_back( new SpriteGeneric() );
	shaders.push_back( new TextGeneric() );
	shaders.push_back( new TestShader() );
}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ShaderManager::~ShaderManager()
{
}

// ------------------------------------------------------------------------------------ //
// Найти шейдер
// ------------------------------------------------------------------------------------ //
le::IShader* le::ShaderManager::FindShader( const char* Name ) const
{
    LIFEENGINE_ASSERT( Name );

	for ( UInt32_t index = 0, count = shaders.size(); index < count; ++index )
		if ( strcmp( shaders[ index ]->GetName(), Name ) == 0 )
			return shaders[ index ];

    // -------
    // Я люблю Source Engine <3
    // -------

    return nullptr;
}