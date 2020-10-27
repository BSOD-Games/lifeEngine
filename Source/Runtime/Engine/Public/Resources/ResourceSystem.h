// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef RESOURCESYSTEM_H
#define RESOURCESYSTEM_H

#include <string>
#include <memory>
#include <unordered_map>

#include "Misc/Object.h"
#include "Resource.h"

namespace le
{
	class ResourceSystem : public Object
	{
	public:
		/* Constructor */
		ResourceSystem();
		
		/* Destructor */
		~ResourceSystem();

		/* Initialize resource system */
		bool Initialize();

		/* Find resource in cahce. If not found - loading from FileSystem */
		FResourceRef FindResource( const Path& InPath, EResourceType InResourceType );

		/* Find default resource */
		FResourceRef FindDefaultResource( EResourceType InResourceType );

		/* Unload resource */
		void UnloadResource( FResourceConstRef& InResource );

		/* Unload all resources */
		void UnloadResources();

	private:
		std::unordered_map< std::string, FResourceRef >			resources;
		std::unordered_map< EResourceType, FResourceRef >		defaultResources;
	};
}

#endif // !RESOURCESYSTEM_H
