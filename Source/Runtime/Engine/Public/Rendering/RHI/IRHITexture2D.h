// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef IRHITEXTURE2D_H
#define IRHITEXTURE2D_H

#include "Misc/Types.h"

namespace le
{
	enum EImageFormat
	{		
		IF_RGBA_8Unorm,
		IF_RGB_8Unorm,
		IF_R_8Unorm,
		IF_RGBA_16Float,
		IF_RGB_16Float,
		IF_Depth24_Stencil8
	};

	enum ESamplerFilter
	{
		SF_Nearest,
		SF_Linear,
		SF_NearestMipmapNearest,
		SF_LinearMipmapNearest,
		SF_NearestMipmapLinear,
		SF_LinearMipmapLinear
	};

	enum ESamplerAddressMode
	{
		SAM_Repeat,
		SAM_Mirror,
		SAM_Clamp,
		SAM_Border
	};

	struct SSampler
	{
		/* Constructor */
		SSampler() :
			minFilter( SF_Nearest ),
			magFilter( SF_Nearest ),
			addressU( SAM_Repeat ),
			addressV( SAM_Repeat ),
			addressW( SAM_Repeat ),
			minLod( 0 ),
			maxLod( UINT32_MAX )
		{
			memset( borderColor, 0, 4 * sizeof( float ) );
		}

		ESamplerFilter				minFilter;
		ESamplerFilter				magFilter;
		ESamplerAddressMode			addressU;
		ESamplerAddressMode			addressV;
		ESamplerAddressMode			addressW;
		float						borderColor[ 4 ];
		uint32						minLod;
		uint32						maxLod;
	};

	class IRHITexture2D
	{
	public:
		/* Constructor */
		virtual ~IRHITexture2D() {}

		/* Allocate data in texture */
		virtual void Allocate( const uint8* InData, uint32 InMipmapLevel = 0 ) = 0;

		/* Update data in texture */
		virtual void Update( uint32 InX, uint32 InY, uint32 InWidth, uint32 InHeight, const uint8* InData, uint32 InMipmapLevel = 0 ) = 0;
	
		/* Resize texture */
		virtual void Resize( uint32 InNewWidth, uint32 InNewHeight, bool InIsSaveData = false ) = 0;

		/* Generate mipmaps */
		virtual void GenerateMipmaps() = 0;

		/* Set sampler */
		virtual void SetSampler( const SSampler& InSampler ) = 0;

		/* Get width */
		virtual uint32 GetWidth( uint32 InMipmapLevel = 0 ) const = 0;

		/* Get height */
		virtual uint32 GetHeight( uint32 InMipmapLevel = 0 ) const = 0;

		/* Get count mipmaps */
		virtual uint32 GetCountMipmaps() const = 0;

		/* Get type image */
		virtual EImageFormat GetFormat() const = 0;

		/* Get sampler */
		virtual const SSampler& GetSampler() const = 0;
	};
}

#endif // !IRHITEXTURE2D_H
