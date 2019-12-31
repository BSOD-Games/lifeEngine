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
#include "studiorender/igpuprogram.h"

#include "global.h"
#include "shaderdll.h"
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
	gpuProgram( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::BaseShader::~BaseShader()
{
	if ( gpuProgram )		g_studioRenderFactory->Delete( gpuProgram );
}

// ------------------------------------------------------------------------------------ //
// Загрузить шейдер
// ------------------------------------------------------------------------------------ //
bool le::BaseShader::LoadShader( const ShaderDescriptor& ShaderDescriptor )
{
	gpuProgram = ( IGPUProgram* ) g_studioRenderFactory->Create( GPUPROGRAM_INTERFACE_VERSION );
	if ( !gpuProgram ) return false;

	if ( !gpuProgram->Compile( ShaderDescriptor ) )
	{
		g_studioRenderFactory->Delete( gpuProgram );
		gpuProgram = nullptr;
		return false;
	}

	return true;
}
