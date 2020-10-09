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
	typedef void* FRHIContext;

	class IRHIShader;
	class IRHIVertexFormat;
	class IRHIGeometry;
	class IRHIRenderTarget;
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
		virtual IRHIShader* CreateShader() = 0;

		/* Create buffer */
		virtual IRHIBuffer* CreateBuffer( EBufferType InBufferType, EUsageBufferType InUsageBufferType = UBT_Static ) = 0;

		/* Create vertex format */
		virtual IRHIVertexFormat* CreateVertexFormat() = 0;

		/* Create geometry */
		virtual IRHIGeometry* CreateGeometry() = 0;

		/* Create texture 2D */
		virtual IRHITexture2D* CreateTexture2D( EImageFormat InImageFormat, uint32 InWidth, uint32 InHeight, uint32 InCountMipmap = 1 ) = 0;

		/* Create render target */
		virtual IRHIRenderTarget* CreateRenderTarget( uint32 InWidth, uint32 InHeight ) = 0;

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

		/* Delete shader */
		virtual void DeleteShader( IRHIShader*& InOutShader ) = 0;

		/* Delete buffer */
		virtual void DeleteBuffer( IRHIBuffer*& InOutBuffer ) = 0;

		/* Delete vertex format */
		virtual void DeleteVertexFormat( IRHIVertexFormat*& InOutVertexFormat ) = 0;

		/* Delete geometry */
		virtual void DeleteGeometry( IRHIGeometry*& InGeometry ) = 0;

		/* Delete texture 2D */
		virtual void DeleteTexture2D( IRHITexture2D*& InTexture2D ) = 0;

		/* Delete render target */
		virtual void DeleteRenderTarget( IRHIRenderTarget*& InRenderTarget ) = 0;

		/* Swap buffers */
		virtual void SwapBuffers( FRHIContext InRHIContext ) = 0;

		/* Set vertical sync */
		virtual void SetVerticalSync( bool InIsEnable = true ) = 0;

		/* Set viewport */
		virtual void SetViewport( uint32 InX, uint32 InY, uint32 InWidth, uint32 InHeight ) = 0;

		/* Set shader */
		virtual void SetShader( IRHIShader* InShader = nullptr ) = 0;

		/* Set geometry */
		virtual void SetGeometry( IRHIGeometry* InGeometry ) = 0;

		/* Set texture 2D */
		virtual void SetTexture2D( IRHITexture2D* InTexture2D, uint32 InTextureLayer = 0 ) = 0;

		/* Set render target */
		virtual void SetRenderTarget( IRHIRenderTarget* InRenderTarget ) = 0;
	};
}

#endif // !IRHI_H
