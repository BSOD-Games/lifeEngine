// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Logging/LogMacros.h"
#include "Rendering/RenderSystem.h"
#include "OpenGL4RHI.h"

/**
 * Constructor
 */
le::RenderSystem::RenderSystem() :
	rhi( nullptr ),
	currentContext( nullptr )
{}

/**
 * Destructor
 */
le::RenderSystem::~RenderSystem()
{
	if ( rhi )
	{
		if ( currentContext )		rhi->DeleteContext( currentContext );
		delete rhi;
	}
}

/**
 * Initialize render system
 */
bool le::RenderSystem::Initialize()
{
	rhi = new OpenGL4RHI();
	return true;
}

/**
 * Make current context
 */
bool le::RenderSystem::MakeCurrentContext( FRHIContext InRHIContext )
{
	if ( currentContext == InRHIContext )
		return true;

	if ( rhi->MakeCurrentContext( InRHIContext ) )
	{
		currentContext = InRHIContext;
		return true;
	}

	return false;
}

/**
 * Start render scene
 */
void le::RenderSystem::Begin()
{}

/**
 * End render scene
 */
void le::RenderSystem::End()
{}

/**
 * Present scent
 */
void le::RenderSystem::Present()
{
	if ( !currentContext ) return;
	rhi->SwapBuffers( currentContext );
}