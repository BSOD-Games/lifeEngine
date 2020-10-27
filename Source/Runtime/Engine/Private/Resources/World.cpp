// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Misc/EngineGlobals.h"
#include "Logging/LogMacros.h"
#include "System/FileSystem.h"
#include "Rendering/RenderSystem.h"
#include "Resources/Parsers/IParserWorld.h"
#include "Resources/Parsers/ParsersWorldFactory.h"
#include "Resources/World.h"
#include "World/ActorFactory.h"

/**
 * Constructor
 */
le::World::World()
{}

/**
 * Constructor of copy
 */
le::World::World( const World& InCopy ) :
	spriteComponents( InCopy.spriteComponents ),
	actors( InCopy.actors )
{}

/**
 * Destructor
 */
le::World::~World()
{
	Clear();
}

/**
 * Serialize resource
 */
bool le::World::Serialize( const Path& InPath )
{
	LIFEENGINE_LOG_WARNING( "Engine", "le::World::Serialize( const Path& InPath ) :: Not implemented" );
	return false;
}

/**
 * Deserialize resource
 */
bool le::World::Deserialize( const Path& InPath )
{
	if ( InPath.IsEmpty() )		return false;

	FIParserWorldRef		parser = GParsersWorldFactory->Get( InPath.GetExtension() );
	FFileHandle				fileHandle = GFileSystem->OpenFile( InPath );
	if ( !fileHandle || !parser )
	{
		if ( fileHandle )		GFileSystem->CloseFile( fileHandle );
		return false;
	}

	if ( !parser->SetFile( fileHandle ) )	return false;
	spriteComponents = parser->GetSpriteComponents();
	
	std::vector< SWorldObject >		worldObjects = parser->GetObjects();
	for ( uint32 index = 0, count = static_cast< uint32 >( worldObjects.size() ); index < count; ++index )
	{
		const SWorldObject&		worldObject = worldObjects[ index ];
		FActorRef				actor = GActorFactory->Create( worldObject.name );
		if ( !actor )	continue;

		actor->Initialize( this, &worldObject.actorVars );
		actors.push_back( actor );
	}

	GFileSystem->CloseFile( fileHandle );
	return true;
}

/**
 * Update
 */
void le::World::Tick()
{
	for ( uint32 index = 0, count = static_cast< uint32 >( actors.size() ); index < count; )
	{
		FActorRef&		actor = actors[ index ];
		
		actor->Tick();
		if ( actor->GetStatus() == AS_Dead )
		{
			actors.erase( actors.begin() + index );
			count = static_cast< uint32 >( actors.size() );
		}
		else
			++index;
	}
}

/**
 * Render
 */
void le::World::Render()
{
	// TODO: [yehor.pohuliaka] Implement place for storage scene cameras (example: player camera, ui camera, camera for mirror and etc). Idea: storage and set cameras implement in World (SetMainCamera, SetUICamera, etc)
	FCameraComponentConstRef&		cameraComponent = GRenderSystem->GetCamera();
	for ( uint32 index = 0, count = static_cast< uint32 >( spriteComponents.size() ); index < count; ++index )
	{
		FSpriteComponentRef&	spriteComponent = spriteComponents[ index ];
		FVector3D				position = spriteComponent->GetTransformComponent()->GetGlobalPosition();
		FVector2D				size = spriteComponent->GetSize();

		// Rendering visible sprites
		if ( !cameraComponent || cameraComponent->GetFrustum().IsVisible( position, FVector3D( position.x + size.x, position.y + size.y, position.z ) ) )
			spriteComponent->Render();
	}

	for ( uint32 index = 0, count = static_cast< uint32 >( actors.size() ); index < count; ++index )
	{
		// Rendering actors
		actors[ index ]->Render();
	}
}

/**
 * Spawn actor
 */
void le::World::Spawn( FActorConstRef& InActor )
{
	LIFEENGINE_ASSERT( InActor );
	actors.push_back( InActor );
}

/**
 * Kill actor
 */
void le::World::Kill( FActorConstRef& InActor )
{
	LIFEENGINE_ASSERT( InActor );
	for ( uint32 index = 0, count = static_cast< uint32 >( actors.size() ); index < count; ++index )
	{
		FActorRef&		actor = actors[ index ];
		if ( actor != InActor )		continue;

		actors.erase( index + actors.begin() );
		return;
	}
}

/**
 * Kill all actors
 */
void le::World::KillAllActors()
{
	actors.clear();
}

/**
 * Clear level
 */
void le::World::Clear()
{
	spriteComponents.clear();
	actors.clear();
}

/**
 * Get type resource
 */
le::EResourceType le::World::GetType() const
{
	return RT_World;
}
