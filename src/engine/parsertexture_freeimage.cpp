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

#include "common/image.h"
#include "studiorender/itexture.h"
#include "studiorender/studiorendersampler.h"

#include "global.h"
#include "consolesystem.h"
#include "parserimage_freeimage.h"
#include "parsertexture_freeimage.h"

// ------------------------------------------------------------------------------------ //
// Get version
// ------------------------------------------------------------------------------------ //
const char* le::ParserTextureFreeImage::GetVersion() const
{
	return FreeImage_GetVersion();
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::ParserTextureFreeImage::GetName() const
{
	return "FreeImage";
}

// ------------------------------------------------------------------------------------ //
// Get count file extensions
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ParserTextureFreeImage::GetCountFileExtensions() const
{
	ParserImageFreeImage		parserImageFreeImage;
	return parserImageFreeImage.GetCountFileExtensions();
}

// ------------------------------------------------------------------------------------ //
// Get file extensions
// ------------------------------------------------------------------------------------ //
const char** le::ParserTextureFreeImage::GetFileExtensions() const
{
	ParserImageFreeImage		parserImageFreeImage;
	return parserImageFreeImage.GetFileExtensions();
}

// ------------------------------------------------------------------------------------ //
// Get author
// ------------------------------------------------------------------------------------ //
const char* le::ParserTextureFreeImage::GetAuthor() const
{
	return "Egor Pogulyaka";
}

// ------------------------------------------------------------------------------------ //
// Read
// ------------------------------------------------------------------------------------ //
le::ITexture* le::ParserTextureFreeImage::Read( const char* Path, IFactory* StudioRenderFactory )
{
	bool				isError = false;
	Image				image;

	ParserImageFreeImage		parserImageFreeImage;
	parserImageFreeImage.Read( Path, image, isError, false, true );
	if ( isError )
	{
		g_consoleSystem->PrintError( "Failed loading texture" );
		return nullptr;
	}

	ITexture*		texture = ( ITexture* ) StudioRenderFactory->Create( TEXTURE_INTERFACE_VERSION );
	if ( !texture )
	{
		g_consoleSystem->PrintError( "Interface le::ITexture [%s] not founded in studiorender", TEXTURE_INTERFACE_VERSION );
		return nullptr;
	}

	texture->Initialize( TT_2D, image.aMask > 0 ? IF_RGBA_8UNORM : IF_RGB_8UNORM, image.width, image.height );
	texture->Bind();
	texture->Append( image.data );
	texture->GenerateMipmaps();

	StudioRenderSampler			sampler;
	sampler.minFilter = SF_LINEAR_MIPMAP_LINEAR;
	sampler.magFilter = SF_LINEAR;
	texture->SetSampler( sampler );

	texture->Unbind();

	delete[] image.data;
	return texture;
}
