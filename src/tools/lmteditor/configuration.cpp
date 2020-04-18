//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <qdebug.h>
#include <qfile.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

#include "common/types.h"
#include "configuration.h"

#define PATH_FILE_CONFIGURATION			"lmteditor.cfg"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Configuration::Configuration()
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Configuration::~Configuration()
{}

// ------------------------------------------------------------------------------------ //
// Load configurations
// ------------------------------------------------------------------------------------ //
bool Configuration::Load()
{
	QFile			file( PATH_FILE_CONFIGURATION );
	qDebug() << "Configurations [" PATH_FILE_CONFIGURATION "] loading";

	if ( !file.open( QIODevice::ReadOnly ) )
	{
		qDebug() << "Configurations [" PATH_FILE_CONFIGURATION "] not loaded. File or directory not found";
		return false;
	}

	// Parse json and get main block
	QJsonDocument		jsonDoc = QJsonDocument::fromJson( file.readAll() );
	QJsonObject			jsonRoot = jsonDoc.object();

	bool				isContainsGames = jsonRoot.contains( "games" );

	// Getting block games
	if ( isContainsGames && jsonRoot[ "games" ].isArray() )
	{
		QJsonArray			jsonGames = jsonRoot[ "games" ].toArray();
		QJsonObject			jsonObject;
		GameDescriptor		game;

		for ( le::UInt32_t index = 0, count = jsonGames.size(); index < count; ++index )
			if ( jsonGames[ index ].isObject() )
			{
				jsonObject = jsonGames[ index ].toObject();
				game.name = jsonObject.contains( "name" ) && jsonObject[ "name" ].isString() ? jsonObject[ "name" ].toString() : "";
				game.path = jsonObject.contains( "path" ) && jsonObject[ "path" ].isString() ? jsonObject[ "path" ].toString() : "";

				if ( game.name.isEmpty() || game.path.isEmpty() ) continue;
				games.push_back( game );
			}
	}

	qDebug() << "Configurations loaded";
	return true;
}

// ------------------------------------------------------------------------------------ //
// Save configurations
// ------------------------------------------------------------------------------------ //
void Configuration::Save()
{
	// Generate block info by games
	QJsonArray			jsonGames;
	for ( auto it = games.begin(), itEnd = games.end(); it != itEnd; ++it )
	{
		QJsonObject		jsonObject;
		jsonObject[ "name" ] = (*it).name;
		jsonObject[ "path" ] = (*it).path;
		jsonGames.append( jsonObject );
	}

	// Generate main block
	QJsonObject			jsonRoot;
	jsonRoot[ "games" ] = jsonGames;

	QJsonDocument		jsonDoc = QJsonDocument( jsonRoot );
	QFile				file( PATH_FILE_CONFIGURATION );
	if ( file.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
	{
		file.write( jsonDoc.toJson() );
		qDebug() << "Configurations saved to [" PATH_FILE_CONFIGURATION "]";
	}
	else
		qDebug() << "Failed creating or open file configurations for save";
}

// ------------------------------------------------------------------------------------ //
// Set games
// ------------------------------------------------------------------------------------ //
void Configuration::SetGames( const std::vector<GameDescriptor>& Games )
{
	games = Games;
}

// ------------------------------------------------------------------------------------ //
// Get games
// ------------------------------------------------------------------------------------ //
const std::vector<GameDescriptor>& Configuration::GetGames() const
{
	return games;
}
