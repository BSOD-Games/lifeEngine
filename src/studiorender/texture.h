//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef TEXTURE_H
#define TEXTURE_H

#include "studiorender/itexture.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Texture : public ITexture
	{
	public:
		// ITexture
		virtual void				Initialize( TEXTURE_TYPE TextureType, IMAGE_FORMAT ImageFormat, UInt32_t Width, UInt32_t Height, UInt32_t CountMipmap = 1 );
		virtual void				Delete();
		virtual void				Bind( UInt32_t Layer = 0 );
		virtual void				Unbind();
		virtual void				GenerateMipmaps();
		virtual void				Append( const UInt8_t* Data, UInt32_t MipmapLevel = 0 );
		virtual void				Update( UInt32_t X, UInt32_t Y, UInt32_t Width, UInt32_t Height, const UInt8_t* Data, UInt32_t MipmapLevel = 0 );

		virtual void				SetSampler( const StudioRenderSampler& Sampler );

		virtual bool				IsCreated() const;
		virtual UInt32_t			GetWidth( UInt32_t MipmapLevel = 0 ) const;
		virtual UInt32_t			GetHeight( UInt32_t MipmapLevel = 0 ) const;
		virtual UInt32_t			GetCountMipmaps() const;
		virtual TEXTURE_TYPE		GetType() const;

		// Texture
		Texture();
		~Texture();

	private:
		bool				isCreated;

		UInt32_t			handle;
		UInt32_t			width;
		UInt32_t			height;
		UInt32_t			layer;
		UInt32_t			countMipmaps;
		IMAGE_FORMAT		imageFormat;
		TEXTURE_TYPE		type;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !TEXTURE_H
