//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IRESOURCESYSTEM_H
#define IRESOURCESYSTEM_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct Image;
	class ITexture;
	class IMaterial;

	typedef		void			( *LoadImageFn_t )( const char* Path, Image& Image, bool& IsError, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels );
	typedef		ITexture*		( *LoadTextureFn_t )( const char* Path, IFactory* StudioRenderFactory );
	typedef		IMaterial*		( *LoadMaterialFn_t )( const char* Path, IResourceSystem* ResourceSystem, IFactory* MaterialSystemFactory );

	//---------------------------------------------------------------------//

	class IResourceSystem
	{
	public:
		virtual void					RegisterLoader_Image( const char* Format, LoadImageFn_t LoadImage ) = 0;
		virtual void					RegisterLoader_Texture( const char* Format, LoadTextureFn_t LoadTexture ) = 0;
		virtual void					RegisterLoader_Material( const char* Format, LoadMaterialFn_t LoadMaterial ) = 0;
		virtual void					UnregisterLoader_Image( const char* Format ) = 0;
		virtual void					UnregisterLoader_Texture( const char* Format ) = 0;
		virtual void					UnregisterLoader_Material( const char* Format ) = 0;

		virtual Image					LoadImage( const char* Path, bool& IsError, bool IsFlipVertical = false, bool IsSwitchRedAndBlueChannels = false ) = 0;		
		virtual ITexture*				LoadTexture( const char* Name, const char* Path ) = 0;
		virtual IMaterial*				LoadMaterial( const char* Name, const char* Path ) = 0;
		virtual void					UnloadImage( Image& Image ) = 0;
		virtual void					UnloadTexture( const char* Name ) = 0;
		virtual void					UnloadMaterial( const char* Name ) = 0;
		virtual void					UnloadMaterials() = 0;
		virtual void					UnloadTextures() = 0;
		virtual void					UnloadAll() = 0;

		virtual ITexture*				GetTexture( const char* Name ) const = 0;
		virtual IMaterial*				GetMaterial( const char* Name ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IRESOURCESYSTEM_H

