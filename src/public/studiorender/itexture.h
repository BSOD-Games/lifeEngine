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
		IF_R_8UNORM,
		IF_RGBA_8UNORM,
		IF_RGB_8UNORM,
		IF_RGBA_16FLOAT,
		IF_RGB_16FLOAT,
		IF_DEPTH24_STENCIL8		
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
		virtual void							Initialize( TEXTURE_TYPE TextureType, IMAGE_FORMAT ImageFormat, UInt32_t Width, UInt32_t Height, UInt32_t CountMipmap = 1 ) = 0;
		virtual void							Delete() = 0;
		virtual void							Bind( UInt32_t Layer = 0 ) = 0;
		virtual void							Unbind() = 0;
		virtual void							GenerateMipmaps() = 0;
		virtual void							Append( const UInt8_t* Data, UInt32_t MipmapLevel = 0 ) = 0;
		virtual void							Update( UInt32_t X, UInt32_t Y, UInt32_t Width, UInt32_t Height, const UInt8_t* Data, UInt32_t MipmapLevel = 0 ) = 0;
		virtual void							Resize( UInt32_t NewWidth, UInt32_t NewHeight, bool IsSaveData = false ) = 0;
			
		virtual void							SetSampler( const StudioRenderSampler& Sampler ) = 0;
			
		virtual bool							IsCreated() const = 0;
		virtual UInt32_t						GetWidth( UInt32_t MipmapLevel = 0 ) const = 0;
		virtual UInt32_t						GetHeight( UInt32_t MipmapLevel = 0 ) const = 0;
		virtual UInt32_t						GetCountMipmaps() const = 0;
		virtual TEXTURE_TYPE					GetType() const = 0;	
		virtual const StudioRenderSampler&		GetSampler() const = 0;	
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define TEXTURE_INTERFACE_VERSION "LE_Texture002"

//---------------------------------------------------------------------//

#endif // !ITEXTURE_H
