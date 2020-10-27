// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef OPENGL4RHI_H
#define OPENGL4RHI_H

#include "Rendering/RHI/IRHI.h"

namespace le
{
	class RHIGeometry;

	class OpenGL4RHI : public IRHI
	{
	public:
		/* Constructor */
		OpenGL4RHI();

		/* Destructor */
		~OpenGL4RHI();

		/* Create render context */
		FRHIContext CreateContext( FWindowHandle InWindowHandle, FRHIContext InShareContext = nullptr ) override;

		/* Create shader */
		FIRHIShaderRef CreateShader() override;

		/* Create buffer */
		FIRHIBufferRef CreateBuffer( EBufferType InBufferType, EUsageBufferType InUsageBufferType = UBT_Static ) override;

		/* Create vertex format */
		FIRHIVertexFormatRef CreateVertexFormat() override;

		/* Create geometry */
		FIRHIGeometryRef CreateGeometry() override;

		/* Create texture 2D */
		FIRHITexture2DRef CreateTexture2D( EImageFormat InImageFormat, uint32 InWidth, uint32 InHeight, uint32 InCountMipmap = 1 ) override;

		/* Create render target */
		FIRHIRenderTargetRef CreateRenderTarget( uint32 InWidth, uint32 InHeight ) override;

		/* Make current context */
		bool MakeCurrentContext( FRHIContext InRHIContext ) override;

		/* Draw geometry */
		void Draw( EDrawOperation InDrawOperation, uint32 InVertexOffset, uint32 InVertexCount, uint32 InInstaceCount = 1 );

		/* Draw indexed geometry */
		void DrawIndexed( EDrawOperation InDrawOperation, uint32 InStartIndex, uint32 InIndexCount, uint32 InVetexOffset, uint32 InVertexCount, uint32 InInstaceCount = 1 );

		/* Clear */
		void Clear( const SColor& InColor, uint32 InClearFlags = EClearType::CT_Color ) override;

		/* Delete context */
		void DeleteContext( FRHIContext& InRHIContext ) override;

		/* Swap buffers */
		void SwapBuffers( FRHIContext InRHIContext ) override;

		/* Set vertical sync */
		void SetVerticalSync( bool InIsEnable = true ) override;

		/* Set viewport */
		void SetViewport( uint32 InX, uint32 InY, uint32 InWidth, uint32 InHeight ) override;

		/* Set shader */
		void SetShader( FIRHIShaderConstRef& InShader )override;

		/* Unset shader */
		void UnsetShader() override;

		/* Set geometry */
		void SetGeometry( FIRHIGeometryConstRef& InGeometry ) override;

		/* Unset geometry */
		void UnsetGeometry() override;

		/* Set texture 2D */
		void SetTexture2D( FIRHITexture2DConstRef& InTexture2D, uint32 InTextureLayer = 0 ) override;

		/* Unset texture 2D */
		void UnsetTexture2D( uint32 InTextureLayer = 0 ) override;

		/* Set render target */
		void SetRenderTarget( FIRHIRenderTargetConstRef& InRenderTarget ) override;

		/* Unset render target */
		void UnsetRenderTarget() override;
	};
}

#endif // !OPENGL4RHI_H
