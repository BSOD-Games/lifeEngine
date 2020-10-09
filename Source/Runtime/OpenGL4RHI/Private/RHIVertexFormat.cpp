// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "RHIGeometry.h"
#include "RHIVertexFormat.h"

/**
 * Constructor
 */
le::RHIVertexFormat::RHIVertexFormat() :
	stride( 0 )
{}

/**
 * Destructor
 */
le::RHIVertexFormat::~RHIVertexFormat()
{
	for ( auto it = usagedInGeometries.begin(), itEnd = usagedInGeometries.end(); it != itEnd; ++it )
		( *it )->NotifyDestroy( this );
}

/**
 * Usage in geometry
 */
void le::RHIVertexFormat::UsageInGeometry( RHIGeometry* InGeometry )
{
	usagedInGeometries.insert( InGeometry );
}

/**
 * Notify destroy geometry
 */
void le::RHIVertexFormat::NotifyDestroyGeometry( RHIGeometry* InGeometry )
{
	usagedInGeometries.erase( InGeometry );
}

/**
 * Push float
 */
void le::RHIVertexFormat::PushFloat( uint32 InCount )
{
	elements.push_back( { GL_FLOAT, InCount, GL_FALSE } );
	stride += InCount * SVertexElement::GetSizeOfType( GL_FLOAT );
}

/**
 * Push uint
 */
void le::RHIVertexFormat::PushUInt( uint32 InCount )
{
	elements.push_back( { GL_UNSIGNED_INT, InCount, GL_FALSE } );
	stride += InCount * SVertexElement::GetSizeOfType( GL_UNSIGNED_INT );
}

/**
 * Push ubyte
 */
void le::RHIVertexFormat::PushUByte( uint32 InCount )
{
	elements.push_back( { GL_UNSIGNED_BYTE, InCount, GL_TRUE } );
	stride += InCount * SVertexElement::GetSizeOfType( GL_UNSIGNED_BYTE );
}

/**
 * Clear
 */
void le::RHIVertexFormat::Clear()
{
	elements.clear();
}

/**
 * Get stride
 */
const le::uint32& le::RHIVertexFormat::GetStride() const
{
	return stride;
}

/**
 * Get elements
 */
const std::vector< le::SVertexElement >& le::RHIVertexFormat::GetElements() const
{
	return elements;
}
