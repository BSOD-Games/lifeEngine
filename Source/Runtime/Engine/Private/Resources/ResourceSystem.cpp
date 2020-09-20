// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include "Misc/EngineDefines.h"
#include "Misc/EngineGlobals.h"
#include "Logging/LogMacros.h"
#include "Resources/ResourceSystem.h"
#include "Resources/Parsers/ParsersTexture2DFactory.h"

// Register parsers
#include "Resources/Parsers/ParserTexture2DSTBImage.h"

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
 * Initialize resource system
 */
bool le::ResourceSystem::Initialize()
{
	GParsersTexture2DFactory->Register( ParserTexture2DSTBImage::GetSupportedExtensions(), []() -> IParserTexture2D* { return new ParserTexture2DSTBImage(); } );	
	return true;
}

/**
 * Find resource
 */
le::Resource* le::ResourceSystem::FindResource( const Path& InPath, EResourceType InResourceType )
{
	auto		it = resources.find( InPath.GetFullPath() );
	if ( it != resources.end() )		return it->second;

	Resource*		resource = nullptr;
	switch ( InResourceType )
	{
	case RT_Texture2D:			resource = new Texture2D();		break;
	default:	return nullptr;
	}

	if ( !resource->Deserialize( InPath ) )
	{
		LIFEENGINE_LOG_ERROR( "Engine", "Failed loading resource [%s]", InPath.GetFullPath().c_str() );

		delete resource;
		return FindDefaultResource( InResourceType );
	}

	LIFEENGINE_LOG_INFO( "Engine", "Loaded resource [%s]", InPath.GetFullPath().c_str() );

	resource->SetName( InPath.GetFullPath() );
	resource->AddRef();
	resources.insert( std::make_pair( InPath.GetFullPath(), resource ) );
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

/**
 * Find default resource
 */
le::Resource* le::ResourceSystem::FindDefaultResource( EResourceType InResourceType )
{
	auto		it = defaultResources.find( InResourceType );
	if ( it != defaultResources.end() )		return it->second;

	Resource*		resource = nullptr;
	Path			path;

	switch ( InResourceType )
	{
	case RT_Texture2D:		
		resource = new Texture2D();	
		path = "Engine/Content/Textures/Error.jpg";	
		break;

	default:	return nullptr;
	}

	if ( !resource->Deserialize( path ) )
	{
		LIFEENGINE_LOG_ERROR( "Engine", "Failed loading default resource [%s]", path.GetFullPath().c_str() );

		delete resource;
		defaultResources.insert( std::make_pair( InResourceType, nullptr ) );
		return nullptr;
	}

	LIFEENGINE_LOG_INFO( "Engine", "Loaded default resource [%s]", path.GetFullPath().c_str() );

	resource->SetName( path.GetFullPath() );
	resource->AddRef();
	defaultResources.insert( std::make_pair( InResourceType, resource ) );
	return resource;
}
