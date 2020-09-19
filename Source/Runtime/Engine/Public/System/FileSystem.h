// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <vector>

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"
#include "System/Object.h"

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
		bool Mount( const std::string& InPath );

		/* Unmount pack */
		void Unmount( const std::string& InPath );

		/* Create directory */
		bool CreateDirectory( const std::string& InPath ) const;

		/* Open file */
		FFileHandle OpenFile( const std::string& InPath, bool InIsTextMode = false, bool InCreateIfNotExist = false, bool InIsClearFile = false ) const;

		/* Close file */
		void CloseFile( FFileHandle& InFile ) const;

		/* Delete directory */
		void DeleteDirectory( const std::string& InPath ) const;

		/* Delete file */
		void DeleteFile( const std::string& InPath ) const;

		/* Write to file */
		void WriteToFile( FFileHandle InFile, const std::string& InString ) const;
		void WriteToFile( FFileHandle InFile, byte* InBuffer, uint64 InSize ) const;

		/* Read from file */
		void ReadFromFile( FFileHandle InFile, std::string& OutString ) const;
		void ReadFromFile( FFileHandle InFile, byte* InBuffer, uint64 InSize ) const;

		/* Read line from file */
		void ReadLineFromFile( FFileHandle InFile, std::string& OutString, char InDelimiting = '\n' ) const;
		void ReadLineFromFile( FFileHandle InFile, byte* InBuffer, uint64 InSize, char InDelimiting = '\n' ) const;
		
		/* Set position in file */
		void SetOffsetInFile( FFileHandle InFile, uint64 InOffset, EFileOffset InOffsetType = FO_Begin ) const;

		/* Set root path */
		FORCEINLINE void SetRootPath( const std::string& InRootPath )
		{
			rootPath = InRootPath;
		}

		/* Is exist file */
		bool IsExistFile( const std::string& InPath ) const;

		/* Get offset in file */
		uint64 GetOffsetInFile( FFileHandle InFile ) const;

		/* Get file size */
		uint64 GetSizeFile( FFileHandle InFile ) const;

	private:
		/* Replace slashes */
		FORCEINLINE void ReplaceSlashes( const std::string& InPath, std::string& OutPath ) const
		{
			OutPath = InPath;
			for ( uint32 index = 0; index < OutPath.size(); ++index )
				if ( OutPath[ index ] == '\\' )		OutPath[ index ] = '/';
		}

		std::string							rootPath;
	};
}

#endif // !FILESYSTEM_H
