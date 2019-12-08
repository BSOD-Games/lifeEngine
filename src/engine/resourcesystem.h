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

#include "engine/iresourcesystem.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ResourceSystem : public IResourceSystem
	{
	public:
		// IResourceSystem
		virtual void					RegisterParser( const char* Format, LoadImageFn_t LoadImage );
		virtual Image*					LoadImage( const char* Name, const char* Group, const char* Path, bool IsFlipVertical = false, bool IsSwitchRedAndBlueChannels = false );
		virtual void					DeleteImage( const char* Name, const char* Group );
		virtual void					DeleteImages( const char* Group );
		virtual void					DeleteImages();
		virtual void					DeleteAll();

		virtual Image*					GetImage( const char* Name, const char* Group );

		// ResourceSystem
		ResourceSystem();
		~ResourceSystem();

	private:
		typedef			std::unordered_map< std::string, std::unordered_map< std::string, Image > >			ImageMap_t;

		std::unordered_map< std::string, LoadImageFn_t >		parserImages;
		ImageMap_t												images;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !RESOURCESYSTEM_H