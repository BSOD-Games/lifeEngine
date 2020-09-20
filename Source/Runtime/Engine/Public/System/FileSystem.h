// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"
#include "Misc/Path.h"
#include "Misc/Object.h"

namespace le
{
	typedef void*		FFileHandle;

	enum EFileOffset
	{
		FO_Begin,
		FO_Current,
		FO_End
	};

	class FileSystem : public Object
	{
	public:
		/* Constructor */
		FileSystem();

		/* Destructor */
		~FileSystem();

		/* Mount pack */
		bool Mount( const Path& InPath );

		/* Unmount pack */
		void Unmount( const Path& InPath );

		/* Create directory */
		bool CreateDirectory( const Path& InPath ) const;

		/* Open file */
		FFileHandle OpenFile( const Path& InPath, bool InIsTextMode = false, bool InCreateIfNotExist = false, bool InIsClearFile = false ) const;

		/* Close file */
		void CloseFile( FFileHandle& InFile ) const;

		/* Delete directory */
		void DeleteDirectory( const Path& InPath ) const;

		/* Delete file */
		void DeleteFile( const Path& InPath ) const;

		/* Write to file */
		void WriteToFile( FFileHandle InFile, const std::string& InString ) const;
		void WriteToFile( FFileHandle InFile, byte* InBuffer, uint64 InSize ) const;

		/* Read from file */
		uint32 ReadFromFile( FFileHandle InFile, std::string& OutString ) const;
		uint32 ReadFromFile( FFileHandle InFile, byte* InBuffer, uint64 InSize ) const;

		/* Read line from file */
		uint32 ReadLineFromFile( FFileHandle InFile, std::string& OutString, char InDelimiting = '\n' ) const;
		uint32 ReadLineFromFile( FFileHandle InFile, byte* InBuffer, uint64 InSize, char InDelimiting = '\n' ) const;
		
		/* Set position in file */
		void SetOffsetInFile( FFileHandle InFile, uint64 InOffset, EFileOffset InOffsetType = FO_Begin ) const;

		/* Set root path */
		FORCEINLINE void SetRootPath( const Path& InRootPath )
		{
			rootPath = InRootPath;
		}

		/* Is exist file */
		bool IsExistFile( const Path& InPath ) const;

		/* Is EOF in file */
		bool IsEOFFile( FFileHandle InFile ) const;

		/* Get offset in file */
		uint64 GetOffsetInFile( FFileHandle InFile ) const;

		/* Get file size */
		uint64 GetSizeFile( FFileHandle InFile ) const;

	private:
		Path		rootPath;
	};
}

#endif // !FILESYSTEM_H
