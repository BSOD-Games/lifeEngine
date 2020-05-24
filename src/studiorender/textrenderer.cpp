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
#include "textrenderer.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::TextRenderer::TextRenderer() :
	isInitialize( false )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::TextRenderer::~TextRenderer()
{}

// ------------------------------------------------------------------------------------ //
// Initialize
// ------------------------------------------------------------------------------------ //
bool le::TextRenderer::Initialize()
{
	return true;
}

// ------------------------------------------------------------------------------------ //
// Render object
// ------------------------------------------------------------------------------------ //
void le::TextRenderer::Render( const std::vector< RenderObject >& Objects, ICamera* Camera )
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
		object.material->GetShader()->OnDrawText( object.transformation, Camera, ( ITexture* ) object.glyph );
		object.vertexArrayObject->Bind();
		glDrawRangeElementsBaseVertex( object.primitiveType, 0, object.countIndeces, object.countIndeces, GL_UNSIGNED_INT, ( void* ) ( object.startIndex * sizeof( UInt32_t ) ), object.startVertexIndex );
	}
}