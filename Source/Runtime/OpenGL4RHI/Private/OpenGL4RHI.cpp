// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <GL/glew.h>

#include "OpenGL4RHI.h"
#include "RHIShader.h"
#include "RHIBuffer.h"
#include "RHIVertexFormat.h"
#include "RHIGeometry.h"

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
le::IRHIShader* le::OpenGL4RHI::CreateShader()
{
	return new RHIShader();
}

/**
 * Create buffer
 */
le::IRHIBuffer* le::OpenGL4RHI::CreateBuffer( EBufferType InBufferType, EUsageBufferType InUsageBufferType )
{
	return new RHIBuffer( InBufferType, InUsageBufferType );
}

/**
 * Create vertex format
 */
le::IRHIVertexFormat* le::OpenGL4RHI::CreateVertexFormat()
{
	return new RHIVertexFormat();
}

/**
 * Create geometry
 */
le::IRHIGeometry* le::OpenGL4RHI::CreateGeometry()
{
	return new RHIGeometry();
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
 * Delete shader
 */
void le::OpenGL4RHI::DeleteShader( IRHIShader*& InOutShader )
{
	LIFEENGINE_ASSERT( InOutShader );
	
	delete InOutShader;
	InOutShader = nullptr;
}

/**
 * Delete buffer
 */
void le::OpenGL4RHI::DeleteBuffer( IRHIBuffer*& InOutBuffer )
{
	LIFEENGINE_ASSERT( InOutBuffer );

	delete InOutBuffer;
	InOutBuffer = nullptr;
}

/**
 * Delete vertex format
 */
void le::OpenGL4RHI::DeleteVertexFormat( IRHIVertexFormat*& InOutVertexFormat )
{
	LIFEENGINE_ASSERT( InOutVertexFormat );

	delete InOutVertexFormat;
	InOutVertexFormat = nullptr;
}

/**
 * Delete geometry
 */
void le::OpenGL4RHI::DeleteGeometry( IRHIGeometry*& InGeometry )
{
	LIFEENGINE_ASSERT( InGeometry );

	delete InGeometry;
	InGeometry = nullptr;
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
void le::OpenGL4RHI::SetShader( IRHIShader* InShader )
{
	if ( !InShader )
		RHIShader::Unbind();
	else if ( InShader->IsLoaded() )
		static_cast< RHIShader* >( InShader )->Bind() ;
}

/**
 * Set geometry for draw
 */
void le::OpenGL4RHI::SetGeometry( IRHIGeometry* InGeometry )
{
	if ( InGeometry )
		static_cast< RHIGeometry* >( InGeometry )->Bind();
	else
		RHIGeometry::Unbind();
}