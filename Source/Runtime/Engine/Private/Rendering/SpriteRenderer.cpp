// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Misc/EngineGlobals.h"
#include "Math/Math.h"
#include "Rendering/RHI/IRHI.h"
#include "Rendering/RHI/IRHIGeometry.h"
#include "Rendering/RHI/IRHIVertexFormat.h"
#include "Rendering/RenderSystem.h"
#include "Rendering/SpriteRenderer.h"

struct SSpriteVertex
{
	le::FVector3D		position;
	le::FVector3D		normal;
	le::FVector2D		texCoord;
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
		{ FVector3D( x0, y0, 0.f ), FVector3D( x0, y0, 0.f ), FVector2D( 0.f, 0.f ) },
		{ FVector3D( x0, y1, 0.f ), FVector3D( x0, y1, 0.f ), FVector2D( 0.f, 1.f ) },
		{ FVector3D( x1, y1, 0.f ), FVector3D( x1, y1, 0.f ), FVector2D( 1.f, 1.f ) },
		{ FVector3D( x1, y0, 0.f ), FVector3D( x1, y0, 0.f ), FVector2D( 1.f, 0.f ) }
	};

	uint32			indeces[] =	{ 3, 2, 1, 0 };

	// Creating geometry for render sprite
	rhiGeometry = GRHI->CreateGeometry();
	rhiVertexFormat = GRHI->CreateVertexFormat();
	rhiVertexBuffer = GRHI->CreateBuffer( BT_Vertex );
	rhiIndexBuffer = GRHI->CreateBuffer( BT_Index );

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
void le::SpriteRenderer::Render( const SSpriteRenderObject& InSpriteRenderObject, CameraComponent* InCameraComponent )
{
	GRHI->SetGeometry( rhiGeometry );
	InSpriteRenderObject.material->Refresh();

	BaseShader*			shader = InSpriteRenderObject.material->GetShader();
	if ( shader )
	{
		shader->OnDrawSprite( GRHI, InSpriteRenderObject.size, InSpriteRenderObject.position, InCameraComponent );
		GRHI->DrawIndexed( DO_TriangleFan, 0, 4, 0, 4 );
	}
}
