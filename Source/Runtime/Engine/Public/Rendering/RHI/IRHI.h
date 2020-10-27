// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef IRHI_H
#define IRHI_H

#include "Misc/Types.h"
#include "System/Window.h"
#include "Misc/Object.h"
#include "IRHIBuffer.h"
#include "IRHITexture2D.h"

namespace le
{
	typedef		void*	FRHIContext;
	struct SColor;

	enum EClearType
	{
		CT_Color = 1 << 0,
		CT_Depth = 1 << 1
	};

	enum EDrawOperation
	{
		DO_PointList,
		DO_LineList,
		DO_LineStrip,
		DO_TriangleList,
		DO_TriangleStrip,
		DO_TriangleFan
	};

	class IRHI : public Object
	{
	public:
		/* Destructor */
		virtual ~IRHI() {}

		/* Create render context */
		virtual FRHIContext CreateContext( FWindowHandle InWindowHandle, FRHIContext InShareContext = nullptr ) = 0;

		/* Create shader */
		virtual FIRHIShaderRef CreateShader() = 0;

		/* Create buffer */
		virtual FIRHIBufferRef CreateBuffer( EBufferType InBufferType, EUsageBufferType InUsageBufferType = UBT_Static ) = 0;

		/* Create vertex format */
		virtual FIRHIVertexFormatRef CreateVertexFormat() = 0;

		/* Create geometry */
		virtual FIRHIGeometryRef CreateGeometry() = 0;

		/* Create texture 2D */
		virtual FIRHITexture2DRef CreateTexture2D( EImageFormat InImageFormat, uint32 InWidth, uint32 InHeight, uint32 InCountMipmap = 1 ) = 0;

		/* Create render target */
		virtual FIRHIRenderTargetRef CreateRenderTarget( uint32 InWidth, uint32 InHeight ) = 0;

		/* Make current context */
		virtual bool MakeCurrentContext( FRHIContext InRHIContext ) = 0;

		/* Draw geometry */
		virtual void Draw( EDrawOperation InDrawOperation, uint32 InVertexOffset, uint32 InVertexCount, uint32 InInstaceCount = 1 ) = 0;

		/* Draw indexed geometry */
		virtual void DrawIndexed( EDrawOperation InDrawOperation, uint32 InStartIndex, uint32 InIndexCount, uint32 InVetexOffset, uint32 InVertexCount, uint32 InInstaceCount = 1 ) = 0;

		/* Clear */
		virtual void Clear( const SColor& InColor, uint32 InClearFlags = EClearType::CT_Color ) = 0;

		/* Delete context */
		virtual void DeleteContext( FRHIContext& InOutRHIContext ) = 0;

		/* Swap buffers */
		virtual void SwapBuffers( FRHIContext InRHIContext ) = 0;

		/* Set vertical sync */
		virtual void SetVerticalSync( bool InIsEnable = true ) = 0;

		/* Set viewport */
		virtual void SetViewport( uint32 InX, uint32 InY, uint32 InWidth, uint32 InHeight ) = 0;

		/* Set shader */
		virtual void SetShader( FIRHIShaderConstRef& InShader ) = 0;

		/* Unset shader */
		virtual void UnsetShader() = 0;

		/* Set geometry */
		virtual void SetGeometry( FIRHIGeometryConstRef& InGeometry ) = 0;

		/* Unset geometry */
		virtual void UnsetGeometry() = 0;

		/* Set texture 2D */
		virtual void SetTexture2D( FIRHITexture2DConstRef& InTexture2D, uint32 InTextureLayer = 0 ) = 0;

		/* Unset texture 2D */
		virtual void UnsetTexture2D( uint32 InTextureLayer = 0 ) = 0;

		/* Set render target */
		virtual void SetRenderTarget( FIRHIRenderTargetConstRef& InRenderTarget ) = 0;

		/* Unset render target */
		virtual void UnsetRenderTarget() = 0;
	};
}

#endif // !IRHI_H
