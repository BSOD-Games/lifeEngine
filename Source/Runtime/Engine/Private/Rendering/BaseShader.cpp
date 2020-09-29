// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

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
void le::BaseShader::OnDrawSprite( IRHI* InRHI, const SVector2D& InSize )
{
	static bool			isPrintedWarning = false;
	if ( !isPrintedWarning )
	{
		LIFEENGINE_LOG_WARNING( "Engine", "BaseShader::OnDrawSprite not implemented for shader [%s]", GetName().c_str() );
		isPrintedWarning = true;
	}
}