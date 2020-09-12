// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IRHI_H
#define IRHI_H

#include "Misc/Types.h"
#include "System/Window.h"
#include "IRHIBuffer.h"

namespace le
{
	typedef void* FRHIContext;

	class IRHIShader;
	class IRHIVertexFormat;
	class IRHIGeometry;

	enum EDrawOperation
	{
		DO_PointList,
		DO_LineList,
		DO_LineStrip,
		DO_TriangleList,
		DO_TriangleStrip,
		DO_TriangleFan
	};

	class IRHI
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

		/* Make current context */
		virtual bool MakeCurrentContext( FRHIContext InRHIContext ) = 0;

		/* Draw geometry */
		virtual void Draw( EDrawOperation InDrawOperation, uint32 InVertexOffset, uint32 InVertexCount, uint32 InInstaceCount = 1 ) = 0;

		/* Draw indexed geometry */
		virtual void DrawIndexed( EDrawOperation InDrawOperation, uint32 InStartIndex, uint32 InIndexCount, uint32 InVetexOffset, uint32 InVertexCount, uint32 InInstaceCount = 1 ) = 0;

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
	};
}

#endif // !IRHI_H
