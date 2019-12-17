//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct StudioRenderSampler;

	//---------------------------------------------------------------------//

	enum IMAGE_FORMAT
	{
		IF_RGBA,
		IF_RGB
	};

	//---------------------------------------------------------------------//

	enum TEXTURE_TYPE
	{
		TT_2D
	};

	//---------------------------------------------------------------------//


	//---------------------------------------------------------------------//

	class ITexture
	{
	public:
		virtual void				Initialize( TEXTURE_TYPE TextureType, IMAGE_FORMAT ImageFormat, UInt32_t Width, UInt32_t Height, UInt32_t CountMipmap = 1 ) = 0;
		virtual void				Delete() = 0;
		virtual void				Bind( UInt32_t Layer = 0 ) = 0;
		virtual void				Unbind() = 0;
		virtual void				GenerateMipmaps() = 0;
		virtual void				Append( const UInt8_t* Data, UInt32_t MipmapLevel = 0 ) = 0;
		virtual void				Update( UInt32_t X, UInt32_t Y, UInt32_t Width, UInt32_t Height, const UInt8_t* Data, UInt32_t MipmapLevel = 0 ) = 0;

		virtual void				SetSampler( const StudioRenderSampler& Sampler ) = 0;

		virtual bool				IsCreated() const = 0;
		virtual UInt32_t			GetWidth( UInt32_t MipmapLevel = 0 ) const = 0;
		virtual UInt32_t			GetHeight( UInt32_t MipmapLevel = 0 ) const = 0;
		virtual UInt32_t			GetCountMipmaps() const = 0;
		virtual TEXTURE_TYPE		GetType() const = 0;		
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define TEXTURE_INTERFACE_VERSION "LE_Texture001"

//---------------------------------------------------------------------//

#endif // !ITEXTURE_H
