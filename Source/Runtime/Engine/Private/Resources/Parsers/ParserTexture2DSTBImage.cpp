// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Misc/EngineGlobals.h"
#include "Logging/LogMacros.h"
#include "System/FileSystem.h"
#include "ParserTexture2DSTBImage.h"

/**
 * Read data from file
 */
int FS_Read( void* InUser, char* InData, int InSize )
{
	LIFEENGINE_ASSERT( InUser );
	return static_cast< int >( le::GFileSystem->ReadFromFile( InUser, ( le::byte* ) InData, InSize ) );
}

/**
 * Skip data in file
 */
void FS_Skip( void* InUser, int InSkipBytes )
{
	LIFEENGINE_ASSERT( InUser );
	le::GFileSystem->SetOffsetInFile( InUser, InSkipBytes, le::FO_Current );
}

/**
 * Is end of file
 */
int FS_EoF( void* InUser )
{
	LIFEENGINE_ASSERT( InUser );
	return le::GFileSystem->IsEOFFile( InUser );
}

/**
 * Constructor
 */
le::ParserTexture2DSTBImage::ParserTexture2DSTBImage() :
	width( 0 ),
	height( 0 ),
	data( nullptr )
{}

/**
 * Destructor
 */
le::ParserTexture2DSTBImage::~ParserTexture2DSTBImage()
{
	Clear();
}

/**
 * Set file
 */
bool le::ParserTexture2DSTBImage::SetFile( FFileHandle InFileHandle )
{
	if ( data )		Clear();

	stbi_io_callbacks		stbiIOCallbacks;
	stbiIOCallbacks.read = FS_Read;
	stbiIOCallbacks.skip = FS_Skip;
	stbiIOCallbacks.eof = FS_EoF;

	int		channels = 0;
	stbi_set_flip_vertically_on_load( true );
	data = stbi_load_from_callbacks( &stbiIOCallbacks, InFileHandle, ( int* ) &width, ( int* ) &height, &channels, STBI_default );

	switch ( channels )
	{
	case 1:		format = IF_R_8Unorm; break;
	case 2:		LIFEENGINE_LOG_WARNING( "Engine", "Unsupported image format with two channels" ); break;
	case 3:		format = IF_RGB_8Unorm; break;
	case 4:		format = IF_RGBA_8Unorm; break;
	}

	return data ? true : false;
}

/**
 * Clear
 */
void le::ParserTexture2DSTBImage::Clear()
{
	if ( !data ) return;

	stbi_image_free( data );
	data = nullptr;
	width = height = 0;
}

/**
 * Get data
 */
le::uint8* le::ParserTexture2DSTBImage::GetData( uint32 InMipmapLevel ) const
{
	return data;
}

/**
 * Get format
 */
le::EImageFormat le::ParserTexture2DSTBImage::GetFormat() const
{
	return format;
}

/**
 * Get count mipmaps
 */
le::uint32 le::ParserTexture2DSTBImage::GetCountMipmaps() const
{
	return 1;
}

/**
 * Get width
 */
le::uint32 le::ParserTexture2DSTBImage::GetWidth() const
{
	return width;
}

/**
 * Get height
 */
le::uint32 le::ParserTexture2DSTBImage::GetHeight() const
{
	return height;
}
