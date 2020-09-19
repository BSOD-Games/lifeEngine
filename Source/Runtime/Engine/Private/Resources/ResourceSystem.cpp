// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Misc/EngineDefines.h"
#include "Logging/LogMacros.h"
#include "Resources/ResourceSystem.h"

// Resources
#include "Resources/Texture2D.h"

/**
 * Constructor
 */
le::ResourceSystem::ResourceSystem()
{}

/**
 * Destructor
 */
le::ResourceSystem::~ResourceSystem()
{
	UnloadResources();
}

/**
 * Find resource
 */
le::Resource* le::ResourceSystem::FindResource( const std::string& InPath, EResourceType InResourceType )
{
	auto		it = resources.find( InPath );
	if ( it != resources.end() )		return it->second;

	Resource*		resource = nullptr;
	switch ( InResourceType )
	{
	case RT_Texture2D:
		resource = new Texture2D();
		break;

	default:	return nullptr;
	}

	if ( !resource->Deserialize( InPath ) )
	{
		delete resource;
		return nullptr;
	}

	LIFEENGINE_LOG_INFO( "Engine", "Loaded resource [%s]", InPath.c_str() );

	resource->SetName( InPath );
	resource->AddRef();
	resources.insert( std::make_pair( InPath, resource ) );
	return resource;
}

/**
 * Unload resource
 */
void le::ResourceSystem::UnloadResource( Resource*& InResource )
{
	LIFEENGINE_ASSERT( InResource );

	if ( InResource->GetRefCount() <= 1 )
	{
		auto		it = resources.find( InResource->GetName() );
		if ( it != resources.end() )
		{
			LIFEENGINE_LOG_INFO( "Engine", "Unloaded resource [%s]", it->first.c_str() );
			resources.erase( it );
		}
	}

	InResource->ReleaseRef();
	InResource = nullptr;
}

/**
 * Unload resources
 */
void le::ResourceSystem::UnloadResources()
{
	for ( auto it = resources.begin(), itEnd = resources.end(); it != itEnd; )
		if ( it->second->GetRefCount() <= 1 )
		{
			LIFEENGINE_LOG_INFO( "Engine", "Unloaded resource [%s]", it->first.c_str() );
			it->second->ReleaseRef();
			
			it = resources.erase( it );
			itEnd = resources.end();
		}
		else
			++it;
}
