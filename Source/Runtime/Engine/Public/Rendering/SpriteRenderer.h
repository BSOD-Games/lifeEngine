// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <list>

#include "Misc/EngineDefines.h"
#include "Misc/Object.h"

namespace le
{
	struct SSpriteRenderObject;
	class CameraComponent;
	class IRHIBuffer;
	class IRHIVertexFormat;
	class IRHIGeometry;

	class SpriteRenderer : public Object
	{
	public:
		/* Constructor */
		SpriteRenderer();

		/* Initialize renderer */
		bool Initialize();

		/* Render sprite */
		void Render( const SSpriteRenderObject& InSpriteRenderObject, CameraComponent* InCameraComponent );

		/* Is initialized */
		FORCEINLINE bool IsInitialized() const				{ return rhiGeometry && rhiVertexBuffer && rhiVertexFormat && rhiIndexBuffer; }

	private:
		IRHIGeometry*			rhiGeometry;
		IRHIBuffer*				rhiVertexBuffer;
		IRHIBuffer*				rhiIndexBuffer;
		IRHIVertexFormat*		rhiVertexFormat;
	};
}

#endif // !SPRITERENDERER_H
