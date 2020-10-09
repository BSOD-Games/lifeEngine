// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include <GL/glew.h>

#include "RHIGeometry.h"
#include "RHIBuffer.h"

/**
 * Convert EBufferType to OpenGL Buffer Type
 */
FORCEINLINE le::uint32 Convert_EBufferTypeToGLBufferType( le::EBufferType InBufferType )
{
	switch ( InBufferType )
	{
	case le::BT_Vertex:			return GL_ARRAY_BUFFER;
	case le::BT_Index:			return GL_ELEMENT_ARRAY_BUFFER;
	default:					return 0;
	}
}

/**
 * Convert OpenGL Buffer Type to EBufferType
 */
FORCEINLINE le::EBufferType Convert_GLBufferTypeToEBufferType( le::uint32 InBufferType )
{
	switch ( InBufferType )
	{
	case GL_ARRAY_BUFFER:				return le::BT_Vertex;
	case GL_ELEMENT_ARRAY_BUFFER:		return le::BT_Index;
	default:							return le::BT_Unknown;
	}
}

/**
 * Convert EUsageBufferType to OpenGL Usage Buffer Type
 */
FORCEINLINE le::uint32 Convert_EUsageBufferTypeToGLUsageBufferType( le::EUsageBufferType InUsageBufferType )
{
	switch ( InUsageBufferType )
	{
	case le::UBT_Static:			return GL_STATIC_DRAW;
	case le::UBT_Dynamic:			return GL_DYNAMIC_DRAW;
	case le::UBT_Stream:			return GL_STREAM_DRAW;
	default:						return 0;
	}
}

/**
 * Convert OpenGL Usage Buffer Type to EUsageBufferType
 */
FORCEINLINE le::EUsageBufferType Convert_GLUsageBufferTypeToEUsageBufferType( le::uint32 InUsageBufferType )
{
	switch ( InUsageBufferType )
	{
	case GL_STATIC_DRAW:		return le::UBT_Static;
	case GL_DYNAMIC_DRAW:		return le::UBT_Dynamic;
	case GL_STREAM_DRAW:		return le::UBT_Stream;
	default:					return le::UBT_Static;
	}
}

/**
 * Constructor
 */
le::RHIBuffer::RHIBuffer( EBufferType InBufferType, EUsageBufferType InUsageBufferType ) :
	type( Convert_EBufferTypeToGLBufferType( InBufferType ) ),
	usageType( Convert_EUsageBufferTypeToGLUsageBufferType( InUsageBufferType ) )
{
	glGenBuffers( 1, &handle );
}

/**
 * Destructor
 */
le::RHIBuffer::~RHIBuffer()
{
	for ( auto it = usagedInGeometries.begin(), itEnd = usagedInGeometries.end(); it != itEnd; ++it )
		( *it )->NotifyDestroy( this );

	glDeleteBuffers( 1, &handle );
}

/**
 * Allocate data in buffer
 */
void le::RHIBuffer::Allocate( const void* InData, uint32 InSize )
{
	LIFEENGINE_ASSERT( handle && InData );
	uint32 currentBuffer = GLState::GetBuffer( type );
	
	GLState::BindBuffer( type, handle );
	glBufferData( type, InSize, InData, usageType );
	size = InSize;
	
	GLState::BindBuffer( type, currentBuffer );
}

/**
 * Update data in buffer
 */
void le::RHIBuffer::Update( const void* InData, uint32 InSize, uint32 InOffset )
{
	LIFEENGINE_ASSERT( handle && InData );
	uint32 currentBuffer = GLState::GetBuffer( type );
	
	GLState::BindBuffer( type, handle );
	glBufferSubData( type, InOffset, InSize, InData );
	GLState::BindBuffer( type, currentBuffer );
}

/**
 * Usage in geometry
 */
void le::RHIBuffer::UsageInGeometry( RHIGeometry* InGeometry )
{
	usagedInGeometries.insert( InGeometry );
}

/**
 * Notify destroy geometry
 */
void le::RHIBuffer::NotifyDestroyGeometry( RHIGeometry* InGeometry )
{
	usagedInGeometries.erase( InGeometry );
}

/**
 * Get size buffer
 */
le::uint32 le::RHIBuffer::GetSize() const
{
	return size;
}

/**
 * Get type buffer
 */
le::EBufferType le::RHIBuffer::GetType() const
{
	return Convert_GLBufferTypeToEBufferType( type );
}

/**
 * Get type usage
 */
le::EUsageBufferType le::RHIBuffer::GetUsageType() const
{
	return Convert_GLUsageBufferTypeToEUsageBufferType( usageType );
}
