// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef BASESHADER_H
#define BASESHADER_H

#include <string>
#include <vector>

#include "Misc/Object.h"
#include "Misc/Types.h"
#include "Math/Math.h"
#include "ShaderVar.h"

namespace le
{
	class IRHI;
	class CameraComponent;

	class BaseShader : public Object
	{
	public:
		/* Constructor */
		BaseShader();

		/* Destructor */
		virtual ~BaseShader();

		/* Initialize shader */
		virtual bool Initialize( const std::vector< ShaderVar >* InShaderVars = nullptr ) = 0;

		/* On draw sprite */
		virtual void OnDrawSprite( IRHI* InRHI, const FVector2D& InSize, const FVector3D& InPosition, CameraComponent* InCameraComponent );

		/* Get name */
		virtual std::string GetName() const = 0;

		/* Get fallback shader */
		virtual std::string GetFallbackShader() const = 0;
	};
}

#endif // !BASESHADER_H
