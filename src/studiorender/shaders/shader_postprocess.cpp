//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>

#include "engine/iresourcesystem.h"

#include "global.h"
#include "gpuprogram.h"
#include "shader_postprocess.h"

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ShaderPostprocess::ShaderPostprocess() :
	gpuProgram( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ShaderPostprocess::~ShaderPostprocess()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Создать шейдер
// ------------------------------------------------------------------------------------ //
bool le::ShaderPostprocess::Create()
{
	gpuProgram = ( GPUProgram* ) g_resourceSystem->LoadGPUProgram( "ShaderPostprocess", "shaders/studiorender/postprocess.shader" );
	if ( !gpuProgram )			return false;

	gpuProgram->IncrementReference();

	gpuProgram->Bind();
	gpuProgram->SetUniform( "albedoSpecular", 0 );
	gpuProgram->SetUniform( "emission", 1 );
	gpuProgram->SetUniform( "finalFrame", 2 );
	gpuProgram->Unbind();
	
	return true;
}

// ------------------------------------------------------------------------------------ //
// Удалить шейдер
// ------------------------------------------------------------------------------------ //
void le::ShaderPostprocess::Delete()
{
    Unbind();

    if ( gpuProgram->GetCountReferences() <= 1 )
        gpuProgram->Release();
    else
        gpuProgram->DecrementReference();

	g_resourceSystem->UnloadGPUProgram( "ShaderPostprocess" );
    gpuProgram = nullptr;
}
