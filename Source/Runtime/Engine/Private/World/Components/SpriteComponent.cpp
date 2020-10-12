// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Misc/EngineGlobals.h"
#include "Rendering/RenderSystem.h"
#include "World/Components/SpriteComponent.h"

/**
 * Constructor
 */
le::SpriteComponent::SpriteComponent() :
	material( nullptr ),
	textureRect( 0.f, 0.f, 1.f, 1.f )
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
	GRenderSystem->DrawSprite( type, material, size, textureRect, GetGlobalPosition() );
}
