// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Misc/EngineDefines.h"
#include "Misc/EngineGlobals.h"
#include "Logging/LogMacros.h"
#include "Rendering/RenderSystem.h"
#include "World/Components/SpriteComponent.h"

// Engine shaders
#include "Rendering/Shaders/UnlitGeneric.h"

/**
 * Constructor
 */
le::RenderSystem::RenderSystem() :
	currentCamera( nullptr )
{}

/**
 * Destructor
 */
le::RenderSystem::~RenderSystem()
{
	if ( currentCamera )			currentCamera->ReleaseRef();
}

/**
 * Initialize render system
 */
bool le::RenderSystem::Initialize()
{
	spriteRenderer.Initialize();
	
	UnlitGeneric::Register();
	return true;
}

/**
 * Start render scene
 */
void le::RenderSystem::Begin()
{
	LIFEENGINE_ASSERT( GRHI );
	GRHI->Clear( SColor( 0, 0, 0, 0 ), CT_Color );
}

/**
 * End render scene
 */
void le::RenderSystem::End()
{}

/**
 * Present scene
 */
void le::RenderSystem::Present( FRHIContext InRHIContext )
{
	LIFEENGINE_ASSERT( GRHI && InRHIContext );

	// Render sprites
	for ( auto it = sprites.begin(); it != sprites.end(); )
	{
		spriteRenderer.Render( *it, currentCamera );
		it = sprites.erase( it );
	}

	GRHI->SwapBuffers( InRHIContext );
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
	position = InRight.position;

	if ( material )		material->AddRef();
	return *this;
}
