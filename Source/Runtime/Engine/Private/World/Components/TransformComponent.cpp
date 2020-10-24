// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "World/Components/TransformComponent.h"

/**
 * Constructor
 */
le::TransformComponent::TransformComponent() :
	isNeedUpdate( false ),
	position( 0.f, 0.f, 0.f ),
	rotation( 1.f, 0.f, 0.f, 0.f ),
	scale( 1.f, 1.f, 1.f ),
	transformation( 1.f ),
	owner( nullptr )
{}

/**
 * Destructor
 */
le::TransformComponent::~TransformComponent()
{
	if ( owner )
	{
		owner->GetEventChannelUpdate().Unsubscribe( this, &TransformComponent::OnOwnerUpdate );
		owner->ReleaseRef();
	}
}

/**
 * Update matrix
 */
void le::TransformComponent::UpdateMatrix() const
{
	transformation = glm::translate( position ) * glm::mat4_cast( rotation ) * glm::scale( scale );
	isNeedUpdate = false;
}

/**
 * Event: Owner transformation is updated
 */
void le::TransformComponent::OnOwnerUpdate( const SEventUpdate& InEventUpdate )
{
	isNeedUpdate = true;
	eventUpdate.Emit( InEventUpdate );
}
