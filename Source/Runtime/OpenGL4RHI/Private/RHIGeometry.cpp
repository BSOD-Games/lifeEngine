// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "RHIBuffer.h"
#include "RHIVertexFormat.h"
#include "RHIGeometry.h"

/**
 * EIndecesType to OpenGL indeces type
 */
FORCEINLINE le::uint32 Convert_EIndecesTypeToGLIndecesType( le::EIndecesType InIndecesType )
{
	switch ( InIndecesType )
	{
	case le::IT_UByte:		return GL_UNSIGNED_BYTE;
	case le::IT_UShort:		return GL_UNSIGNED_SHORT;
	case le::IT_UInt:		return GL_UNSIGNED_INT;
	default:				return GL_INVALID_ENUM;
	}
}

/**
 * Constructor
 */
le::RHIGeometry::RHIGeometry() :
	isNeedRefresh( false ),
	vao( 0 ),
	vertexBuffer( nullptr ),
	vertexFormat( nullptr ),
	indexBuffer( nullptr )
{}

/**
 * Destructor
 */
le::RHIGeometry::~RHIGeometry()
{
	Destroy();
}

/**
 * Bind geometry
 */
void le::RHIGeometry::Bind()
{
	if ( isNeedRefresh )
	{
		if ( vao > 0 )							Destroy();
		if ( vertexBuffer && vertexFormat )		Create();

		isNeedRefresh = false;
	}

	GLState::BindVAO( vao );
	GLState::SetIndecesType( Convert_EIndecesTypeToGLIndecesType( indecesType ) );
}

/**
 * Notify destroy
 */
void le::RHIGeometry::NotifyDestroy( RHIBuffer* InBuffer )
{
	LIFEENGINE_ASSERT( InBuffer );

	switch ( InBuffer->GetType() )
	{
	case BT_Vertex:
		if ( InBuffer != vertexBuffer )			return;

		vertexBuffer = nullptr;
		break;

	case BT_Index:
		if ( InBuffer != indexBuffer )			return;

		indexBuffer = nullptr;
		break;

	default: return;
	}

	isNeedRefresh = true;
}

/**
 * Notify destroy
 */
void le::RHIGeometry::NotifyDestroy( RHIVertexFormat* InVertexFormat )
{
	LIFEENGINE_ASSERT( InVertexFormat );
	if ( InVertexFormat != vertexFormat )		return;
	
	vertexFormat = nullptr;
	isNeedRefresh = true;
}

/**
 * Set vertex buffer
 */
void le::RHIGeometry::SetVertexBuffer( IRHIBuffer* InVertexBuffer )
{
	LIFEENGINE_ASSERT( InVertexBuffer );
	if ( InVertexBuffer->GetType() != BT_Vertex )	return;
	if ( vertexBuffer )		vertexBuffer->NotifyDestroyGeometry( this );

	vertexBuffer = static_cast< RHIBuffer* >( InVertexBuffer );
	isNeedRefresh = true;
}

/**
 * Set vertex format
 */
void le::RHIGeometry::SetVertexFormat( IRHIVertexFormat* InVertexFormat )
{
	LIFEENGINE_ASSERT( InVertexFormat );
	if ( vertexFormat )		vertexFormat->NotifyDestroyGeometry( this );

	vertexFormat = static_cast< RHIVertexFormat* >( InVertexFormat );
	isNeedRefresh = true;
}

/**
 * Set index buffer
 */
void le::RHIGeometry::SetIndexBuffer( IRHIBuffer* InIndexBuffer, EIndecesType InIndecesType )
{
	LIFEENGINE_ASSERT( InIndexBuffer );
	if ( InIndexBuffer->GetType() != BT_Index )	return;
	if ( indexBuffer )		indexBuffer->NotifyDestroyGeometry( this );

	indexBuffer = static_cast< RHIBuffer* >( InIndexBuffer );
	indecesType = InIndecesType;
	isNeedRefresh = true;
}

/**
 * Get indeces type
 */
le::EIndecesType le::RHIGeometry::GetIndecesType() const
{
	return indecesType;
}

/**
 * Create
 */
void le::RHIGeometry::Create()
{
	if ( vao > 0 )		return;
	LIFEENGINE_ASSERT( vertexBuffer && vertexFormat );
	uint32		currentVAO = GLState::GetVAO();

	glGenVertexArrays( 1, &vao );
	GLState::BindVAO( vao );
	vertexBuffer->Bind();
	if ( indexBuffer )		indexBuffer->Bind();

	const std::vector< SVertexElement >&	elements = vertexFormat->GetElements();
	uint32									offset = 0;

	for ( uint32 index = 0, count = ( uint32 ) elements.size(); index < count; ++index )
	{
		const SVertexElement&		element = elements[ index ];

		glEnableVertexAttribArray( index );
		glVertexAttribPointer( index, element.count, element.type, element.normalized, vertexFormat->GetStride(), ( void* ) offset );
		offset += element.count * SVertexElement::GetSizeOfType( element.type );
	}

	GLState::BindVAO( currentVAO );
	vertexBuffer->Unbind();
	if ( indexBuffer )		indexBuffer->Unbind();
}