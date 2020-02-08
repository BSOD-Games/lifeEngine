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

#include "studiorender/studiorender.h"
#include "openglstate.h"
 
//---------------------------------------------------------------------//

bool					le::OpenGLState::isDepthTest = true;
bool					le::OpenGLState::isDepthWrite = true;
bool					le::OpenGLState::isBlend = true;
bool					le::OpenGLState::isCullFace = true;
le::CULLFACE_TYPE		le::OpenGLState::cullFaceType = le::CT_BACK;

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
// Инициализировать состояние OpenGL
// ------------------------------------------------------------------------------------ //
void le::OpenGLState::Initialize()
{
	isDepthTest ? glEnable( GL_DEPTH_TEST ) : glDisable( GL_DEPTH_TEST );
	isDepthWrite ? glDepthMask( GL_TRUE ) : glDepthMask( GL_FALSE );
	isBlend ? glEnable( GL_BLEND ) : glDisable( GL_BLEND );
	isCullFace ? glEnable( GL_CULL_FACE ) : glDisable( GL_CULL_FACE );

	switch ( cullFaceType )
	{
	case CT_BACK:		glCullFace( GL_BACK ); return;
	case CT_FRONT:		glCullFace( GL_FRONT ); return;
	}
}
