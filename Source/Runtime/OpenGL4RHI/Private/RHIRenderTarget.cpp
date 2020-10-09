// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include <GL/glew.h>

#include "RHITexture2D.h"
#include "RHIRenderTarget.h"

/**
 * Constructor
 */
le::RHIRenderTarget::RHIRenderTarget() :
	width( 0 ),
	height( 0 )
{
	glGenFramebuffers( 1, &handle );
}

/**
 * Constructor
 */
le::RHIRenderTarget::RHIRenderTarget( uint32 InWidth, uint32 InHeight ) :
	width( InWidth ),
	height( InHeight )
{
	glGenFramebuffers( 1, &handle );
}

/**
 * Destructor
 */
le::RHIRenderTarget::~RHIRenderTarget()
{
	Destroy();
}

/**
 * Create texture 2D
 */
void le::RHIRenderTarget::CreateTexture2D( EImageFormat InImageFormat, const SSampler& InSampler )
{
	uint32			currentFBO = GLState::GetFBO();
	GLState::BindFBO( handle );

	uint32				typeAttach = InImageFormat != IF_Depth24_Stencil8 ? GL_COLOR_ATTACHMENT0 + static_cast< uint32 >( buffers.size() ) : GL_DEPTH_STENCIL_ATTACHMENT;
	RHITexture2D*		texture2D = new RHITexture2D( InImageFormat, width, height, 1 );
	texture2D->Bind();
	texture2D->Allocate( nullptr );
	texture2D->SetSampler( InSampler );
	glFramebufferTexture2D( GL_FRAMEBUFFER, typeAttach, GL_TEXTURE_2D, texture2D->GetHandle(), 0 );
	texture2D->Unbind();

	buffers.push_back( texture2D );
	GLState::BindFBO( currentFBO );
}

/**
 * Destroy texture 2D
 */
void le::RHIRenderTarget::DestroyTexture2D( uint32 InIndex )
{
	LIFEENGINE_ASSERT( InIndex < buffers.size() );
	RHITexture2D*	texture2D = buffers[ InIndex ];
	uint32			typeAttach = texture2D->GetFormat() != IF_Depth24_Stencil8 ? GL_COLOR_ATTACHMENT0 + InIndex : GL_DEPTH_STENCIL_ATTACHMENT;

	uint32			currentFBO = GLState::GetFBO();
	GLState::BindFBO( handle );
	glFramebufferTexture2D( GL_FRAMEBUFFER, typeAttach, GL_TEXTURE_2D, 0, 0 );
	GLState::BindFBO( currentFBO );

	delete texture2D;
	buffers.erase( buffers.begin() + InIndex );
}

/**
 * Set size
 */
void le::RHIRenderTarget::SetSize( uint32 InWidth, uint32 InHeight )
{
	width = InWidth;
	height = InHeight;

	for ( uint32 index = 0, count = static_cast< uint32 >( buffers.size() ); index < count; ++index )
		buffers[ index ]->Resize( InWidth, InHeight );
}

/**
 * Get width
 */
le::uint32 le::RHIRenderTarget::GetWidth() const
{
	return width;
}

/**
 * Get height
 */
le::uint32 le::RHIRenderTarget::GetHeight() const
{
	return height;
}

/**
 * Get texture 2D
 */
le::IRHITexture2D* le::RHIRenderTarget::GetTexture2D( uint32 InIndex ) const
{
	LIFEENGINE_ASSERT( InIndex < buffers.size() );
	return buffers[ InIndex ];
}

/**
 * Destroy
 */
void le::RHIRenderTarget::Destroy()
{
	uint32			currentFBO = GLState::GetFBO();
	GLState::BindFBO( handle );

	for ( uint32 index = 0, count = static_cast< uint32 >( buffers.size() ); index < count; ++index )
		DestroyTexture2D( 0 );

	glDeleteFramebuffers( 1, &handle );
	GLState::BindFBO( currentFBO );

	handle = 0;
}