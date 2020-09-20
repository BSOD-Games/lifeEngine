// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef PARSERTEXTURE2DSTBIMAGE_H
#define PARSERTEXTURE2DSTBIMAGE_H

#include <vector>

#include "Resources/Parsers/IParserTexture2D.h"

namespace le
{
	class ParserTexture2DSTBImage : public IParserTexture2D
	{
	public:
		/* Constructor */
		ParserTexture2DSTBImage();

		/* Destructor */
		~ParserTexture2DSTBImage();

		/* Set file */
		bool SetFile( FFileHandle InFileHandle ) override;

		FORCEINLINE static std::vector< std::string > GetSupportedExtensions()
		{
			return { "jpg", "png", "tga", "bmp", "psd", "gif", "hdr", "pic", "pnm" };
		}

		/* Get data */
		byte* GetData( uint32 InMipmapLevel = 0 ) const override;

		/* Get type image */
		EImageFormat GetFormat() const override;

		/* Get count mipmaps */
		uint32 GetCountMipmaps() const override;

		/* Get width */
		uint32 GetWidth() const override;

		/* Get height */
		uint32 GetHeight() const override;

	private:
		/* Clear */
		void Clear();

		EImageFormat		format;
		uint32				width;
		uint32				height;
		byte*				data;
	};
}

#endif // !PARSERTEXTURE2DSTBIMAGE_H