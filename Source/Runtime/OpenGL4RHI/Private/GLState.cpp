// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include <GL/glew.h>

#include "GLState.h"

le::uint32																	le::GLState::shader = 0;
le::uint32																	le::GLState::vao = 0;
le::uint32																	le::GLState::indecesType = GL_INVALID_ENUM;
le::uint32																	le::GLState::textureLayer = GL_TEXTURE0;
le::uint32																	le::GLState::fbo = 0;
le::SColor																	le::GLState::clearColor;
std::unordered_map< le::uint32, le::uint32 >								le::GLState::buffers;
std::unordered_map< le::uint32, std::pair< le::uint32, le::uint32 > >		le::GLState::textures;

/**
 * Bind buffer
 */
void le::GLState::BindBuffer( uint32 InBufferType, uint32 InBuffer )
{
	auto it = buffers.find( InBufferType );
	if ( it != buffers.end() && it->second == InBuffer )
		return;

	glBindBuffer( InBufferType, InBuffer );
	buffers[ InBufferType ] = InBuffer;
}

/**
 * Bind shader
 */
void le::GLState::BindShader( uint32 InShader )
{
	if ( shader == InShader ) return;
	
	glUseProgram( InShader );
	shader = InShader;
}

/**
 * Bind VAO
 */
void le::GLState::BindVAO( uint32 InVAO )
{
	if ( vao == InVAO ) return;

	glBindVertexArray( InVAO );
	vao = InVAO;
}

/**
 * Bind FBO
 */
void le::GLState::BindFBO( uint32 InFBO )
{
	if ( fbo == InFBO ) return;

	glBindFramebuffer( GL_FRAMEBUFFER, InFBO );
	fbo = InFBO;
}

/**
 * Bind texture
 */
void le::GLState::BindTexture( uint32 InTextureType, uint32 InTexture )
{
	auto			it = textures.find( textureLayer );
	if ( it == textures.end() || InTextureType != it->second.first || InTexture != it->second.second )
	{
		if ( !InTexture )		glBindTexture( InTextureType, 0 );
		else					glBindTexture( InTextureType, InTexture );

		textures[ textureLayer ] = std::make_pair( InTextureType, InTexture );
	}
}

/**
 * Set indeces type
 */
void le::GLState::SetIndecesType( uint32 InIndecesType )
{
	indecesType = InIndecesType;
}

/**
 * Bind texture layer
 */
void le::GLState::SetTextureLayer( uint32 InTextureLayer )
{
	if ( textureLayer == InTextureLayer ) return;
	
	glActiveTexture( GL_TEXTURE0 + InTextureLayer );
	textureLayer = InTextureLayer;
}

/**
 * Set clear color
 */
void le::GLState::SetClearColor( const SColor& InColor )
{
	if ( clearColor == InColor ) return;
	glClearColor( InColor.r / 255.f, InColor.g / 255.f, InColor.b / 255.f, InColor.a / 255.f );
}
