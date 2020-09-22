// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <GL/glew.h>

#include "Logging/LogMacros.h"
#include "RHITexture2D.h"

struct SGLImageFormat
{
	le::uint32		internalFormat;
	le::uint32		format;
	le::uint32		type;
};

/**
 * EImageFormat to OpenGL image format
 */
FORCEINLINE SGLImageFormat Convert_EImageFormatToGLImageFormat( le::EImageFormat InImageFormat )
{
	switch ( InImageFormat )
	{
	case le::IF_R_8Unorm:			return { GL_RED, GL_RED, GL_UNSIGNED_BYTE };
	case le::IF_RGBA_8Unorm:		return { GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE };
	case le::IF_RGB_8Unorm:			return { GL_RGB, GL_RGB, GL_UNSIGNED_BYTE };
	case le::IF_RGBA_16Float:		return { GL_RGBA16F, GL_RGBA, GL_FLOAT };
	case le::IF_RGB_16Float:		return { GL_RGB16F, GL_RGB, GL_FLOAT };
	case le::IF_Depth24_Stencil8:	return { GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8 };
	default:						return { 0, 0, 0 };
	}
}

/**
 * ESamplerFilter to OpenGL sampler
 */
FORCEINLINE le::uint32 Convert_ESamplerFilterToGLSamplerFilter( le::ESamplerFilter InSamplerFilter )
{
	switch ( InSamplerFilter )
	{
	case le::SF_Linear:						return GL_LINEAR;
	case le::SF_Nearest:					return GL_NEAREST;
	case le::SF_NearestMipmapNearest:		return GL_NEAREST_MIPMAP_NEAREST;
	case le::SF_NearestMipmapLinear:		return GL_NEAREST_MIPMAP_LINEAR;
	case le::SF_LinearMipmapNearest:		return GL_LINEAR_MIPMAP_NEAREST;
	case le::SF_LinearMipmapLinear:			return GL_LINEAR_MIPMAP_LINEAR;
	default:								return 0;
	}
}

/**
 * ESamplerAddressMode to OpenGL sampler address mode
 */
FORCEINLINE le::uint32 Convert_ESamplerAddressModeToGLSamplerAddressMode( le::ESamplerAddressMode InSamplerAddressMode )
{
	switch ( InSamplerAddressMode )
	{
	case le::SAM_Border:					return GL_CLAMP_TO_BORDER;
	case le::SAM_Clamp:						return GL_CLAMP_TO_EDGE;
	case le::SAM_Mirror:					return GL_MIRRORED_REPEAT;
	case le::SAM_Repeat:					return GL_REPEAT;
	default:								return 0;
	}
}

/**
 * Constructor
 */
le::RHITexture2D::RHITexture2D( EImageFormat InImageFormat, uint32 InWidth, uint32 InHeight, uint32 InCountMipmap ) :
	imageFormat( InImageFormat ),
	width( InWidth ),
	height( InHeight ),
	countMipmaps( InCountMipmap ),
	handle( 0 )
{
	glGenTextures( 1, &handle );
}

/**
 * Destructor
 */
le::RHITexture2D::~RHITexture2D()
{
	glDeleteTextures( 1, &handle );
}

/**
 * Allocate data in texture
 */
void le::RHITexture2D::Allocate( const uint8* InData, uint32 InMipmapLevel )
{
	LIFEENGINE_ASSERT( InMipmapLevel >= 0 && InMipmapLevel < countMipmaps );

	SGLImageFormat		glImageFormat = Convert_EImageFormatToGLImageFormat( imageFormat );
	uint32				width = GetWidth( InMipmapLevel );
	uint32				height = GetHeight( InMipmapLevel );

	uint32				currentTextureType = 0;
	uint32				currentTexture = GLState::GetTexture( GLState::GetTextureLayer(), &currentTextureType );

	GLState::BindTexture( GL_TEXTURE_2D, handle );
	glTexImage2D( GL_TEXTURE_2D, InMipmapLevel, glImageFormat.internalFormat, width, height, 0, glImageFormat.format, glImageFormat.type, InData );
	GLState::BindTexture( currentTextureType, currentTexture );
}

/**
 * Update data in texture
 */
void le::RHITexture2D::Update( uint32 InX, uint32 InY, uint32 InWidth, uint32 InHeight, const uint8* InData, uint32 InMipmapLevel )
{
	LIFEENGINE_ASSERT( InMipmapLevel >= 0 && InMipmapLevel < countMipmaps&& InWidth <= GetWidth( InMipmapLevel ) && InHeight <= GetHeight( InMipmapLevel ) );

	uint32				currentTextureType = 0;
	uint32				currentTexture = GLState::GetTexture( GLState::GetTextureLayer(), &currentTextureType );	
	GLState::BindTexture( GL_TEXTURE_2D, handle );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );	
	
	glTexSubImage2D( GL_TEXTURE_2D, InMipmapLevel, InX, InY, InWidth, InHeight, Convert_EImageFormatToGLImageFormat( imageFormat ).format, GL_UNSIGNED_BYTE, InData );
	
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
	GLState::BindTexture( currentTextureType, currentTexture );
}

/**
 * Resize texure
 */
void le::RHITexture2D::Resize( uint32 InNewWidth, uint32 InNewHeight, bool InIsSaveData )
{
	// If we don’t need to save the data, just recreate the texture
	if ( !InIsSaveData )
	{
		SGLImageFormat		glImageFormat = Convert_EImageFormatToGLImageFormat( imageFormat );
		uint32				currentTextureType = 0;
		uint32				currentTexture = GLState::GetTexture( GLState::GetTextureLayer(), &currentTextureType );
		
		GLState::BindTexture( GL_TEXTURE_2D, handle );
		glTexImage2D( GL_TEXTURE_2D, 0, glImageFormat.internalFormat, InNewWidth, InNewHeight, 0, glImageFormat.format, glImageFormat.type, nullptr );
		GLState::BindTexture( currentTextureType, currentTexture );

		width = InNewWidth;
		height = InNewHeight;
		return;
	}

	// We save the current frame buffer binders so that we can restore them later
	GLint		readFramebuffer = 0;
	GLint		drawFramebuffer = 0;

	glGetIntegerv( GL_READ_FRAMEBUFFER_BINDING, &readFramebuffer );
	glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebuffer );

	// Create frame buffers
	GLuint		sourceFramebuffer = 0;
	GLuint		destFramebuffer = 0;

	glGenFramebuffers( 1, &sourceFramebuffer );
	glGenFramebuffers( 1, &destFramebuffer );

	if ( !sourceFramebuffer || !destFramebuffer )
	{
		LIFEENGINE_LOG_ERROR( "OpenGL4RHI", "Cannot copy texture, failed to create a frame buffer object" );
		return;
	}

	// Remember the identifier of the old texture and create a new oneþ
	SGLImageFormat		glImageFormat = Convert_EImageFormatToGLImageFormat( imageFormat );
	GLuint				oldTexture = handle;
	uint32				currentTextureType = 0;
	uint32				currentTexture = GLState::GetTexture( GLState::GetTextureLayer(), &currentTextureType );

	glGenTextures( 1, &handle );
	GLState::BindTexture( GL_TEXTURE_2D, handle );
	glTexImage2D( GL_TEXTURE_2D, 0, glImageFormat.internalFormat, InNewWidth, InNewHeight, 0, glImageFormat.format, glImageFormat.type, nullptr );

	ApplySampler( sampler );

	// Join the original texture with the original frame buffer
	glBindFramebuffer( GL_READ_FRAMEBUFFER, sourceFramebuffer );
	glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, oldTexture, 0 );

	// Join destination texture with destination frame buffer
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, destFramebuffer );
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle, 0 );

	// Final check for correct creation of buffers
	GLenum			sourceStatus;
	sourceStatus = glCheckFramebufferStatus( GL_READ_FRAMEBUFFER );

	GLenum			destStatus;
	destStatus = glCheckFramebufferStatus( GL_DRAW_FRAMEBUFFER );

	if ( sourceStatus == GL_FRAMEBUFFER_COMPLETE && destStatus == GL_FRAMEBUFFER_COMPLETE )
	{
		// Draws the contents of the texture from the source to the destination of the texture
		glBlitFramebuffer(
			0, 0, width, height,		// source rectangle
			0, 0, width, height,		// destination rectangle
			GL_COLOR_BUFFER_BIT, GL_NEAREST
		);
	}
	else
		LIFEENGINE_LOG_ERROR( "OpenGL4RHI", "Cannot copy texture, failed to link texture to frame buffer" );

	// Restore buffers
	glBindFramebuffer( GL_READ_FRAMEBUFFER, readFramebuffer );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, drawFramebuffer );

	// Delete the buffers that were created and the old texture
	glDeleteFramebuffers( 1, &sourceFramebuffer );
	glDeleteFramebuffers( 1, &destFramebuffer );
	glDeleteTextures( 1, &oldTexture );

	// If previously there were mipmap levels, then we generate them again
	if ( countMipmaps > 1 )		GenerateMipmaps();

	width = InNewWidth;
	height = InNewHeight;
	GLState::BindTexture( GL_TEXTURE_2D, currentTexture );
}

/**
 * Generate mipmaps
 */
void le::RHITexture2D::GenerateMipmaps()
{
	uint32				currentTextureType = 0;
	uint32				currentTexture = GLState::GetTexture( GLState::GetTextureLayer(), &currentTextureType );
	countMipmaps = static_cast< uint32 >( floor( log2( width > height ? width : height ) ) );

	GLState::BindTexture( GL_TEXTURE_2D, handle );
	glGenerateMipmap( GL_TEXTURE_2D );
	GLState::BindTexture( GL_TEXTURE_2D, currentTexture );
}

/**
 * Set sampler
 */
void le::RHITexture2D::SetSampler( const SSampler& InSampler )
{
	sampler = InSampler;
	ApplySampler( sampler );
}

/**
 * Get width
 */
le::uint32 le::RHITexture2D::GetWidth( uint32 InMipmapLevel ) const
{
	LIFEENGINE_ASSERT( InMipmapLevel >= 0 && InMipmapLevel < countMipmaps );
	
	uint32			width = this->width >> InMipmapLevel;
	return width >= 1 ? width : 1;
}

/**
 * Get height
 */
le::uint32 le::RHITexture2D::GetHeight( uint32 InMipmapLevel ) const
{
	LIFEENGINE_ASSERT( InMipmapLevel >= 0 && InMipmapLevel < countMipmaps );

	uint32			height = this->height >> InMipmapLevel;
	return height >= 1 ? height : 1;
}

/**
 * Get count mipmaps
 */
le::uint32 le::RHITexture2D::GetCountMipmaps() const
{
	return countMipmaps;
}

/**
 * Get image format
 */
le::EImageFormat le::RHITexture2D::GetFormat() const
{
	return imageFormat;
}

/**
 * Get sampler
 */
const le::SSampler& le::RHITexture2D::GetSampler() const
{
	return sampler;
}

/**
 * Apply sampler
 */
void le::RHITexture2D::ApplySampler( const SSampler& InSampler )
{
	uint32				currentTextureType = 0;
	uint32				currentTexture = GLState::GetTexture( GLState::GetTextureLayer(), &currentTextureType );

	GLState::BindTexture( GL_TEXTURE_2D, handle );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Convert_ESamplerAddressModeToGLSamplerAddressMode( InSampler.addressU ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Convert_ESamplerAddressModeToGLSamplerAddressMode( InSampler.addressV ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, Convert_ESamplerAddressModeToGLSamplerAddressMode( InSampler.addressW ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Convert_ESamplerFilterToGLSamplerFilter( InSampler.minFilter ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Convert_ESamplerFilterToGLSamplerFilter( InSampler.magFilter ) );
	glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, InSampler.borderColor );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, InSampler.minLod );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, InSampler.maxLod );
	GLState::BindTexture( GL_TEXTURE_2D, currentTexture );
}
