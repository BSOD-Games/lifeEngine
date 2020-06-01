//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>
#include <functional>

#include "engine/iconsolesystem.h"

#include "global.h"
#include "studiorender.h"
#include "texture.h"
#include "gpuprogram.h"
#include "openglstate.h"
 
//---------------------------------------------------------------------//

bool														le::OpenGLState::isDepthTest = true;
bool														le::OpenGLState::isDepthWrite = true;
bool														le::OpenGLState::isBlend = true;
bool														le::OpenGLState::isCullFace = true;
bool														le::OpenGLState::isStencilTest = false;

bool														le::OpenGLState::colorMask[ 4 ] = { true, true, true, true };
le::CULLFACE_TYPE											le::OpenGLState::cullFaceType = le::CT_BACK;
le::UInt32_t												le::OpenGLState::stencilFuncType = GL_ALWAYS;
le::UInt32_t												le::OpenGLState::stencilFunc_ref = 0;
le::UInt32_t												le::OpenGLState::stencilFunc_mask = 0;
le::UInt32_t												le::OpenGLState::textureLayer = 0;
le::UInt32_t												le::OpenGLState::blendFunc_sFactor = GL_ONE;
le::UInt32_t												le::OpenGLState::blendFunc_dFactor = GL_ONE;
le::UInt32_t												le::OpenGLState::blendEquation_mode = GL_FUNC_ADD;
le::GPUProgram*												le::OpenGLState::bindedGPUProgram = nullptr;

std::unordered_map< le::UInt32_t, le::Texture* >			le::OpenGLState::bindedTextures = std::unordered_map< le::UInt32_t, le::Texture* >();

//---------------------------------------------------------------------//

// ------------------------------------------------------------------------------------ //
// Включить ли тест глубины
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::EnableDepthTest( bool Enable )
{
	if ( isDepthTest == Enable ) return;
	isDepthTest = Enable;

	isDepthTest ? glEnable( GL_DEPTH_TEST ) : glDisable( GL_DEPTH_TEST );
}

// ------------------------------------------------------------------------------------ //
// Включить ли запись в буфер глубины
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::EnableDepthWrite( bool Enable )
{
	if ( isDepthWrite == Enable ) return;
	isDepthWrite = Enable;

	isDepthWrite ? glDepthMask( GL_TRUE ) : glDepthMask( GL_FALSE );
}

// ------------------------------------------------------------------------------------ //
// Включить ли смешивание
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::EnableBlend( bool Enable )
{
	if ( isBlend == Enable ) return;
	isBlend = Enable;

	isBlend ? glEnable( GL_BLEND ) : glDisable( GL_BLEND );
}

// ------------------------------------------------------------------------------------ //
// Включить ли отсечение полигонов
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::EnableCullFace( bool Enable )
{
	if ( isCullFace == Enable ) return;
	isCullFace = Enable;

	isCullFace ? glEnable( GL_CULL_FACE ) : glDisable( GL_CULL_FACE );
}

// ------------------------------------------------------------------------------------ //
// Включить ли тест трафарета
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::EnableStencilTest( bool Enable )
{
	if ( isStencilTest == Enable )	return;
	isStencilTest = Enable;
	
	isStencilTest ? glEnable( GL_STENCIL_TEST ) : glDisable( GL_STENCIL_TEST );
}

// ------------------------------------------------------------------------------------ //
// Set gpu program
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::SetGPUProgram( GPUProgram* GPUProgram )
{
	if ( bindedGPUProgram == GPUProgram )		return;

	if ( !GPUProgram )		glUseProgram( 0 );
	else					glUseProgram( GPUProgram->GetHandle() );

	bindedGPUProgram = GPUProgram;
}

// ------------------------------------------------------------------------------------ //
// Set texture layer
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::SetTextureLayer( UInt32_t Layer )
{
	if ( textureLayer == Layer ) return;
	glActiveTexture( GL_TEXTURE0 + Layer );
	textureLayer = Layer;
}

// ------------------------------------------------------------------------------------ //
// Задать тип отсекаемых полигонов
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::SetTexture( Texture* Texture )
{
	auto			it = bindedTextures.find( textureLayer );
	if ( it == bindedTextures.end() || Texture != it->second )
	{
		if ( !Texture )		glBindTexture( GL_TEXTURE_2D, 0 );
		else				glBindTexture( GL_TEXTURE_2D, Texture->GetHandle() );
		
		bindedTextures[ textureLayer ] = Texture;
	}
}

// ------------------------------------------------------------------------------------ //
// Задать тип отсекаемых полигонов
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::SetCullFaceType( CULLFACE_TYPE CullFaceType )
{
	if ( cullFaceType == CullFaceType ) return;
	cullFaceType = CullFaceType;

	switch ( cullFaceType )
	{
	case CT_BACK:		glCullFace( GL_BACK ); return;
	case CT_FRONT:		glCullFace( GL_FRONT ); return;
	}
}

// ------------------------------------------------------------------------------------ //
// Задать маску цвета
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::SetColorMask( bool R, bool G, bool B, bool A )
{
	if ( colorMask[ 0 ] == R && colorMask[ 1 ] == G &&
	colorMask[ 2 ] == B && colorMask[ 3 ] == A  )
	return;

	colorMask[ 0 ] = R;
	colorMask[ 1 ] = G;
	colorMask[ 2 ] = B;
	colorMask[ 3 ] = A;

	glColorMask( R, G, B, A );
}

// ------------------------------------------------------------------------------------ //
// Задать функцию смешивания
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::SetBlendFunc( UInt32_t SFactor, UInt32_t DFactor )
{
	if ( SFactor == blendFunc_sFactor && DFactor == blendFunc_dFactor )
		return;

	blendFunc_sFactor = SFactor;
	blendFunc_dFactor = DFactor;

	glBlendFunc( SFactor, DFactor );
}

// ------------------------------------------------------------------------------------ //
// Задать уравнение смешивания
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::SetBlendEquation( UInt32_t Mode )
{
	if ( Mode == blendEquation_mode ) return;
	blendEquation_mode = Mode;

	glBlendEquation( Mode );
}

// ------------------------------------------------------------------------------------ //
// Задать функцию трафарета
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::SetStencilFunc( UInt32_t StencilFuncType, UInt32_t Ref, UInt32_t Mask )
{
	if ( stencilFuncType == StencilFuncType && stencilFunc_ref == Ref &&
	stencilFunc_mask == Mask )
		return;
	
	stencilFuncType = StencilFuncType;
	stencilFunc_ref = Ref;
	stencilFunc_mask = Mask;

	glStencilFunc( StencilFuncType, Ref, Mask );
}

// ------------------------------------------------------------------------------------ //
// Задать операцию заполнения трафарета
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::SetStencilOpSeparate( UInt32_t Face, UInt32_t SFail, UInt32_t DpFail, UInt32_t DpPass )
{
	glStencilOpSeparate( Face, SFail, DpFail, DpPass );
}

// ------------------------------------------------------------------------------------ //
// Инициализировать состояние OpenGL
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::Initialize()
{
	isDepthTest ? glEnable( GL_DEPTH_TEST ) : glDisable( GL_DEPTH_TEST );
	isDepthWrite ? glDepthMask( GL_TRUE ) : glDepthMask( GL_FALSE );
	isBlend ? glEnable( GL_BLEND ) : glDisable( GL_BLEND );
	isCullFace ? glEnable( GL_CULL_FACE ) : glDisable( GL_CULL_FACE );
	isStencilTest ? glEnable( GL_STENCIL_TEST ) : glDisable( GL_STENCIL_TEST );

	switch ( cullFaceType )
	{
	case CT_BACK:		glCullFace( GL_BACK ); break;
	case CT_FRONT:		glCullFace( GL_FRONT ); break;
	}

	glColorMask( colorMask[ 0 ], colorMask[ 1 ], colorMask[ 2 ], colorMask[ 3 ] );
	glStencilFunc( stencilFuncType, stencilFunc_ref, stencilFunc_mask );
	glBlendFunc( blendFunc_sFactor, blendFunc_dFactor );
	glBlendEquation( blendEquation_mode );
}
