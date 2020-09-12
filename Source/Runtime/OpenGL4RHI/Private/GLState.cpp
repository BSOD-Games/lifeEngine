// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <GL/glew.h>

#include "GLState.h"

le::uint32											le::GLState::gpuProgram = 0;
le::uint32											le::GLState::vao = 0;
le::uint32											le::GLState::indecesType = GL_INVALID_ENUM;
std::unordered_map< le::uint32, le::uint32 >		le::GLState::buffers;

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
void le::GLState::BindShader( uint32 InGpuProgram )
{
	if ( gpuProgram == InGpuProgram ) return;
	
	glUseProgram( InGpuProgram );
	gpuProgram = InGpuProgram;
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
 * Set indeces type
 */
void le::GLState::SetIndecesType( uint32 InIndecesType )
{
	indecesType = InIndecesType;
}
