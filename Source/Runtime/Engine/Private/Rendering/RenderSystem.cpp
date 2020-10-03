// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Misc/EngineDefines.h"
#include "Logging/LogMacros.h"
#include "Rendering/RenderSystem.h"
#include "World/Components/SpriteComponent.h"
#include "OpenGL4RHI.h"

// Engine shaders
#include "Rendering/Shaders/UnlitGeneric.h"

/**
 * Constructor
 */
le::RenderSystem::RenderSystem() :
	rhi( nullptr ),
	currentContext( nullptr ),
	currentCamera( nullptr )
{}

/**
 * Destructor
 */
le::RenderSystem::~RenderSystem()
{
	if ( currentCamera )			currentCamera->ReleaseRef();

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

	UnlitGeneric::Register();
	return true;
}

/**
 * Make current context
 */
bool le::RenderSystem::MakeCurrentContext( FRHIContext InRHIContext )
{
	if ( currentContext != InRHIContext && !rhi->MakeCurrentContext( InRHIContext ) )
		return false;

	if ( InRHIContext )
	{
		if ( !spriteRenderer.IsInitialized() )		spriteRenderer.Initialize();
	}
	
	currentContext = InRHIContext;
	return true;
}

/**
 * Start render scene
 */
void le::RenderSystem::Begin()
{
	rhi->Clear( SColor( 0, 0, 0, 0 ), CT_Color );
}

/**
 * End render scene
 */
void le::RenderSystem::End()
{}

/**
 * Present scene
 */
void le::RenderSystem::Present()
{
	if ( !currentContext ) return;
	
	// Render sprites
	for ( auto it = sprites.begin(); it != sprites.end(); )
	{
		spriteRenderer.Render( *it, currentCamera );
		it = sprites.erase( it );
	}

	rhi->SwapBuffers( currentContext );
}

/**
 * Constructor
 */
le::SSpriteRenderObject::SSpriteRenderObject() :
	type( ST_Static ),
	material( nullptr )
{}

/**
 * Constructor
 */
le::SSpriteRenderObject::SSpriteRenderObject( ESpriteType InSpriteType, Material* InMaterial, const FVector2D& InSize, const FVector3D& InPosition ) :
	type( InSpriteType ),
	material( InMaterial ),
	size( InSize ),
	position( InPosition )
{
	if ( material )			material->AddRef();
}

/**
 * Constructor of copy
 */
le::SSpriteRenderObject::SSpriteRenderObject( const SSpriteRenderObject& InCopy ) :
	type( InCopy.type ),
	material( InCopy.material ),
	size( InCopy.size ),
	position( InCopy.position )
{
	if ( material )			material->AddRef();
}

/**
 * Destructor
 */ 
le::SSpriteRenderObject::~SSpriteRenderObject()
{
	if ( material )			material->ReleaseRef();
}

/**
 * Operator =
 */
le::SSpriteRenderObject& le::SSpriteRenderObject::operator=( const SSpriteRenderObject& InRight )
{
	type = InRight.type;
	size = InRight.size;
	material = InRight.material;

	if ( material )		material->AddRef();
	return *this;
}
