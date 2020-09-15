// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef RHITEXTURE2D_H
#define RHITEXTURE2D_H

#include "Misc/EngineDefines.h"
#include "Rendering/RHI/IRHITexture2D.h"
#include "GLState.h"

namespace le
{
	class RHITexture2D : public IRHITexture2D
	{
	public:
		/* Constructor */
		RHITexture2D( EImageFormat InImageFormat, uint32 InWidth, uint32 InHeight, uint32 InCountMipmap );

		/* Destructor */
		~RHITexture2D();

		/* Bind texture */
		FORCEINLINE void Bind( uint32 InTextureLayer = 0 )
		{
			GLState::SetTextureLayer( InTextureLayer );
			GLState::BindTexture( GL_TEXTURE_2D, handle );
		}

		/* Unbind texture */
		FORCEINLINE static void Unbind( uint32 InTextureLayer = 0 )
		{
			GLState::SetTextureLayer( InTextureLayer );
			GLState::BindTexture( GL_TEXTURE_2D, 0 );
		}

		/* Allocate data in texture */
		void Allocate( const uint8* InData, uint32 InMipmapLevel = 0 ) override;

		/* Update data in texture */
		void Update( uint32 InX, uint32 InY, uint32 InWidth, uint32 InHeight, const uint8* InData, uint32 InMipmapLevel = 0 ) override;

		/* Resize texture */
		void Resize( uint32 InNewWidth, uint32 InNewHeight, bool InIsSaveData = false ) override;

		/* Generate mipmaps */
		void GenerateMipmaps() override;

		/* Set sampler */
		void SetSampler( const SSampler& InSampler ) override;

		/* Get width */
		uint32 GetWidth( uint32 InMipmapLevel = 0 ) const override;

		/* Get height */
		uint32 GetHeight( uint32 InMipmapLevel = 0 ) const override;

		/* Get count mipmaps */
		uint32 GetCountMipmaps() const override;

		/* Get type image */
		EImageFormat GetFormat() const override;

		/* Get sampler */
		const SSampler& GetSampler() const override;

		/* Get handle */
		FORCEINLINE uint32 GetHandle() const
		{
			return handle;
		}

	private:	
		/* Apply sampler */
		void ApplySampler( const SSampler& InSampler );

		uint32				handle;
		uint32				width;
		uint32				height;
		uint32				countMipmaps;
		EImageFormat		imageFormat;
		SSampler			sampler;
	};
}

#endif // !RHITEXTURE2D_H
