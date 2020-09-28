// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <list>

#include "System/Window.h"
#include "RHI/IRHI.h"
#include "Misc/Object.h"
#include "World/Components/SpriteComponent.h"
#include "Rendering/SpriteRenderer.h"

namespace le
{
	class SpriteComponent;

	struct SSpriteRenderObject
	{
		/* Constructor */
		SSpriteRenderObject();
		SSpriteRenderObject( ESpriteType InSpriteType, Material* InMaterial, const SVector2D& InSize );

		/* Constructor of copy */
		SSpriteRenderObject( const SSpriteRenderObject& InCopy );

		/* Destructor */
		~SSpriteRenderObject();

		/* Operator = */
		SSpriteRenderObject& operator=( const SSpriteRenderObject& InRight );

		ESpriteType		type;
		Material*		material;
		SVector2D		size;
	};

	class RenderSystem : public Object
	{
	public:
		friend class SpriteRenderer;

		/* Constructor */
		RenderSystem();

		/* Destructor */
		~RenderSystem();

		/* Initialize render system */
		bool Initialize();

		/* Create render context */
		FORCEINLINE FRHIContext CreateContext( FWindowHandle InWindowHandle, FRHIContext InShareContext = nullptr )								{ return rhi->CreateContext( InWindowHandle, InShareContext ); }

		/* Create shader */
		FORCEINLINE IRHIShader* CreateShader()									{ return rhi->CreateShader(); }

		/* Create texture 2D */
		FORCEINLINE IRHITexture2D* CreateTexture2D( EImageFormat InImageFormat, uint32 InWidth, uint32 InHeight, uint32 InCountMipmap = 1 )		{ return rhi->CreateTexture2D( InImageFormat, InWidth, InHeight, InCountMipmap ); }

		/* Make current context */
		bool MakeCurrentContext( FRHIContext InRHIContext );

		/* Delete context */
		FORCEINLINE void DeleteContext( FRHIContext InRHIContext )				{ rhi->DeleteContext( InRHIContext ); }

		/* Delete shader */
		FORCEINLINE void DeleteShader( IRHIShader*& InShader )					{ rhi->DeleteShader( InShader ); }

		/* Delete texture 2D */
		FORCEINLINE void DeleteTexture2D( IRHITexture2D*& InTexture2D )			{ rhi->DeleteTexture2D( InTexture2D ); }

		/* Draw sprite */
		FORCEINLINE void DrawSprite( ESpriteType InSpriteType, Material* InMaterial, const SVector2D& InSize )
		{
			LIFEENGINE_ASSERT( InMaterial );
			sprites.push_back( SSpriteRenderObject( InSpriteType, InMaterial, InSize ) );
		}

		/* Start render scene */
		void Begin();

		/* End render scene */
		void End();

		/* Present scent */
		void Present();

		/* Set vertical sync */
		FORCEINLINE void SetVerticalSync( bool InIsEnable = true )				{ rhi->SetVerticalSync( InIsEnable ); }

		/* Set viewport */
		FORCEINLINE void SetViewport( uint32 InX, uint32 InY, uint32 InWidth, uint32 InHeight )			{ rhi->SetViewport( InX, InY, InWidth, InHeight ); }

	private:
		IRHI*									rhi;
		FRHIContext								currentContext;
		SpriteRenderer							spriteRenderer;

		std::list< SSpriteRenderObject >		sprites;
	};
}

#endif // !RENDERSYSTEM_H
