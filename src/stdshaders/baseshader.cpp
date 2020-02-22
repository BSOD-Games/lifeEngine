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
le::BaseShader::BaseShader()
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::BaseShader::~BaseShader()
{
	for ( auto it = gpuPrograms.begin(), itEnd = gpuPrograms.end(); it != itEnd; ++it )
		g_studioRenderFactory->Delete( it->second );

	gpuPrograms.clear();	
}

// ------------------------------------------------------------------------------------ //
// Загрузить шейдер
// ------------------------------------------------------------------------------------ //
bool le::BaseShader::LoadShader( const ShaderDescriptor& ShaderDescriptor, const std::vector< const char* >& Defines, UInt32_t Flags )
{
	if ( gpuPrograms.find( Flags ) != gpuPrograms.end() )
		return true;

	IGPUProgram*			gpuProgram = ( IGPUProgram* ) g_studioRenderFactory->Create( GPUPROGRAM_INTERFACE_VERSION );
	if ( !gpuProgram ) return false;

	if ( !gpuProgram->Compile( ShaderDescriptor, Defines.size(), ( const char** ) Defines.data() ) )
	{
		g_studioRenderFactory->Delete( gpuProgram );
		gpuProgram = nullptr;
		return false;
	}

	gpuPrograms[ Flags ] = gpuProgram;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Get gpu program by shader flags
// ------------------------------------------------------------------------------------ //
le::IGPUProgram* le::BaseShader::GetGPUProgram( UInt32_t Flags ) const
{
	auto 		itGpuProgram = gpuPrograms.find( Flags );
	if ( itGpuProgram == gpuPrograms.end() )
		return nullptr;

	return itGpuProgram->second;
}