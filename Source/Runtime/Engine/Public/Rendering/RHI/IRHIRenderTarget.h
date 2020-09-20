// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IRHIRENDERTARGET_H
#define IRHIRENDERTARGET_H

#include "Misc/Types.h"
#include "Misc/Object.h"
#include "Rendering/RHI/IRHITexture2D.h"

namespace le
{
	class IRHIRenderTarget : public Object
	{
	public:
		/* Destructor */
		virtual ~IRHIRenderTarget() {}

		/* Create texture 2D */
		virtual void CreateTexture2D( EImageFormat InImageFormat, const SSampler& InSampler ) = 0;

		/* Destroy texture 2D */
		virtual void DestroyTexture2D( uint32 InIndex ) = 0;

		/* Set size */
		virtual void SetSize( uint32 InWidth, uint32 InHeight ) = 0;

		/* Get width */
		virtual uint32 GetWidth() const = 0;

		/* Get height */
		virtual uint32 GetHeight() const = 0;

		/* Get texture 2D */
		virtual IRHITexture2D* GetTexture2D( uint32 InIndex ) const = 0;
	};
}

#endif // !IRHIRENDERTARGET_H
