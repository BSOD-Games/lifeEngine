//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/iresourcesystem.h"

#include "global.h"
#include "shader_debugprimitives.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::ShaderDebugPrimitives::ShaderDebugPrimitives() :
	gpuProgram( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::ShaderDebugPrimitives::~ShaderDebugPrimitives()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Create
// ------------------------------------------------------------------------------------ //
bool le::ShaderDebugPrimitives::Create()
{
	gpuProgram = ( GPUProgram* ) g_resourceSystem->LoadGPUProgram( "DebugPrimitives", "shaders/studiorender/debugprimitives.shader" );
	if ( !gpuProgram )		return false;

	gpuProgram->IncrementReference();
	return true;
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
void le::ShaderDebugPrimitives::Delete()
{
	if ( !gpuProgram )	return;
	Unbind();

	if ( gpuProgram->GetCountReferences() <= 1 )		gpuProgram->Release();
	else												gpuProgram->DecrementReference();

	g_resourceSystem->UnloadGPUProgram( "DebugPrimitives" );
	gpuProgram = nullptr;
}