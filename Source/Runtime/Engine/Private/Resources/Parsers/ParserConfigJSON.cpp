// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "Misc/EngineGlobals.h"
#include "System/FileSystem.h"
#include "ParserConfigJSON.h"

/**
 * Constructor
 */
le::ParserConfigJSON::ParserConfigJSON() :
	jsonDocument( nullptr )
{}

/**
 * Destructor
 */
le::ParserConfigJSON::~ParserConfigJSON()
{
	Clear();
}

/**
 * Set file
 */
bool le::ParserConfigJSON::SetFile( FFileHandle InFileHandle )
{
	LIFEENGINE_ASSERT( InFileHandle );
	if ( IsOppened() )		Clear();

	std::string			strBuffer;
	GFileSystem->ReadLineFromFile( InFileHandle, strBuffer, '\0' );

	rapidjson::Document* jsonDocument = new rapidjson::Document();
	this->jsonDocument = jsonDocument;

	jsonDocument->Parse( strBuffer.c_str() );
	if ( jsonDocument->HasParseError() )		return false;
	return true;
}

/**
 * Get groups
 */
std::vector< le::Config::Group > le::ParserConfigJSON::GetGroups() const
{
	if ( !IsOppened() )						return std::vector< Config::Group >();
	std::vector< Config::Group >			groups;
	rapidjson::Document*					jsonDocument = static_cast< rapidjson::Document* >( this->jsonDocument );

	for ( auto itRoot = jsonDocument->MemberBegin(), itRootEnd = jsonDocument->MemberEnd(); itRoot != itRootEnd; ++itRoot )
		if ( itRoot->value.IsObject() )
		{
			Config::Group		group;
			group.SetName( itRoot->name.GetString() );

			for ( auto itGroup = itRoot->value.GetObject().MemberBegin(), itGroupEnd = itRoot->value.GetObject().MemberEnd(); itGroup != itGroupEnd; ++itGroup )
			{
				Config::Var			var;
				var.SetName( itGroup->name.GetString() );
				
				if ( itGroup->value.IsInt() )				var.SetValueInt( itGroup->value.GetInt() );
				else if ( itGroup->value.IsFloat() )		var.SetValueFloat( itGroup->value.GetFloat() );
				else if ( itGroup->value.IsString() )		var.SetValueString( itGroup->value.GetString() );

				group.AddVar( var );
			}

			groups.push_back( group );
		}

	return groups;
}

/**
 * Clear
 */
void le::ParserConfigJSON::Clear()
{
	if ( !jsonDocument )	return;
	delete static_cast< rapidjson::Document* >( jsonDocument );
	jsonDocument = nullptr;
}
