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

	private:
		/* On resource delete */
		void OnResourceDelete( const Resource::EventDelete& InEvent );

		std::unordered_map< std::string, Resource* >			resources;
		std::unordered_map< EResourceType, Resource* >		defaultResources;
	};
}

#endif // !RESOURCESYSTEM_H
