// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Logging/LogMacros.h"
#include "Rendering/RHI/IRHIShader.h"
#include "Rendering/RHI/IRHIBuffer.h"
#include "Rendering/RHI/IRHIVertexFormat.h"
#include "Rendering/RHI/IRHIGeometry.h"
#include "Rendering/RHI/IRHITexture2D.h"
#include "Rendering/RenderSystem.h"
#include "System/FileSystem.h"
#include "OpenGL4RHI.h"

le::IRHIShader*			shader;
le::IRHIBuffer*			vertexBuffer;
le::IRHIVertexFormat*	vertexFormat;
le::IRHIGeometry*		geometry;
le::IRHITexture2D*		texture2d;

/**
 * Constructor
 */
le::RenderSystem::RenderSystem() :
	rhi( nullptr ),
	currentContext( nullptr )
{}

/**
 * Destructor
 */
le::RenderSystem::~RenderSystem()
{
	if ( rhi )
	{
		if ( currentContext )		rhi->DeleteContext( currentContext );
		delete rhi;
	}
}

/**
 * Initialize render system
 */
bool le::RenderSystem::Initialize()
{
	rhi = new OpenGL4RHI();
	return true;
}

/**
 * Create render context
 */
le::FRHIContext le::RenderSystem::CreateContext( FWindowHandle InWindowHandle, FRHIContext InShareContext )
{
	return rhi->CreateContext( InWindowHandle, InShareContext );
}

/**
 * Make current context
 */
bool le::RenderSystem::MakeCurrentContext( FRHIContext InRHIContext )
{
	if ( currentContext == InRHIContext )
		return true;

	if ( rhi->MakeCurrentContext( InRHIContext ) )
	{
		currentContext = InRHIContext;
		
		shader = rhi->CreateShader();
		vertexBuffer = rhi->CreateBuffer( BT_Vertex );
		vertexFormat = rhi->CreateVertexFormat();
		geometry = rhi->CreateGeometry();
		texture2d = rhi->CreateTexture2D( IF_R_8Unorm, 1, 1 );

		std::string				buffer, errorLog;
		le::FFileHandle			file_vShader = le::FileSystem::GetInstance()->OpenFile( "Content/vertex.glsl", true );
		if ( !file_vShader )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Content/vertex.glsl: not found" );
			return false;
		}

		le::FileSystem::GetInstance()->ReadLineFromFile( file_vShader, buffer, '\0' );
		if ( !shader->Compile( buffer, ST_Vertex, nullptr, &errorLog ) )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Vertex shader error: %s", errorLog.c_str() );
			return false;
		}

		le::FileSystem::GetInstance()->CloseFile( file_vShader );
		le::FFileHandle			file_pShader = le::FileSystem::GetInstance()->OpenFile( "Content/pixel.glsl", true );
		if ( !file_pShader )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Content/pixel.glsl: not found" );
			return false;
		}

		le::FileSystem::GetInstance()->ReadLineFromFile( file_pShader, buffer, '\0' );
		if ( !shader->Compile( buffer, ST_Pixel, nullptr, &errorLog ) )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Pixel shader error: %s", errorLog.c_str() );
			return false;
		}

		le::FileSystem::GetInstance()->CloseFile( file_pShader );
		if ( !shader->Link( &errorLog ) )
		{
			LIFEENGINE_LOG_ERROR( "Engine", "Link shader error: %s", errorLog.c_str() );
			return false;
		}

		uint8 pixel = 255;
		texture2d->Allocate( &pixel );

		float		verteces[] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.0f,  0.5f, 0.0f, 0.0f, 0.0f
		};

		vertexBuffer->Allocate( verteces, sizeof( verteces ) );
		vertexFormat->PushFloat( 3 );
		vertexFormat->PushFloat( 2 );
		geometry->SetVertexBuffer( vertexBuffer );
		geometry->SetVertexFormat( vertexFormat );

		return true;
	}

	return false;
}

/**
 * Delete context
 */
void le::RenderSystem::DeleteContext( FRHIContext InRHIContext )
{
	rhi->DeleteContext( InRHIContext );
}

/**
 * Start render scene
 */
void le::RenderSystem::Begin()
{

}

/**
 * End render scene
 */
void le::RenderSystem::End()
{}

/**
 * Present scent
 */
void le::RenderSystem::Present()
{
	if ( !currentContext ) return;
	
	rhi->SetShader( shader );
	rhi->SetGeometry( geometry );
	rhi->SetTexture2D( texture2d );
	rhi->Draw( DO_TriangleList, 0, 3 );
	rhi->SetGeometry( nullptr );

	rhi->SwapBuffers( currentContext );
}

/**
 * Set vertical sync
 */
void le::RenderSystem::SetVerticalSync( bool InIsEnable )
{
	rhi->SetVerticalSync( InIsEnable );
}

/**
 * Set viewport
 */
void le::RenderSystem::SetViewport( uint32 InX, uint32 InY, uint32 InWidth, uint32 InHeight )
{
	rhi->SetViewport( InX, InY, InWidth, InHeight );
}
