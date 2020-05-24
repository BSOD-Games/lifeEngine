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

#include "engine/icamera.h"
#include "engine/imaterialinternal.h"
#include "engine/ishader.h"

#include "openglstate.h"
#include "vertexarrayobject.h"
#include "spriterenderer.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::SpriteRenderer::SpriteRenderer() :
	isInitialize( false )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::SpriteRenderer::~SpriteRenderer()
{}

// ------------------------------------------------------------------------------------ //
// Initialize
// ------------------------------------------------------------------------------------ //
bool le::SpriteRenderer::Initialize()
{
	return true;
}

// ------------------------------------------------------------------------------------ //
// Render object
// ------------------------------------------------------------------------------------ //
void le::SpriteRenderer::Render( const std::vector< RenderObject >& Objects, ICamera* Camera )
{
	for ( UInt32_t index = 0, count = Objects.size(); index < count; ++index )
	{
		const RenderObject&			object = Objects[ index ];

		OpenGLState::EnableDepthTest( object.material->IsDepthTest() );
		OpenGLState::EnableDepthWrite( object.material->IsDepthWrite() );
		OpenGLState::EnableBlend( object.material->IsBlend() );
		OpenGLState::EnableCullFace( object.material->IsCullFace() );
		OpenGLState::SetCullFaceType( object.material->GetCullFaceType() );

		object.material->Refresh();
		object.material->GetShader()->OnDrawSprite( object.transformation, Camera );
		object.vertexArrayObject->Bind();
		glDrawRangeElementsBaseVertex( object.primitiveType, 0, object.countIndeces, object.countIndeces, GL_UNSIGNED_INT, ( void* ) ( object.startIndex * sizeof( UInt32_t ) ), object.startVertexIndex );
	}
}