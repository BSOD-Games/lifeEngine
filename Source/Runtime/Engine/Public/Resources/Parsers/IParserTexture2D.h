// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IPARSERTEXTURE2D_H
#define IPARSERTEXTURE2D_H

#include <string>
#include <vector>

#include "Rendering/RHI/IRHITexture2D.h"
#include "System/FileSystem.h"
#include "IParser.h"

namespace le
{
	class IParserTexture2D : public IParser
	{
	public:
		/* Destructor */
		virtual ~IParserTexture2D() {}

		/* Read */
		virtual bool Read( std::vector< uint8 >& OutData, uint32 InMipmapLevel = 0 ) const = 0;

		/* Set file */
		virtual void SetFile( FFileHandle* InFileHandle ) = 0;

		/* Get type image */
		virtual EImageFormat GetFormat() const = 0;

		/* Get count mipmaps */
		virtual uint32 GetCountMipmaps() const = 0;

		/* Get width */
		virtual uint32 GetWidth() const = 0;

		/* Get height */
		virtual uint32 GetHeight() const = 0;
	};
}

#endif // !IPARSERTEXTURE2D_H
