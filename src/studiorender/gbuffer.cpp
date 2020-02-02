//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>

#include "common/types.h"
#include "engine/iconsolesystem.h"
#include "studiorender/studiorendersampler.h"
#include "global.h"
#include "gbuffer.h"

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::GBuffer::GBuffer() :
	isInitialize( false ),
	handle_depthBuffer( 0 ),
	handle_frameBuffer( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::GBuffer::~GBuffer()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Инициализировать графический буфер
// ------------------------------------------------------------------------------------ //
bool le::GBuffer::Initialize( const Vector2DInt_t& WindowSize )
{
	// Генерируем буферы

	glGenFramebuffers( 1, &handle_frameBuffer );
	glGenRenderbuffers( 1, &handle_depthBuffer );
	glBindFramebuffer( GL_FRAMEBUFFER, handle_frameBuffer );

	StudioRenderSampler			sampler;
	sampler.minFilter = SF_NEAREST;
	sampler.magFilter = SF_NEAREST;

	// Присоединяем буфер Albedo + Specular
	
	albedoSpecular.Initialize( TT_2D, IF_RGBA_16FLOAT, WindowSize.x, WindowSize.y );
	albedoSpecular.Bind();
	albedoSpecular.Append( nullptr );
	albedoSpecular.SetSampler( sampler );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, albedoSpecular.GetHandle(), 0 );

	// Присоединяем буфер Normal + Shininess

	normalShininess.Initialize( TT_2D, IF_RGBA_16FLOAT, WindowSize.x, WindowSize.y );
	normalShininess.Bind();
	normalShininess.Append( nullptr );
	normalShininess.SetSampler( sampler );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalShininess.GetHandle(), 0 );

	// Присоединяем буфер Emission
	
	emission.Initialize( TT_2D, IF_RGBA_16FLOAT, WindowSize.x, WindowSize.y );
	emission.Bind();
	emission.Append( nullptr );
	emission.SetSampler( sampler );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, emission.GetHandle(), 0 );

	// Инициализируем буфер глубины

	glBindRenderbuffer( GL_RENDERBUFFER, handle_depthBuffer );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, WindowSize.x, WindowSize.y );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, handle_depthBuffer );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );

	// Разрешаем рендер в буфферы нормалей, позиций и т.д

	GLenum			attachments[ 3 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers( 3, attachments );

	// Проверяем статус FBO, создан ли он

	GLenum		status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if ( status != GL_FRAMEBUFFER_COMPLETE )
	{
		g_consoleSystem->PrintError( "Framebuffer status [0x%i]", status );
		Delete();
		return false;
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	windowSize = WindowSize;
	isInitialize = true;

	g_consoleSystem->PrintInfo( "GBuffer initialized with size (%ix%i)", windowSize.x, windowSize.y );
	return true;
}

// ------------------------------------------------------------------------------------ //
// Удалить графический буфер
// ------------------------------------------------------------------------------------ //
void le::GBuffer::Delete()
{
	if ( handle_frameBuffer > 0 )		glDeleteFramebuffers( 1, &handle_frameBuffer );
	if ( handle_depthBuffer > 0 )		glDeleteRenderbuffers( 1, &handle_depthBuffer );
	if ( albedoSpecular.IsCreated() )	albedoSpecular.Delete();
	if ( normalShininess.IsCreated() )	normalShininess.Delete();
	if ( emission.IsCreated() )			emission.Delete();

	isInitialize = false;
}

// ------------------------------------------------------------------------------------ //
// Активировать буфер для прохода
// ------------------------------------------------------------------------------------ //
void le::GBuffer::Bind( BIND_TYPE BindType )
{
	if ( !isInitialize ) return;

	switch ( BindType )
	{
	case BT_GEOMETRY:
		glBindFramebuffer( GL_FRAMEBUFFER, handle_frameBuffer );
		break;

	case BT_LIGHT:	
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );

		// Albedo + Specular
		albedoSpecular.Bind( 0 );

		// Normal + Shininess
		normalShininess.Bind( 1 );

		// Emission
		emission.Bind( 2 );
		break;
	}
}

// ------------------------------------------------------------------------------------ //
// Деактивировать буфер
// ------------------------------------------------------------------------------------ //
void le::GBuffer::Unbind()
{
	if ( !isInitialize ) return;
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

// ------------------------------------------------------------------------------------ //
// Показать все буферы на экране
// ------------------------------------------------------------------------------------ //
void le::GBuffer::ShowBuffers()
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glClear( GL_DEPTH_BUFFER_BIT );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, handle_frameBuffer );

	float			halfWidth = windowSize.x / 2.f;
	float			halfHeight = windowSize.y / 2.f;

	// Albedo + Specular
	glReadBuffer( GL_COLOR_ATTACHMENT0 ); 
	glBlitFramebuffer( 0, 0, windowSize.x, windowSize.y, 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	
	// Normal + Shininess
	glReadBuffer( GL_COLOR_ATTACHMENT1 ); 
	glBlitFramebuffer( 0, 0, windowSize.x, windowSize.y, 0, halfHeight, halfWidth, windowSize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	
	// Emission
	glReadBuffer( GL_COLOR_ATTACHMENT2 ); 
	glBlitFramebuffer( 0, 0, windowSize.x, windowSize.y, halfWidth, halfHeight, windowSize.x, windowSize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );
}

// ------------------------------------------------------------------------------------ //
// Инициализирован ли графический буфер
// ------------------------------------------------------------------------------------ //
bool le::GBuffer::IsInitialize() const
{
	return isInitialize;
}
