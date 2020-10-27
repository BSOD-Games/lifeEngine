// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <list>

#include "Misc/EngineDefines.h"
#include "Misc/Object.h"
#include "Misc/Types.h"

#include "Rendering/RHI/IRHIGeometry.h"
#include "Rendering/RHI/IRHIBuffer.h"
#include "Rendering/RHI/IRHIVertexFormat.h"

namespace le
{
	struct SSpriteRenderObject;

	class SpriteRenderer : public Object
	{
	public:
		/* Constructor */
		SpriteRenderer();

		/* Initialize renderer */
		bool Initialize();

		/* Render sprite */
		void Render( const SSpriteRenderObject& InSpriteRenderObject, FCameraComponentConstRef& InCameraComponent );

		/* Is initialized */
		FORCEINLINE bool IsInitialized() const				{ return rhiGeometry && rhiVertexBuffer && rhiVertexFormat && rhiIndexBuffer; }

	private:
		FIRHIGeometryRef			rhiGeometry;
		FIRHIBufferRef				rhiVertexBuffer;
		FIRHIBufferRef				rhiIndexBuffer;
		FIRHIVertexFormatRef		rhiVertexFormat;
	};
}

#endif // !SPRITERENDERER_H
