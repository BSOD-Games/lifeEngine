// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Misc/EngineInternalGlobals.h"
#include "Rendering/RHI/IRHI.h"
#include "Rendering/RHI/IRHIShader.h"
#include "Rendering/ShaderManager.h"
#include "Resources/Texture2D.h"
#include "World/Components/CameraComponent.h"
#include "UnlitGeneric.h"

/**
 * Constructor
 */
le::UnlitGeneric::UnlitGeneric() :
	flags( 0 ),
	baseTexture( nullptr ),
	rhiShader( nullptr )
{}

/**
 * Destructor
 */
le::UnlitGeneric::~UnlitGeneric()
{
	ClearVars();
}

/**
 * Initialize shader
 */
bool le::UnlitGeneric::Initialize( const std::vector< ShaderVar >* InShaderVars )
{
	ClearVars();
	
	SShaderPaths		shaderPaths;
	shaderPaths.vertexShaderPath = "Engine/Content/Shaders/UnlitGeneric_Vertex.glsl";
	shaderPaths.pixelShaderPath = "Engine/Content/Shaders/UnlitGeneric_Pixel.glsl";
	
	if ( InShaderVars )
		for ( uint32 index = 0, count = static_cast< uint32 >( InShaderVars->size() ); index < count; ++index )
		{
			ShaderVar		shaderVar = InShaderVars->at( index );
			if ( !shaderVar.IsDefined() )		continue;

			switch ( shaderVar.GetType() )
			{
			case SVT_Texture2D:
				if ( shaderVar.GetName() == "BaseTexture" )
				{
					baseTexture = shaderVar.GetValueTexture2D();
					baseTexture->AddRef();
				}
				break;
			}
		}

	rhiShader = GShaderManager->FindShader( "UnlitGeneric", shaderPaths );
	if ( !rhiShader )
	{
		ClearVars();
		return false;
	}

	return true;
}

/**
 * On draw sprite
 */
void le::UnlitGeneric::OnDrawSprite( IRHI* InRHI, const FVector2D& InSize, const FSRectFloat& InTextureRect, const FVector3D& InPosition, CameraComponent* InCameraComponent )
{
	if ( !rhiShader )		return;
	if ( baseTexture )		InRHI->SetTexture2D( baseTexture->GetHandle() );

	InRHI->SetShader( rhiShader );
	if ( InCameraComponent )		rhiShader->SetUniform( "PVTMatrix", InCameraComponent->GetProjectionMatrix() * InCameraComponent->GetViewMatrix() );
	rhiShader->SetUniform( "baseTexture", 0 );
	rhiShader->SetUniform( "position", InPosition );
	rhiShader->SetUniform( "spriteSize", InSize );
	rhiShader->SetUniform( "textureRect", InTextureRect );
}

/**
 * Clear vars
 */
void le::UnlitGeneric::ClearVars()
{
	if ( baseTexture )
	{
		baseTexture->ReleaseRef();
		baseTexture = nullptr;
	}

	if ( rhiShader )
	{
		rhiShader->ReleaseRef();
		rhiShader = nullptr;

		GShaderManager->UnloadShader( "UnlitGeneric", flags );
	}

	flags = 0;
}

/**
 * Get name
 */
std::string le::UnlitGeneric::GetName() const
{
	return "UnlitGeneric";
}

/**
 * Get fallback shader
 */
std::string le::UnlitGeneric::GetFallbackShader() const
{
	return "";
}
