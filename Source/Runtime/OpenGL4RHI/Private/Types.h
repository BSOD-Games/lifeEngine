// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef RHITYPES_H
#define RHITYPES_H

#include "Misc/RefCountPtr.h"

namespace le
{
	class RHIShader;
	class RHIBuffer;
	class RHIVertexFormat;
	class RHIGeometry;
	class RHITexture2D;
	class RHIRenderTarget;

	typedef TRefCountPtr< RHIShader >					FRHIShaderRef;
	typedef const TRefCountPtr< RHIShader >				FRHIShaderConstRef;
	typedef TRefCountPtr< RHIBuffer >					FRHIBufferRef;
	typedef const TRefCountPtr< RHIBuffer >				FRHIBufferConstRef;
	typedef TRefCountPtr< RHIVertexFormat >				FRHIVertexFormatRef;
	typedef const TRefCountPtr< RHIVertexFormat >		FRHIVertexFormatConstRef;
	typedef TRefCountPtr< RHIGeometry >					FRHIGeometryRef;
	typedef const TRefCountPtr< RHIGeometry >			FRHIGeometryConstRef;
	typedef TRefCountPtr< RHITexture2D >				FRHITexture2DRef;
	typedef const TRefCountPtr< RHITexture2D >			FRHITexture2DConstRef;
	typedef TRefCountPtr< RHIRenderTarget >				FRHIRenderTargetRef;
	typedef const TRefCountPtr< RHIRenderTarget >		FRHIRenderTargetConstRef;
}

#endif // !RHITYPES_H
