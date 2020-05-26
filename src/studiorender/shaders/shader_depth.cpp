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

#include "engine/iresourcesystem.h"

#include "global.h"
#include "gpuprogram.h"
#include "shader_depth.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::ShaderDepth::ShaderDepth() :
	gpuProgram( nullptr ),
	activeType( GT_SPHERE )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::ShaderDepth::~ShaderDepth()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Create shader
// ------------------------------------------------------------------------------------ //
bool le::ShaderDepth::Create()
{
	// Loading shader write in buffer depth for geometry "Sphere"
	std::vector< const char* >		defines = { "SPHERE" };
	GPUProgram*         gpuProgram_sphere = ( GPUProgram* ) g_resourceSystem->LoadGPUProgram( "ShaderDepth", "shaders/studiorender/depth.shader", GT_SPHERE, defines.size(), defines.data() );
	if ( !gpuProgram_sphere )		return false;

	gpuProgram_sphere->IncrementReference();

	// Loading shader write in buffer depth for geometry "Cone"
	defines = { "CONE" };
	GPUProgram*         gpuProgram_cone = ( GPUProgram* ) g_resourceSystem->LoadGPUProgram( "ShaderDepth", "shaders/studiorender/depth.shader", GT_CONE, defines.size(), defines.data() );
	if ( !gpuProgram_cone )		return false;

	gpuProgram_cone->IncrementReference();

	// Loading shader write in buffer depth for geometry "Unknown"
	GPUProgram*         gpuProgram_unknown = ( GPUProgram* ) g_resourceSystem->LoadGPUProgram( "ShaderDepth", "shaders/studiorender/depth.shader" );
	if ( !gpuProgram_unknown )		return false;
	
	gpuProgram_unknown->IncrementReference();

	gpuProgram = gpuProgram_sphere;
	activeType = GT_SPHERE;

	gpuPrograms[ GT_SPHERE ] = gpuProgram_sphere;
	gpuPrograms[ GT_CONE ] = gpuProgram_cone;
	gpuPrograms[ GT_UNKNOWN ] = gpuProgram_unknown;

	return true;
}

// ------------------------------------------------------------------------------------ //
// Delete shader
// ------------------------------------------------------------------------------------ //
void le::ShaderDepth::Delete()
{
	Unbind();

	for ( auto it = gpuPrograms.begin(), itEnd = gpuPrograms.end(); it != itEnd; ++it )
		if ( it->second->GetCountReferences() <= 1 )
			it->second->Release();
		else
			it->second->DecrementReference();

	g_resourceSystem->UnloadGPUProgram( "ShaderDepth", GT_SPHERE );
	g_resourceSystem->UnloadGPUProgram( "ShaderDepth", GT_CONE );
	g_resourceSystem->UnloadGPUProgram( "ShaderDepth", GT_UNKNOWN );

	activeType = GT_SPHERE;
	gpuProgram = nullptr;
	gpuPrograms.clear();
}
