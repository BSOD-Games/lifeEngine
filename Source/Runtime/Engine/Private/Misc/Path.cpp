// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#include "Misc/Path.h"

/**
 * Constructor
 */
le::Path::Path()
{}

/**
 * Constructor
 */
le::Path::Path( const std::string& InPath )
{
	ReplaceSlashes( InPath, path );
}

/**
 * Constructor
 */
le::Path::Path( const char* InPath ) :
	path( InPath )
{
	ReplaceSlashes( InPath, path );
}

/**
 * Constructor of copy
 */
le::Path::Path( const Path& InCopy ) :
	path( InCopy.path )
{}

/**
 * Destructor
 */
le::Path::~Path()
{}

/**
 * Get path
 */
std::string le::Path::GetPath() const
{
	std::string			outPath = path;
	std::size_t			indexSlash = outPath.find_last_of( '/' );
	
	if ( indexSlash != std::string::npos )		outPath.erase( indexSlash, outPath.size() );
	return outPath;
}

/**
 * Get clean file name
 */
std::string le::Path::GetCleanFilename() const
{
	std::string			outPath = path;
	std::size_t			indexSlash = outPath.find_last_of( '/' );
	
	if ( indexSlash != std::string::npos )		outPath.erase( 0, indexSlash + 1 );
	return outPath;
}

/**
 * Get base file name
 */
std::string le::Path::GetBaseFilename() const
{
	std::string			outPath = path;
	std::size_t			indexSlash = outPath.find_last_of( '/' );
	if ( indexSlash != std::string::npos )		outPath.erase( 0, indexSlash + 1 );

	std::size_t			indexDot = outPath.find_last_of( '.' );
	if ( indexDot != std::string::npos )		outPath.erase( indexDot, outPath.size() );
	return outPath;
}

/**
 * Get extension
 */
std::string le::Path::GetExtension( bool InIsIncludeDot ) const
{
	std::string			outPath = path;
	std::size_t			indexSlash = outPath.find_last_of( '/' );
	if ( indexSlash != std::string::npos )		outPath.erase( 0, indexSlash + 1 );

	std::size_t			indexDot = outPath.find_last_of( '.' );
	if ( indexDot != std::string::npos )		outPath.erase( 0, InIsIncludeDot ? indexDot : indexDot + 1 );
	return outPath;
}
