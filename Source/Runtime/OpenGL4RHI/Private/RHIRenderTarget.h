// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef RHIRENDERTARGET_H
#define RHIRENDERTARGET_H

#include <vector>

#include "Misc/EngineDefines.h"
#include "Rendering/RHI/IRHIRenderTarget.h"
#include "GLState.h"
#include "Types.h"

namespace le
{
	class RHITexture2D;

	class RHIRenderTarget : public IRHIRenderTarget
	{
	public:
		/* Constructor */
		RHIRenderTarget();
		RHIRenderTarget( uint32 InWidth, uint32 InHeight );

		/* Destructor */
		~RHIRenderTarget();

		/* Bind */
		FORCEINLINE void Bind()
		{
			if ( !handle )	return;
			GLState::BindFBO( handle );
		}

		/* Unbind */
		FORCEINLINE static void Unbind()
		{
			GLState::BindFBO( 0 );
		}

		/* Create texture 2D */
		void CreateTexture2D( EImageFormat InImageFormat, const SSampler& InSampler ) override;

		/* Destroy texture 2D */
		void DestroyTexture2D( uint32 InIndex ) override;

		/* Set size */
		void SetSize( uint32 InWidth, uint32 InHeight ) override;

		/* Get width */
		uint32 GetWidth() const override;

		/* Get height */
		uint32 GetHeight() const override;

		/* Get texture 2D */
		FIRHITexture2DRef GetTexture2D( uint32 InIndex ) const override;

	private:
		/* Destroy */
		void Destroy();

		uint32								handle;
		uint32								width;
		uint32								height;
		std::vector< FRHITexture2DRef >		buffers;
	};
}

#endif // !RHIRENDERTARGET_H
