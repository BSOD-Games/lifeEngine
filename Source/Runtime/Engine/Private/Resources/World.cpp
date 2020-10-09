// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Logging/LogMacros.h"
#include "System/FileSystem.h"
#include "Resources/Parsers/IParserWorld.h"
#include "Resources/Parsers/ParsersWorldFactory.h"
#include "Resources/World.h"

/**
 * Constructor
 */
le::World::World()
{}

/**
 * Constructor of copy
 */
le::World::World( const World& InCopy ) :
	spriteComponents( InCopy.spriteComponents )
{
	for ( uint32 index = 0, count = static_cast< uint32 >( InCopy.actors.size() ); index < count; ++index )
	{
		Actor*		actor = InCopy.actors[ index ];
		actor->AddRef();
		actors.push_back( actor );
	}
}

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

	IParserWorld*			parser = GParsersWorldFactory->Get( InPath.GetExtension() );
	FFileHandle				fileHandle = GFileSystem->OpenFile( InPath );
	if ( !fileHandle || !parser )
	{
		if ( parser )			parser->ReleaseRef();
		if ( fileHandle )		GFileSystem->CloseFile( fileHandle );
		return false;
	}

	if ( !parser->SetFile( fileHandle ) )	return false;
	spriteComponents = parser->GetSpriteComponents();
	actors = parser->GetActors();

	for ( uint32 index = 0, count = static_cast< uint32 >( actors.size() ); index < count; ++index )
		actors[ index ]->AddRef();

	parser->ReleaseRef();
	GFileSystem->CloseFile( fileHandle );
	return true;
}

/**
 * Update
 */
void le::World::Tick()
{
	for ( uint32 index = 0, count = static_cast< uint32 >( actors.size() ); index < count; ++index )
		actors[ index ]->Tick();
}

/**
 * Render
 */
void le::World::Render()
{
	for ( uint32 index = 0, count = static_cast< uint32 >( spriteComponents.size() ); index < count; ++index )
		spriteComponents[ index ].Render();

	for ( uint32 index = 0, count = static_cast< uint32 >( actors.size() ); index < count; ++index )
		actors[ index ]->Render();
}

/**
 * Spawn actor
 */
void le::World::Spawn( Actor* InActor )
{
	LIFEENGINE_ASSERT( InActor );

	InActor->AddRef();
	if ( !InActor->IsInitialized() )		InActor->Initialize();
	actors.push_back( InActor );
}

/**
 * Kill actor
 */
void le::World::Kill( Actor* InActor )
{
	LIFEENGINE_ASSERT( InActor );
	for ( uint32 index = 0, count = static_cast< uint32 >( actors.size() ); index < count; ++index )
	{
		Actor* actor = actors[ index ];

		actor->ReleaseRef();
		actors.erase( index + actors.begin() );
		return;
	}
}

/**
 * Kill all actors
 */
void le::World::KillAllActors()
{
	for ( uint32 index = 0, count = static_cast< uint32 >( actors.size() ); index < count; ++index )
		actors[ index ]->ReleaseRef();

	actors.clear();
}

/**
 * Clear level
 */
void le::World::Clear()
{
	for ( uint32 index = 0, count = static_cast< uint32 >( actors.size() ); index < count; ++index )
		actors[ index ]->ReleaseRef();

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
