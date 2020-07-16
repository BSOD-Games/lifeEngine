//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <FreeImage.h>

#include "global.h"
#include "consolesystem.h"
#include "parserimage_freeimage.h"

// ------------------------------------------------------------------------------------ //
// Release
// ------------------------------------------------------------------------------------ //
void le::ParserImageFreeImage::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get version
// ------------------------------------------------------------------------------------ //
const char* le::ParserImageFreeImage::GetVersion() const
{
	return FreeImage_GetVersion();
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::ParserImageFreeImage::GetName() const
{
	return "FreeImage";
}

// ------------------------------------------------------------------------------------ //
// Get count file extensions
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ParserImageFreeImage::GetCountFileExtensions() const
{
	return 3;
}

// ------------------------------------------------------------------------------------ //
// Get file extensions
// ------------------------------------------------------------------------------------ //
const char** le::ParserImageFreeImage::GetFileExtensions() const
{
	static const char*			extensions[ 4 ] = 
	{ 
		"png", "jpg", "tga", "ico" 
	};

	return extensions;
}

// ------------------------------------------------------------------------------------ //
// Get author
// ------------------------------------------------------------------------------------ //
const char* le::ParserImageFreeImage::GetAuthor() const
{
	return "Egor Pogulyaka";
}

// ------------------------------------------------------------------------------------ //
// Read
// ------------------------------------------------------------------------------------ //
void le::ParserImageFreeImage::Read( const char* Path, Image& Image, bool& IsError, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels )
{
	IsError = false;
	FREE_IMAGE_FORMAT		imageFormat = FIF_UNKNOWN;
	imageFormat = FreeImage_GetFileType( Path, 0 );

	if ( imageFormat == FIF_UNKNOWN )
		imageFormat = FreeImage_GetFIFFromFilename( Path );

	FIBITMAP* bitmap = FreeImage_Load( imageFormat, Path, 0 );
	if ( !bitmap )
	{
		IsError = true;
		g_consoleSystem->PrintError( "Failed open image" );
		return;
	}

	if ( IsFlipVertical )				FreeImage_FlipVertical( bitmap );
	if ( IsSwitchRedAndBlueChannels )
	{
		auto		red = FreeImage_GetChannel( bitmap, FREE_IMAGE_COLOR_CHANNEL::FICC_RED );
		auto		blue = FreeImage_GetChannel( bitmap, FREE_IMAGE_COLOR_CHANNEL::FICC_BLUE );

		FreeImage_SetChannel( bitmap, blue, FREE_IMAGE_COLOR_CHANNEL::FICC_RED );
		FreeImage_SetChannel( bitmap, red, FREE_IMAGE_COLOR_CHANNEL::FICC_BLUE );

		FreeImage_Unload( red );
		FreeImage_Unload( blue );
	}

	le::UInt8_t* tempData = FreeImage_GetBits( bitmap );
	Image.width = FreeImage_GetWidth( bitmap );
	Image.height = FreeImage_GetHeight( bitmap );
	Image.depth = FreeImage_GetBPP( bitmap );
	Image.pitch = FreeImage_GetPitch( bitmap );

	Image.data = ( le::UInt8_t* ) malloc( Image.pitch * Image.height );
	memcpy( Image.data, tempData, Image.pitch * Image.height );

	Image.rMask = 0x00ff0000;
	Image.gMask = 0x0000ff00;
	Image.bMask = 0x000000ff;
	Image.aMask = ( Image.depth == 24 ) ? 0 : 0xff000000;

	FreeImage_Unload( bitmap );
	return;
}
