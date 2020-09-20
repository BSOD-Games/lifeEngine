// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef RESOURCESYSTEM_H
#define RESOURCESYSTEM_H

#include <string>
#include <memory>
#include <unordered_map>

#include "Misc/Object.h"
#include "Resource.h"

namespace le
{
	class Resource;

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
		Resource* FindResource( const Path& InPath, EResourceType InResourceType );

		/* Find default resource */
		Resource* FindDefaultResource( EResourceType InResourceType );

		/* Unload resource */
		void UnloadResource( Resource*& InResource );

		/* Unload all resources */
		void UnloadResources();

	private:
		std::unordered_map< std::string, Resource* >		resources;
		std::unordered_map< EResourceType, Resource* >		defaultResources;
	};
}

#endif // !RESOURCESYSTEM_H
