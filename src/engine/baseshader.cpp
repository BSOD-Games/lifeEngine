//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/ifactory.h"
#include "engine/resourcesystem.h"
#include "studiorender/igpuprogram.h"

#include "global.h"
#include "baseshader.h"

// ------------------------------------------------------------------------------------ //
// Получить количество параметров
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::BaseShader::GetCountParams() const
{
	return shaderParams.size();
}

// ------------------------------------------------------------------------------------ //
// Получить массив параметров
// ------------------------------------------------------------------------------------ //
le::ShaderParamInfo* le::BaseShader::GetParams() const
{
	return ( ShaderParamInfo* ) shaderParams.data();
}

// ------------------------------------------------------------------------------------ //
// Получить параметр
// ------------------------------------------------------------------------------------ //
le::ShaderParamInfo* le::BaseShader::GetParam( UInt32_t Index ) const
{
	if ( Index >= shaderParams.size() ) return nullptr;
	return ( ShaderParamInfo* ) &shaderParams[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::BaseShader::BaseShader() :
	gpuProgram( nullptr ),
	flagShader( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::BaseShader::~BaseShader()
{
	if ( gpuProgram )
	{
		gpuProgram->DecrementReference();
		
		if ( gpuProgram->GetCountReferences() <= 1 )
			g_resourceSystem->UnloadGPUProgram( nameShader.c_str(), flagShader );
	}
}

// ------------------------------------------------------------------------------------ //
// Загрузить шейдер
// ------------------------------------------------------------------------------------ //
bool le::BaseShader::LoadShader( const std::string& Name, const std::string& Path, const std::vector< const char* >& Defines, UInt32_t Flags )
{
	gpuProgram = g_resourceSystem->LoadGPUProgram( Name.c_str(), Path.c_str(), Flags, Defines.size(), ( const char** ) Defines.data() );
	if ( gpuProgram )		gpuProgram->IncrementReference();

	return gpuProgram;
}