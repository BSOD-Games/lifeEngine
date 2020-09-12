// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <fstream>

#include "Logging/LogMacros.h"
#include "System/FileSystem.h"

#ifdef PLATFORM_WINDOWS
	#include <Windows.h>

	#undef CreateDirectory
	#undef CreateFile
	#undef DeleteFile
#else
	#error Unknown platform
#endif // PLATFORM_WINDOWS


/* Constructor */
le::FileSystem::FileSystem()
{}

/* Destructor */
le::FileSystem::~FileSystem()
{}

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

/* Create directory */
bool le::FileSystem::CreateDirectory( const std::string& InPath ) const
{
	std::string			path;
	ReplaceSlashes( InPath, path );

#ifdef PLATFORM_WINDOWS
	return CreateDirectoryA( ( rootPath + "/" + path ).c_str(), 0 );
#else
	return false;
#endif // PLATFORM_WINDOWS
}

/* Open file */
le::FFileHandle le::FileSystem::OpenFile( const std::string& InPath, bool InIsTextMode, bool InCreateIfNotExist, bool InIsClearFile ) const
{
	std::fstream*		file = new std::fstream();
	std::string			path;
	ReplaceSlashes( InPath, path );

	int			flags = std::ios::in | std::ios::out;
	if ( !InIsTextMode )			flags |= std::ios::binary;
	if ( !InCreateIfNotExist )		flags |= std::ios::_Nocreate;
	if ( InIsClearFile )			flags |= std::ios::trunc;

	file->open( rootPath + "/" + path, flags );
	if ( file->is_open() )		return file;

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

/* Delete directory */
void le::FileSystem::DeleteDirectory( const std::string& InPath ) const
{
	// TODO: Implement recursive deleting files

	std::string			path;
	ReplaceSlashes( InPath, path );

#ifdef PLATFORM_WINDOWS
	RemoveDirectoryA( ( rootPath + "/" + path ).c_str() );
#endif // PLATFORM_WINDOWS
}

/* Delete file */
void le::FileSystem::DeleteFile( const std::string& InPath ) const
{
	std::string			path;
	ReplaceSlashes( InPath, path );

#ifdef PLATFORM_WINDOWS
	::DeleteFileA( ( rootPath + "/" + path ).c_str() );
#endif // PLATFORM_WINDOWS
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
	file.open( rootPath + "/" + InPath, std::ios::_Nocreate );
	if ( file.is_open() )		return true;
	
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
