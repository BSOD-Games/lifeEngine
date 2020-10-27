// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include <GL/glew.h>

#include "OpenGL4RHI.h"
#include "RHIShader.h"
#include "RHIBuffer.h"
#include "RHIVertexFormat.h"
#include "RHIGeometry.h"
#include "RHITexture2D.h"
#include "RHIRenderTarget.h"

// Windows Context
#ifdef PLATFORM_WINDOWS
	#include "Platform/Windows/WGLContext.h"
#else
	#error Unsupported platform
#endif

/**
 * EDrawOperation to OpenGL draw operation
 */
FORCEINLINE le::uint32 Convert_EDrawOperationToGLDrawOperation( le::EDrawOperation InDrawOperation )
{
	switch ( InDrawOperation )
	{
	case le::DO_PointList:			return GL_POINTS;
	case le::DO_LineList:			return GL_LINES;
	case le::DO_LineStrip:			return GL_LINE_STRIP;
	case le::DO_TriangleStrip:		return GL_TRIANGLE_STRIP;
	case le::DO_TriangleFan:		return GL_TRIANGLE_FAN;

	default:
	case le::DO_TriangleList:		return GL_TRIANGLES;
	}
}

/**
 * Get index sizeof by EIndecesType
 */
FORCEINLINE le::uint32 GetIndexSizeof( le::uint32 InIndecesType )
{
	switch ( InIndecesType )
	{
	case GL_UNSIGNED_BYTE:		return sizeof( le::byte );
	case GL_UNSIGNED_SHORT:		return sizeof( le::uint16 );
	case GL_UNSIGNED_INT:		return sizeof( le::uint32 );
	default:					return 0;
	}
}

/**
 * Get clear flags from EClearType to OpenGL Clear Mask
 */
FORCEINLINE le::uint32 GetClearFlags( le::uint32 InClearFlags )
{
	le::uint32		clearFlags = 0;
	if ( InClearFlags & le::CT_Color )		clearFlags |= GL_COLOR_BUFFER_BIT;
	if ( InClearFlags & le::CT_Depth )		clearFlags |= GL_DEPTH_BUFFER_BIT;

	return clearFlags;
}

/**
 * Constructor
 */
le::OpenGL4RHI::OpenGL4RHI()
{}

/**
 * Destructor
 */
le::OpenGL4RHI::~OpenGL4RHI()
{}

/**
 * Create render context
 */
le::FRHIContext le::OpenGL4RHI::CreateContext( FWindowHandle InWindowHandle, FRHIContext InShareContext )
{
	SSettingsContext		settingsContext;
	settingsContext.majorVersion = 4;
	settingsContext.minorVersion = 5;
	settingsContext.depthBits = 24;
	settingsContext.stencilBits = 8;

#ifdef LIFEENGINE_DEBUG
	settingsContext.attributeFlags = SSettingsContext::CA_Core | SSettingsContext::CA_Debug;
#else
	settingsContext.attributeFlags = SSettingsContext::CA_Core;
#endif // LIFEENGINE_DEBUG

#ifdef PLATFORM_WINDOWS
	return WinGL_CreateContext( InWindowHandle, settingsContext, InShareContext );
#else
	return nullptr;
#endif // PLATFORM_WINDOWS	
}

/**
 * Create shader
 */
le::FIRHIShaderRef le::OpenGL4RHI::CreateShader()
{
	return FIRHIShaderRef( new RHIShader() );
}

/**
 * Create buffer
 */
le::FIRHIBufferRef le::OpenGL4RHI::CreateBuffer( EBufferType InBufferType, EUsageBufferType InUsageBufferType )
{
	return FIRHIBufferRef( new RHIBuffer( InBufferType, InUsageBufferType ) );
}

/**
 * Create vertex format
 */
le::FIRHIVertexFormatRef le::OpenGL4RHI::CreateVertexFormat()
{
	return FIRHIVertexFormatRef( new RHIVertexFormat() );
}

/**
 * Create geometry
 */
le::FIRHIGeometryRef le::OpenGL4RHI::CreateGeometry()
{
	return FIRHIGeometryRef( new RHIGeometry() );
}

/**
 * Create texture 2d
 */
le::FIRHITexture2DRef le::OpenGL4RHI::CreateTexture2D( EImageFormat InImageFormat, uint32 InWidth, uint32 InHeight, uint32 InCountMipmap )
{
	return FIRHITexture2DRef( new RHITexture2D( InImageFormat, InWidth, InHeight, InCountMipmap ) );
}

/**
 * Create render target
 */
le::FIRHIRenderTargetRef le::OpenGL4RHI::CreateRenderTarget( uint32 InWidth, uint32 InHeight )
{
	return FIRHIRenderTargetRef( new RHIRenderTarget( InWidth, InHeight ) );
}

/**
 * Make current context
 */
bool le::OpenGL4RHI::MakeCurrentContext( FRHIContext InRHIContext )
{
#ifdef PLATFORM_WINDOWS
	return WinGL_MakeCurrentContext( InRHIContext );
#else
	return false;
#endif // PLATFORM_WINDOWS	
}

/**
 * Draw geometry
 */
void le::OpenGL4RHI::Draw( EDrawOperation InDrawOperation, uint32 InVertexOffset, uint32 InVertexCount, uint32 InInstaceCount )
{
	if ( InInstaceCount <= 1 )
		glDrawArrays( Convert_EDrawOperationToGLDrawOperation( InDrawOperation ), InVertexOffset, InVertexCount );
	else
		glDrawArraysInstanced( Convert_EDrawOperationToGLDrawOperation( InDrawOperation ), InVertexOffset, InVertexCount, InInstaceCount );
}

/**
 * Draw indexed geometry
 */
void le::OpenGL4RHI::DrawIndexed( EDrawOperation InDrawOperation, uint32 InStartIndex, uint32 InIndexCount, uint32 InVetexOffset, uint32 InVertexCount, uint32 InInstaceCount )
{
	uint32		indecesType = GLState::GetIndecesType();

	if ( InInstaceCount <= 1 )
		glDrawElementsBaseVertex( Convert_EDrawOperationToGLDrawOperation( InDrawOperation ), InIndexCount, indecesType, ( void* )( InStartIndex * GetIndexSizeof( indecesType ) ), InVetexOffset );
	else
		glDrawElementsInstancedBaseVertex( Convert_EDrawOperationToGLDrawOperation( InDrawOperation ), InIndexCount, indecesType, ( void* )( InStartIndex * GetIndexSizeof( indecesType ) ), InInstaceCount, InVetexOffset  );
}

/**
 * Clear
 */
void le::OpenGL4RHI::Clear( const SColor& InColor, uint32 InClearFlags )
{
	GLState::SetClearColor( InColor );
	glClear( GetClearFlags( InClearFlags ) );
}

/**
 * Delete context
 */
void le::OpenGL4RHI::DeleteContext( FRHIContext& InRHIContext )
{
#ifdef PLATFORM_WINDOWS
	return WinGL_DeleteContext( InRHIContext );
#endif // PLATFORM_WINDOWS	

	InRHIContext = nullptr;
}

/**
 * Swap buffers
 */
void le::OpenGL4RHI::SwapBuffers( FRHIContext InRHIContext )
{
#ifdef PLATFORM_WINDOWS
	return WinGL_SwapBuffers( InRHIContext );
#endif // PLATFORM_WINDOWS	
}

/**
 * Set vertical sync
 */
void le::OpenGL4RHI::SetVerticalSync( bool InIsEnable )
{
#ifdef PLATFORM_WINDOWS
	return WinGL_SetVerticalSync( InIsEnable );
#endif // PLATFORM_WINDOWS	
}

/**
 * Set viewport
 */
void le::OpenGL4RHI::SetViewport( uint32 InX, uint32 InY, uint32 InWidth, uint32 InHeight )
{
	glViewport( InX, InY, InWidth, InHeight );
}

/**
 * Set shader
 */
void le::OpenGL4RHI::SetShader( FIRHIShaderConstRef& InShader )
{
	LIFEENGINE_ASSERT( InShader.IsValid() );
	static_cast< RHIShader* >( InShader.GetPtr() )->Bind() ;
}

/**
 * Unset shader
 */
void le::OpenGL4RHI::UnsetShader()
{
	RHIShader::Unbind();
}

/**
 * Set geometry for draw
 */
void le::OpenGL4RHI::SetGeometry( FIRHIGeometryConstRef& InGeometry )
{
	LIFEENGINE_ASSERT( InGeometry.IsValid() );
	static_cast< RHIGeometry* >( InGeometry.GetPtr() )->Bind();
}

/**
 * Unset geometry
 */
void le::OpenGL4RHI::UnsetGeometry()
{
	RHIGeometry::Unbind();
}

/**
 * Set texture 2d
 */
void le::OpenGL4RHI::SetTexture2D( FIRHITexture2DConstRef& InTexture2D, uint32 InTextureLayer )
{
	LIFEENGINE_ASSERT( InTexture2D.IsValid() );
	static_cast< RHITexture2D* >( InTexture2D.GetPtr() )->Bind( InTextureLayer );
}

/**
 * Unset texture 2D
 */
void le::OpenGL4RHI::UnsetTexture2D( uint32 InTextureLayer )
{
	RHITexture2D::Unbind( InTextureLayer );
}

/**
 * Set render target
 */
void le::OpenGL4RHI::SetRenderTarget( FIRHIRenderTargetConstRef& InRenderTarget )
{
	LIFEENGINE_ASSERT( InRenderTarget.IsValid() );
	static_cast< RHIRenderTarget* >( InRenderTarget.GetPtr() )->Bind();
}

/**
 * Unset render target
 */
void le::OpenGL4RHI::UnsetRenderTarget()
{
	RHIRenderTarget::Unbind();
}