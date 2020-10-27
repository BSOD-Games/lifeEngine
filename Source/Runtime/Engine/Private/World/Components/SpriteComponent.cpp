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
	textureRect( 0.f, 0.f, 1.f, 1.f ),
	transformComponent( new TransformComponent() )
{}

/**
 * Destructor
 */
le::SpriteComponent::~SpriteComponent()
{}

/**
 * Render sprite
 */
void le::SpriteComponent::Render() const
{
	GRenderSystem->DrawSprite( type, material, size, textureRect, transformComponent->GetGlobalPosition() );
}