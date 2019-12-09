//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef RESOURCESYSTEM_H
#define RESOURCESYSTEM_H

#include <string>
#include <unordered_map>

#include "engine/iresourcesysteminternal.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct GameInfo;
	class IFactory;

	//---------------------------------------------------------------------//

	class ResourceSystem : public IResourceSystemInternal
	{
	public:
		// IResourceSystem
		virtual void					RegisterParser_Image( const char* Format, LoadImageFn_t LoadImage );
		virtual void					RegisterParser_Texture( const char* Format, LoadTextureFn_t LoadTexture );
		virtual void					UnregisterParser_Image( const char* Format );
		virtual void					UnregisterParser_Texture( const char* Format );

		virtual Image					LoadImage( const char* Path, bool& IsError, bool IsFlipVertical = false, bool IsSwitchRedAndBlueChannels = false );
		virtual ITexture*				LoadTexture( const char* Name, const char* Group, const char* Path );
		virtual void					UnloadImage( Image& Image );
		virtual void					UnloadTexture( const char* Name, const char* Group );
		virtual void					UnloadTextures( const char* Group );
		virtual void					UnloadTextures();
		virtual void					UnloadAll();

		virtual ITexture*				GetTexture( const char* Name, const char* Group ) const;

		// IResourceSystemInternal
		virtual bool					Initialize( IEngine* Engine );

		virtual void					SetGameDir( const char* GameDir );

		// ResourceSystem
		ResourceSystem();
		~ResourceSystem();

	private:
		typedef			std::unordered_map< std::string, std::unordered_map< std::string, ITexture* > >			TextureMap_t;

		IFactory*												studioRenderFactory;
		std::string												gameDir;
		std::unordered_map< std::string, LoadImageFn_t >		parserImages;
		std::unordered_map< std::string, LoadTextureFn_t >		parserTextures;
		TextureMap_t											textures;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !RESOURCESYSTEM_H