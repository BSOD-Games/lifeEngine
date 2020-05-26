//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>

#include "engine/iresourcesystem.h"

#include "global.h"
#include "gpuprogram.h"
#include "shader_lighting.h"

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ShaderLighting::ShaderLighting() :
	gpuProgram( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ShaderLighting::~ShaderLighting()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Создать шейдер
// ------------------------------------------------------------------------------------ //
bool le::ShaderLighting::Create()
{
	// Loading shader for point light
	std::vector< const char* >		defines = { "POINT_LIGHT" };
	GPUProgram* 		gpuProgram_pointLight = ( GPUProgram* ) g_resourceSystem->LoadGPUProgram( "ShaderLighting", "shaders/studiorender/lighting.shader", LT_POINT, defines.size(), defines.data() );
	if ( !gpuProgram_pointLight )		return false;

	gpuProgram_pointLight->IncrementReference();
	gpuProgram_pointLight->Bind();
	gpuProgram_pointLight->SetUniform( "albedoSpecular", 0 );
	gpuProgram_pointLight->SetUniform( "normalShininess", 1 );
	gpuProgram_pointLight->SetUniform( "emission", 2 );
	gpuProgram_pointLight->SetUniform( "depth", 3 );
	gpuProgram_pointLight->Unbind();

	// Loading shader for spot light
	defines = { "SPOT_LIGHT" };
	GPUProgram*			gpuProgram_spotLight = ( GPUProgram* ) g_resourceSystem->LoadGPUProgram( "ShaderLighting", "shaders/studiorender/lighting.shader", LT_SPOT, defines.size(), defines.data() );
	if ( !gpuProgram_spotLight )		return false;

	gpuProgram_spotLight->IncrementReference();
	gpuProgram_spotLight->Bind();
	gpuProgram_spotLight->SetUniform( "albedoSpecular", 0 );
	gpuProgram_spotLight->SetUniform( "normalShininess", 1 );
	gpuProgram_spotLight->SetUniform( "emission", 2 );
	gpuProgram_spotLight->SetUniform( "depth", 3 );
	gpuProgram_spotLight->Unbind();

	// Loading shader fpr directional light
	defines = { "DIRECTIONAL_LIGHT" };
	GPUProgram*			gpuProgram_directionalLight = ( GPUProgram* ) g_resourceSystem->LoadGPUProgram( "ShaderLighting", "shaders/studiorender/lighting.shader", LT_DIRECTIONAL, defines.size(), defines.data() );
	if ( !gpuProgram_directionalLight )		return false;

	gpuProgram_directionalLight->IncrementReference();
	gpuProgram_directionalLight->Bind();
	gpuProgram_directionalLight->SetUniform( "albedoSpecular", 0 );
	gpuProgram_directionalLight->SetUniform( "normalShininess", 1 );
	gpuProgram_directionalLight->SetUniform( "emission", 2 );
	gpuProgram_directionalLight->SetUniform( "depth", 3 );
	gpuProgram_directionalLight->Unbind();

	gpuProgram = gpuProgram_pointLight;
	gpuPrograms[ LT_POINT ] = gpuProgram_pointLight;
	gpuPrograms[ LT_SPOT ] = gpuProgram_spotLight;
	gpuPrograms[ LT_DIRECTIONAL ] = gpuProgram_directionalLight;

	return true;
}

// ------------------------------------------------------------------------------------ //
// Удалить шейдер
// ------------------------------------------------------------------------------------ //
void le::ShaderLighting::Delete()
{
	Unbind();

	for ( auto it = gpuPrograms.begin(), itEnd = gpuPrograms.end(); it != itEnd; ++it )
		if ( it->second->GetCountReferences() <= 1 )
			it->second->Release();
		else
			it->second->DecrementReference();

	g_resourceSystem->UnloadGPUProgram( "ShaderLighting", LT_POINT );
	g_resourceSystem->UnloadGPUProgram( "ShaderLighting", LT_SPOT );
	g_resourceSystem->UnloadGPUProgram( "ShaderLighting", LT_DIRECTIONAL );

	gpuPrograms.clear();
	gpuProgram = nullptr;
}
