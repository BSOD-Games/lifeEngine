// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <fstream>

#include "Logging/LogMacros.h"
#include "System/FileSystem.h"

/* Constructor */
le::FileSystem::FileSystem()
{}

/* Destructor */
le::FileSystem::~FileSystem()
{}

/* Add search path */
void le::FileSystem::AddSearchPath( const std::string& InPath )
{
	std::string				newPath;
	ReplaceSlashes( InPath, newPath );

	searchPaths.push_back( newPath );
	LIFEENGINE_LOG_INFO( "Engine", "Added to search path: %s", newPath.c_str() );
}

/* Remove search path */
void le::FileSystem::RemoveSearchPath( uint32 InIndex )
{
	if ( InIndex >= searchPaths.size() ) return;

	LIFEENGINE_LOG_INFO( "Engine", "Removed from search path: %s", searchPaths[ InIndex ].c_str() );
	searchPaths.erase( searchPaths.begin() + InIndex );
}

/* Mount pack */
bool le::FileSystem::Mount( const std::string& InPath )
{
	// TODO: Implement this
	LIFEENGINE_LOG_WARNING( "Engine", "bool le::FileSystem::Mount( const std::string& InPath ) :: Not implemented" );
	return false;
}

/* Unmount pack */
void le::FileSystem::Unmount( const std::string& InPath )
{
	// TODO: Implement this
	LIFEENGINE_LOG_WARNING( "Engine", "bool le::FileSystem::Unmount( const std::string& InPath ) :: Not implemented" );
}

/* Create file */
le::FFileHandle le::FileSystem::CreateFile( const std::string& InPath ) const
{
	std::fstream*		file = new std::fstream();
	std::string			path;
	ReplaceSlashes( InPath, path );

	for ( uint32 index = 0; index < searchPaths.size(); ++index )
	{
		file->open( searchPaths[ index ] + "/" + path, std::ios::in | std::ios::out | std::ios::trunc );
		if ( file->is_open() )		return file;
	}

	delete file;
	return nullptr;
}

/* Open file */
le::FFileHandle le::FileSystem::OpenFile( const std::string& InPath ) const
{
	std::fstream*		file = new std::fstream();
	std::string			path;
	ReplaceSlashes( InPath, path );

	for ( uint32 index = 0; index < searchPaths.size(); ++index )
	{
		file->open( searchPaths[ index ] + "/" + path, std::ios::in | std::ios::out | std::ios::app );
		if ( file->is_open() )		return file;
	}

	delete file;
	return nullptr;
}

/* Close file */
void le::FileSystem::CloseFile( FFileHandle& InFile ) const
{
	LIFEENGINE_ASSERT( InFile );
	
	std::fstream*		file = static_cast< std::fstream* >( InFile );
	if ( !file->is_open() )	return;

	file->close();
	delete file;
	InFile = nullptr;
}

/* Write to file */
void le::FileSystem::WriteToFile( FFileHandle InFile, const std::string& InString ) const
{
	LIFEENGINE_ASSERT( InFile );
	*static_cast< std::fstream* >( InFile ) << InString;
}

/* Write to file */
void le::FileSystem::WriteToFile( FFileHandle InFile, byte* InBuffer, uint64 InSize ) const
{
	LIFEENGINE_ASSERT( InFile && InBuffer && InSize > 0 );

	std::fstream*		file = static_cast< std::fstream* >( InFile );
	file->write( ( char* ) InBuffer, InSize );
}

/* Read from file */
void le::FileSystem::ReadFromFile( FFileHandle InFile, std::string& OutString ) const
{
	LIFEENGINE_ASSERT( InFile );
	*static_cast< std::fstream* >( InFile ) >> OutString;
}

/* Read from file */
void le::FileSystem::ReadFromFile( FFileHandle InFile, byte* InBuffer, uint64 InSize ) const
{
	LIFEENGINE_ASSERT( InFile && InBuffer && InSize > 0 );

	std::fstream*		file = static_cast< std::fstream* >( InFile );
	file->read( ( char* ) InBuffer, InSize );
}

/* Read line from file */
void le::FileSystem::ReadLineFromFile( FFileHandle InFile, std::string& OutString, char InDelimiting ) const
{
	LIFEENGINE_ASSERT( InFile );
	std::getline( *static_cast< std::fstream* >( InFile ), OutString, InDelimiting );
}

/* Read line from file */
void le::FileSystem::ReadLineFromFile( FFileHandle InFile, byte* InBuffer, uint64 InSize, char InDelimiting ) const
{
	LIFEENGINE_ASSERT( InFile && InBuffer && InSize > 0 );
	static_cast< std::fstream* >( InFile )->getline( ( char* ) InBuffer, InSize, InDelimiting );
}

/* Set offset in file */
void le::FileSystem::SetOffsetInFile( FFileHandle InFile, uint64 InOffset, EFileOffset InOffsetType ) const
{
	LIFEENGINE_ASSERT( InFile );

	std::fstream*		file = static_cast< std::fstream* >( InFile );
	uint32				offsetType = 0;
	
	switch ( InOffsetType )
	{
	case FO_Begin:		offsetType = std::ios::beg;		break;
	case FO_Current:	offsetType = std::ios::cur;		break;
	case FO_End:		offsetType = std::ios::end;		break;
	}

	file->seekg( InOffset, offsetType );
}

/* Is exist file */
bool le::FileSystem::IsExistFile( const std::string& InPath ) const
{
	std::fstream		file;
	for ( uint32 index = 0; index < searchPaths.size(); ++index )
	{
		file.open( searchPaths[ index ] + "/" + InPath, std::ios::in | std::ios::out | std::ios::app );
		if ( file.is_open() )		return true;
	}
	
	return false;
}

/* Get offset in file */
le::uint64 le::FileSystem::GetOffsetInFile( FFileHandle InFile ) const
{
	LIFEENGINE_ASSERT( InFile );

	std::fstream*		file = static_cast< std::fstream* >( InFile );
	return file->tellg();
}

/* Get file size */
le::uint64 le::FileSystem::GetSizeFile( FFileHandle InFile ) const
{
	LIFEENGINE_ASSERT( InFile );

	uint64		currentOffset = GetOffsetInFile( InFile );
	uint64		sizeFile = 0;

	SetOffsetInFile( InFile, 0, FO_End );
	sizeFile = GetOffsetInFile( InFile );
	SetOffsetInFile( InFile, currentOffset, FO_Begin );

	return sizeFile;
}
