// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef PATH_H
#define PATH_H

#include <string>

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"
#include "Misc/Object.h"

namespace le
{
	class Path : public Object
	{
	public:
		/* Constructor */
		Path();
		Path( const std::string& InPath );
		Path( const char* InPath );
		Path( const Path& InCopy );

		/* Destructor */
		~Path();

		/* Replace slashes */
		FORCEINLINE static void ReplaceSlashes( const std::string& InPath, std::string& OutPath )
		{
			OutPath = InPath;
			for ( uint32 index = 0; index < OutPath.size(); ++index )
				if ( OutPath[ index ] == '\\' )		OutPath[ index ] = '/';
		}

		/* Set full path */
		FORCEINLINE void SetFullPath( const std::string& InPath )
		{
			ReplaceSlashes( InPath, path );
		}

		/* Get full path */
		FORCEINLINE const std::string& GetFullPath() const
		{
			return path;
		}

		/* Is path empty */
		FORCEINLINE bool IsEmpty() const
		{
			return path.empty();
		}

		/* Returns the portion of the path before the last separator */
		std::string GetPath() const;

		/* Returns the portion of the path after the last separator */
		std::string GetCleanFilename() const;

		/* Returns the portion of the path after the last separator and before the last dot */
		std::string GetBaseFilename() const;

		/* Returns the portion of the path after the last dot following the last separator, optionally including the dot */
		std::string GetExtension( bool InIsIncludeDot = false ) const;

		/* Operator + */
		FORCEINLINE Path operator+( const Path& InRight ) const
		{
			Path		outPath( !GetExtension().empty() ? GetPath() : path );
			outPath.path += '/' + InRight.path;
			
			return outPath;
		}

		/* Operator = */
		FORCEINLINE Path& operator=( const std::string& InRight )
		{
			path = InRight;
			return *this;
		}

		FORCEINLINE Path& operator=( const char* InRight )
		{
			path = InRight;
			return *this;
		}

		FORCEINLINE Path& operator=( const Path& InRight )
		{
			path = InRight.path;
			return *this;
		}

	private:
		std::string			path;
	};
}

#endif // !PATHVIEWS_H
