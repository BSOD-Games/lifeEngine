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
 * Create render context
 */
le::FRHIContext le::RenderSystem::CreateContext( FWindowHandle InWindowHandle, FRHIContext InShareContext )
{
	return rhi->CreateContext( InWindowHandle, InShareContext );
}

/**
 * Create texture 2D
 */
le::IRHITexture2D* le::RenderSystem::CreateTexture2D( EImageFormat InImageFormat, uint32 InWidth, uint32 InHeight, uint32 InCountMipmap )
{
	return rhi->CreateTexture2D( InImageFormat, InWidth, InHeight, InCountMipmap );
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
 * Delete context
 */
void le::RenderSystem::DeleteContext( FRHIContext InRHIContext )
{
	rhi->DeleteContext( InRHIContext );
}

/**
 * Delete texture 2D
 */
void le::RenderSystem::DeleteTexture2D( IRHITexture2D*& InTexture2D )
{
	rhi->DeleteTexture2D( InTexture2D );
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

/**
 * Set vertical sync
 */
void le::RenderSystem::SetVerticalSync( bool InIsEnable )
{
	rhi->SetVerticalSync( InIsEnable );
}

/**
 * Set viewport
 */
void le::RenderSystem::SetViewport( uint32 InX, uint32 InY, uint32 InWidth, uint32 InHeight )
{
	rhi->SetViewport( InX, InY, InWidth, InHeight );
}
