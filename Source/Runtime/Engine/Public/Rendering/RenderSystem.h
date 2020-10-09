// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <list>

#include "System/Window.h"
#include "RHI/IRHI.h"
#include "Misc/Object.h"
#include "World/Components/SpriteComponent.h"
#include "World/Components/CameraComponent.h"
#include "Rendering/SpriteRenderer.h"

namespace le
{
	struct SSpriteRenderObject
	{
		/* Constructor */
		SSpriteRenderObject();
		SSpriteRenderObject( ESpriteType InSpriteType, Material* InMaterial, const FVector2D& InSize, const FVector3D& InPosition );

		/* Constructor of copy */
		SSpriteRenderObject( const SSpriteRenderObject& InCopy );

		/* Destructor */
		~SSpriteRenderObject();

		/* Operator = */
		SSpriteRenderObject& operator=( const SSpriteRenderObject& InRight );

		ESpriteType		type;
		Material*		material;
		FVector2D		size;
		FVector3D		position;
	};

	class RenderSystem : public Object
	{
	public:
		/* Constructor */
		RenderSystem();

		/* Destructor */
		~RenderSystem();

		/* Initialize render system */
		bool Initialize();

		/* Draw sprite */
		FORCEINLINE void DrawSprite( ESpriteType InSpriteType, Material* InMaterial, const FVector2D& InSize, const FVector3D& InPosition )
		{
			LIFEENGINE_ASSERT( InMaterial );
			sprites.push_back( SSpriteRenderObject( InSpriteType, InMaterial, InSize, InPosition ) );
		}

		/* Start render scene */
		void Begin();

		/* End render scene */
		void End();

		/* Present scent */
		void Present( FRHIContext InRHIContext );

		/* Set camera */
		FORCEINLINE void SetCamera( CameraComponent* InCameraComponent )
		{
			if ( currentCamera )		currentCamera->ReleaseRef();

			currentCamera = InCameraComponent;
			if ( InCameraComponent )	InCameraComponent->AddRef();
		}

	private:
		SpriteRenderer							spriteRenderer;
		CameraComponent*						currentCamera;

		std::list< SSpriteRenderObject >		sprites;
	};
}

#endif // !RENDERSYSTEM_H
