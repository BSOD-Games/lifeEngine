// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Misc/EngineDefines.h"
#include "Misc/EngineGlobals.h"
#include "Logging/LogMacros.h"
#include "Resources/ResourceSystem.h"

// Parsers factory
#include "Resources/Parsers/ParsersTexture2DFactory.h"
#include "Resources/Parsers/ParsersConfigFactory.h"
#include "Resources/Parsers/ParsersMaterialFactory.h"
#include "Resources/Parsers/ParsersWorldFactory.h"

// Resources parsers
#include "Resources/Parsers/ParserTexture2DSTBImage.h"
#include "Resources/Parsers/ParserConfigJSON.h"
#include "Resources/Parsers/ParserMaterialLMT.h"
#include "Resources/Parsers/ParserWorldTMX.h"

// Resources
#include "Resources/Texture2D.h"
#include "Resources/Config.h"
#include "Resources/Material.h"
#include "Resources/World.h"

/**
 * Constructor
 */
le::ResourceSystem::ResourceSystem()
{}

/**
 * Destructor
 */
le::ResourceSystem::~ResourceSystem()
{}

/**
 * Initialize resource system
 */
bool le::ResourceSystem::Initialize()
{
	GParsersTexture2DFactory->Register( ParserTexture2DSTBImage::GetSupportedExtensions(), []() -> FIParserTexture2DRef { return new ParserTexture2DSTBImage(); } );
	GParsersConfigFactory->Register( ParserConfigJSON::GetSupportedExtensions(), []() -> FIParserConfigRef { return new ParserConfigJSON(); } );
	GParsersMaterialFactory->Register( ParserMaterialLMT::GetSupportedExtensions(), []() -> FIParserMaterialRef { return new ParserMaterialLMT(); } );
	GParsersWorldFactory->Register( ParserWorldTMX::GetSupportedExtensions(), []() -> FIParserWorldRef { return new ParserWorldTMX(); } );
	return true;
}

/**
 * Find resource
 */
le::FResourceRef le::ResourceSystem::FindResource( const Path& InPath, EResourceType InResourceType )
{
	auto		it = resources.find( InPath.GetFullPath() );
	if ( it != resources.end() )		return it->second;

	FResourceRef		resource;
	switch ( InResourceType )
	{
	case RT_Texture2D:			resource = new Texture2D();		break;
	case RT_Config:				resource = new Config();		break;
	case RT_Material:			resource = new Material();		break;
	case RT_World:				resource = new World();			break;
	default:					return nullptr;
	}

	if ( !resource->Deserialize( InPath ) )
	{
		LIFEENGINE_LOG_ERROR( "Engine", "Failed loading resource [%s]", InPath.GetFullPath().c_str() );
		return FindDefaultResource( InResourceType );
	}

	LIFEENGINE_LOG_INFO( "Engine", "Loaded resource [%s]", InPath.GetFullPath().c_str() );

	resource->SetName( InPath.GetFullPath() );
	resource->GetEventChannelDelete().Subscribe( this, &ResourceSystem::OnResourceDelete );
	
	resources.insert( std::make_pair( InPath.GetFullPath(), resource.GetPtr() ) );
	return resource;
}

/**
 * Find default resource
 */
le::FResourceRef le::ResourceSystem::FindDefaultResource( EResourceType InResourceType )
{
	auto		it = defaultResources.find( InResourceType );
	if ( it != defaultResources.end() )		return it->second;

	FResourceRef	resource;
	Path			path;

	switch ( InResourceType )
	{
	case RT_Texture2D:		
		resource = new Texture2D();	
		path = "Engine/Content/Textures/DefaultTexture.jpg";	
		break;

	case RT_Material:
		resource = new Material();
		path = "Engine/Content/Materials/DefaultMaterial.lmt";
		break;

	default:	return nullptr;
	}

	if ( !resource->Deserialize( path ) )
	{
		LIFEENGINE_LOG_ERROR( "Engine", "Failed loading default resource [%s]", path.GetFullPath().c_str() );

		defaultResources.insert( std::make_pair( InResourceType, nullptr ) );
		return nullptr;
	}

	LIFEENGINE_LOG_INFO( "Engine", "Loaded default resource [%s]", path.GetFullPath().c_str() );

	resource->SetName( path.GetFullPath() );
	resource->GetEventChannelDelete().Subscribe( this, &ResourceSystem::OnResourceDelete );

	defaultResources.insert( std::make_pair( InResourceType, resource.GetPtr() ) );
	return resource;
}

/**
 * On resource delete
 */
void le::ResourceSystem::OnResourceDelete( const Resource::EventDelete& InEvent )
{
	LIFEENGINE_ASSERT( InEvent.resource );
	
	auto		it = resources.find( InEvent.resource->GetName() );
	if ( it != resources.end() )
	{
		LIFEENGINE_LOG_INFO( "Engine", "Unloaded resource [%s]", it->first.c_str() );
		resources.erase( it );
	}
}