// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Misc/EngineGlobals.h"
#include "Logging/LogMacros.h"
#include "System/FileSystem.h"
#include "Rendering/RHI/IRHI.h"
#include "Resources/Parsers/ParsersTexture2DFactory.h"
#include "Resources/Parsers/IParserTexture2D.h"
#include "Resources/Texture2D.h"

/**
 * Constructor
 */
le::Texture2D::Texture2D()
{}

/**
 * Destructor
 */
le::Texture2D::~Texture2D()
{}

/**
 * Serialize resource
 */
bool le::Texture2D::Serialize( const Path& InPath )
{
	LIFEENGINE_LOG_WARNING( "Engine", "le::Texture2D::Serialize( const std::string& InPath ) :: Not implemented" );
	return false;
}

/**
 * Deserialize resource
 */
bool le::Texture2D::Deserialize( const Path& InPath )
{
	if ( InPath.IsEmpty() )		return false;
	LIFEENGINE_ASSERT( GRHI );

	FIParserTexture2DRef	parser = GParsersTexture2DFactory->Get( InPath.GetExtension() );
	FFileHandle				fileHandle = GFileSystem->OpenFile( InPath );
	if ( !fileHandle || !parser )
	{
		if ( fileHandle )		GFileSystem->CloseFile( fileHandle );
		return false;
	}
	
	if ( !parser->SetFile( fileHandle ) )		return false;
	uint32			countMipmaps = parser->GetCountMipmaps();
	uint32			width = parser->GetWidth();
	uint32			height = parser->GetHeight();
	EImageFormat	format = parser->GetFormat();
		
	handle = GRHI->CreateTexture2D( format, width, height, countMipmaps );
	for ( uint32 mipmap = 0; mipmap < countMipmaps; ++mipmap )
		handle->Allocate( parser->GetData( mipmap ), mipmap );
	
	if ( countMipmaps == 1 )		handle->GenerateMipmaps();

	SSampler		sampler;
	sampler.magFilter = SF_Nearest;
	sampler.minFilter = SF_Nearest;
	handle->SetSampler( sampler );

	GFileSystem->CloseFile( fileHandle );
	return true;
}

/**
 * Get type
 */
le::EResourceType le::Texture2D::GetType() const
{
	return RT_Texture2D;
}
