// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef RESOURCESYSTEM_H
#define RESOURCESYSTEM_H

#include <string>
#include <memory>
#include <unordered_map>

#include "System/Object.h"
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

		/* Find resource in cahce. If not found - loading from FileSystem */
		Resource* FindResource( const std::string& InPath, EResourceType InResourceType );

		/* Unload resource */
		void UnloadResource( Resource*& InResource );

		/* Unload all resources */
		void UnloadResources();

	private:
		std::unordered_map< std::string, Resource* >		resources;
	};
}

#endif // !RESOURCESYSTEM_H
