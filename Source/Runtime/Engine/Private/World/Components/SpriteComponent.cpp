// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Misc/EngineGlobals.h"
#include "Rendering/RenderSystem.h"
#include "World/Components/SpriteComponent.h"

/**
 * Constructor
 */
le::SpriteComponent::SpriteComponent() :
	material( nullptr )
{}

/**
 * Destructor
 */
le::SpriteComponent::~SpriteComponent()
{
	if ( material )		material->ReleaseRef();
}

/**
 * Render sprite
 */
void le::SpriteComponent::Render()
{
	GRenderSystem->DrawSprite( type, material, size );
}
