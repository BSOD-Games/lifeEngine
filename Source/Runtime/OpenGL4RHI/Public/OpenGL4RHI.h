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
		IRHIShader* CreateShader() override;

		/* Create buffer */
		IRHIBuffer* CreateBuffer( EBufferType InBufferType, EUsageBufferType InUsageBufferType = UBT_Static ) override;

		/* Create vertex format */
		IRHIVertexFormat* CreateVertexFormat() override;

		/* Create geometry */
		IRHIGeometry* CreateGeometry() override;

		/* Create texture 2D */
		IRHITexture2D* CreateTexture2D( EImageFormat InImageFormat, uint32 InWidth, uint32 InHeight, uint32 InCountMipmap = 1 ) override;

		/* Create render target */
		IRHIRenderTarget* CreateRenderTarget( uint32 InWidth, uint32 InHeight ) override;

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

		/* Delete shader */
		void DeleteShader( IRHIShader*& InOutShader ) override;

		/* Delete buffer */
		void DeleteBuffer( IRHIBuffer*& InOutBuffer ) override;

		/* Delete vertex format */
		void DeleteVertexFormat( IRHIVertexFormat*& InOutVertexFormat ) override;

		/* Delete geometry */
		void DeleteGeometry( IRHIGeometry*& InGeometry ) override;

		/* Delete texture 2D */
		void DeleteTexture2D( IRHITexture2D*& InTexture2D ) override;

		/* Delete render target */
		void DeleteRenderTarget( IRHIRenderTarget*& InRenderTarget ) override;

		/* Swap buffers */
		void SwapBuffers( FRHIContext InRHIContext ) override;

		/* Set vertical sync */
		void SetVerticalSync( bool InIsEnable = true ) override;

		/* Set viewport */
		void SetViewport( uint32 InX, uint32 InY, uint32 InWidth, uint32 InHeight ) override;

		/* Set shader */
		void SetShader( IRHIShader* InShader = nullptr ) override;

		/* Set geometry */
		void SetGeometry( IRHIGeometry* InGeometry ) override;

		/* Set texture 2D */
		void SetTexture2D( IRHITexture2D* InTexture2D, uint32 InTextureLayer = 0 ) override;

		/* Set render target */
		void SetRenderTarget( IRHIRenderTarget* InRenderTarget ) override;
	};
}

#endif // !OPENGL4RHI_H
