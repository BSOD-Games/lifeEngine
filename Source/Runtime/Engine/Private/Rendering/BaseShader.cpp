// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Logging/LogMacros.h"
#include "Rendering/BaseShader.h"

/**
 * Constructor
 */
le::BaseShader::BaseShader()
{}

/**
 * Destructor
 */
le::BaseShader::~BaseShader()
{}

/**
 * On draw sprite
 */
void le::BaseShader::OnDrawSprite( IRHI* InRHI, const FVector2D& InSize, const FVector3D& InPosition, CameraComponent* InCameraComponent )
{
	static bool			isPrintedWarning = false;
	if ( !isPrintedWarning )
	{
		LIFEENGINE_LOG_WARNING( "Engine", "BaseShader::OnDrawSprite not implemented for shader [%s]", GetName().c_str() );
		isPrintedWarning = true;
	}
}