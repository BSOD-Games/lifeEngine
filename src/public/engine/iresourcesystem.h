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
	class IFactory;
	class ITexture;
	class IMaterial;
	class IMesh;
	class ILevel;
	class IFont;
	class IResourceSystem;
    class IMaterialManager;

	typedef		void			( *LoadImageFn_t )( const char* Path, Image& Image, bool& IsError, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels );
	typedef		ITexture*		( *LoadTextureFn_t )( const char* Path, IFactory* StudioRenderFactory );
    typedef		IMaterial*		( *LoadMaterialFn_t )( const char* Path, IResourceSystem* ResourceSystem, IMaterialManager* MaterialManager, IFactory* StudioRenderFactory );
    typedef		IMesh*			( *LoadMeshFn_t )( const char* Path, IResourceSystem* ResourceSystem, IFactory* StudioRenderFactory );
    typedef		ILevel*			( *LoadLevelFn_t )( const char* Path, IFactory* GameFactory );
	typedef		IFont*			( *LoadFontFn_t )( const char* Path );

	//---------------------------------------------------------------------//

	class IResourceSystem
	{
	public:
		virtual void					RegisterLoader_Image( const char* Format, LoadImageFn_t LoadImage ) = 0;
		virtual void					RegisterLoader_Texture( const char* Format, LoadTextureFn_t LoadTexture ) = 0;
		virtual void					RegisterLoader_Material( const char* Format, LoadMaterialFn_t LoadMaterial ) = 0;
		virtual void					RegisterLoader_Mesh( const char* Format, LoadMeshFn_t LoadMesh ) = 0;
		virtual void					RegisterLoader_Level( const char* Format, LoadLevelFn_t LoadLevel ) = 0;
		virtual void					RegisterLoader_Font( const char* Format, LoadFontFn_t LoadFont ) = 0;
		virtual void					UnregisterLoader_Image( const char* Format ) = 0;
		virtual void					UnregisterLoader_Texture( const char* Format ) = 0;
		virtual void					UnregisterLoader_Material( const char* Format ) = 0;
		virtual void					UnregisterLoader_Mesh( const char* Format ) = 0;
		virtual void					UnregisterLoader_Level( const char* Format ) = 0;
		virtual void					UnregisterLoader_Font( const char* Format ) = 0;

		virtual Image					LoadImage( const char* Path, bool& IsError, bool IsFlipVertical = false, bool IsSwitchRedAndBlueChannels = false ) = 0;		
		virtual ITexture*				LoadTexture( const char* Name, const char* Path ) = 0;
        virtual IMaterial*				LoadMaterial( const char* Name, const char* Path ) = 0;
        virtual IMesh*					LoadMesh( const char* Name, const char* Path ) = 0;
        virtual ILevel*					LoadLevel( const char* Name, const char* Path, IFactory* GameFactory ) = 0;
		virtual IFont*					LoadFont( const char* Name, const char* Path ) = 0;
		virtual void					UnloadImage( Image& Image ) = 0;
		virtual void					UnloadTexture( const char* Name ) = 0;
		virtual void					UnloadMaterial( const char* Name ) = 0;
		virtual void					UnloadMesh( const char* Name ) = 0;
		virtual void					UnloadLevel( const char* Name ) = 0;
		virtual void					UnloadFont( const char* Name ) = 0;
		virtual void					UnloadTextures() = 0;
		virtual void					UnloadMaterials() = 0;
		virtual void					UnloadMeshes() = 0;
		virtual void					UnloadLevels() = 0;
		virtual void					UnloadFonts() = 0;
		virtual void					UnloadAll() = 0;

		virtual ITexture*				GetTexture( const char* Name ) const = 0;
		virtual IMaterial*				GetMaterial( const char* Name ) const = 0;
		virtual IMesh*					GetMesh( const char* Name ) const = 0;
		virtual ILevel*					GetLevel( const char* Name ) const = 0;
		virtual IFont*					GetFont( const char* Name ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IRESOURCESYSTEM_H
