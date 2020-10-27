// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef UNLITGENERIC_H
#define UNLITGENERIC_H

#include "Misc/EngineDefines.h"
#include "Misc/EngineGlobals.h"
#include "Rendering/ShaderFactory.h"
#include "Rendering/BaseShader.h"

namespace le
{
	class Texture2D;
	class IRHIShader;

	class UnlitGeneric : public BaseShader
	{
	public:
		/* Constructor */
		UnlitGeneric();

		/* Destructor */
		~UnlitGeneric();

		/* Register shader */
		FORCEINLINE static void Register()				{ GShaderFactory->Register( "UnlitGeneric", []() -> FBaseShaderRef { return new UnlitGeneric(); } ); }

		/* Initialize shader */
		bool Initialize( const std::vector< ShaderVar >* InShaderVars = nullptr ) override;

		/* On draw sprite */
		void OnDrawSprite( IRHI* InRHI, const FVector2D& InSize, const FSRectFloat& InTextureRect, const FVector3D& InPosition, FCameraComponentConstRef& InCameraComponent ) override;

		/* Clear vars */
		void ClearVars();

		/* Get name */
		std::string GetName() const override;

		/* Get fallback shader */
		std::string GetFallbackShader() const override;

	private:
		uint32				flags;
		FTexture2DRef		baseTexture;
		FIRHIShaderRef		rhiShader;
	};
}

#endif // !UNLITGENERIC_H
