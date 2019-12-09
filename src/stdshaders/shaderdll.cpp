//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/lifeengine.h"
#include "engine/iengine.h"
#include "engine/iconsolesystem.h"

#include "shaderdll.h"
#include "testshader.h"

LIFEENGINE_STDSHADERS_API( le::ShaderDLL );

// ------------------------------------------------------------------------------------ //
// Инициализировать библиотеку шейдеров
// ------------------------------------------------------------------------------------ //
bool le::ShaderDLL::Initialize( IEngine* Engine )
{
	shaders.push_back( new TestShader() );

	return true;
}

// ------------------------------------------------------------------------------------ //
// Получить количество шейдеров
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ShaderDLL::GetShaderCount() const
{
	return shaders.size();
}

// ------------------------------------------------------------------------------------ //
// Получить шейдер
// ------------------------------------------------------------------------------------ //
le::IShader* le::ShaderDLL::GetShader( UInt32_t Index ) const
{
	if ( Index >= shaders.size() ) return nullptr;
	return shaders[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ShaderDLL::~ShaderDLL()
{
	for ( UInt32_t index = 0, count = shaders.size(); index < count; ++index )
		delete shaders[ index ];

	shaders.clear();
}