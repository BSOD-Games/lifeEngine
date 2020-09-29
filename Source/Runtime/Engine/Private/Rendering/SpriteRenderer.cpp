// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Math/Vector2D.h"
#include "Math/Vector3D.h"
#include "Rendering/RHI/IRHI.h"
#include "Rendering/RHI/IRHIGeometry.h"
#include "Rendering/RHI/IRHIVertexFormat.h"
#include "Rendering/RenderSystem.h"
#include "Rendering/SpriteRenderer.h"

struct SSpriteVertex
{
	le::SVector3D		position;
	le::SVector3D		normal;
	le::SVector2D		texCoord;
};

/**
 * Constructor
 */
le::SpriteRenderer::SpriteRenderer() :
	rhiGeometry( nullptr ),
	rhiIndexBuffer( nullptr ),
	rhiVertexBuffer( nullptr ),
	rhiVertexFormat( nullptr )
{}

/**
 * Initialize renderer
 */
bool le::SpriteRenderer::Initialize()
{
	float	x0 = -1.f,	y0 = -1.f;
	float	x1 = 1.f,	y1 = 1.f;

	SSpriteVertex		verteces[] =
	{
		{ SVector3D( x0, y0, 1.f ), SVector3D( x0, y0, 0.f ), SVector2D( 0.f, 0.f ) },
		{ SVector3D( x0, y1, 1.0f ), SVector3D( x0, y1, 0.0f ), SVector2D( 0.0f, 1.0f ) },
		{ SVector3D( x1, y1, 1.0f ), SVector3D( x1, y1, 0.0f ), SVector2D( 1.0f, 1.0f ) },
		{ SVector3D( x1, y0, 1.0f ), SVector3D( x1, y0, 0.0f ), SVector2D( 1.0f, 0.0f ) }
	};

	uint32			indeces[] =	{ 3, 2, 1, 0 };

	// Creating geometry for render sprite
	rhiGeometry = GRenderSystem->rhi->CreateGeometry();
	rhiVertexFormat = GRenderSystem->rhi->CreateVertexFormat();
	rhiVertexBuffer = GRenderSystem->rhi->CreateBuffer( BT_Vertex );
	rhiIndexBuffer = GRenderSystem->rhi->CreateBuffer( BT_Index );

	rhiVertexBuffer->Allocate( verteces, sizeof( verteces ) );
	rhiIndexBuffer->Allocate( indeces, sizeof( indeces ) );
	rhiVertexFormat->PushFloat( 3 );
	rhiVertexFormat->PushFloat( 3 );
	rhiVertexFormat->PushFloat( 2 );

	rhiGeometry->SetVertexBuffer( rhiVertexBuffer );
	rhiGeometry->SetVertexFormat( rhiVertexFormat );
	rhiGeometry->SetIndexBuffer( rhiIndexBuffer, IT_UInt );
	return true;
}

/**
 * Render sprite
 */
void le::SpriteRenderer::Render( const SSpriteRenderObject& InSpriteRenderObject )
{
	GRenderSystem->rhi->SetGeometry( rhiGeometry );	
	InSpriteRenderObject.material->Refresh();

	BaseShader*			shader = InSpriteRenderObject.material->GetShader();
	if ( shader )
	{
		shader->OnDrawSprite( GRenderSystem->rhi, InSpriteRenderObject.size );
		GRenderSystem->rhi->DrawIndexed( DO_TriangleFan, 0, 4, 0, 4 );
	}
}
