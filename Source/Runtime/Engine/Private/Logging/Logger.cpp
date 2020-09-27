// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <stdarg.h>
#include <sstream>

#include "Logging/Logger.h"

/**
 * Constructor
 */
le::Logger::Logger() :
	file( nullptr ),
	startLogging( std::chrono::steady_clock::now() )
{}

/**
 * Destructor
 */
le::Logger::~Logger()
{
	CloseFile();
}

/**
 * Print message to log file
 */
void le::Logger::Logf( ELogType InLogType, const std::string& InCategory, const char* InMessage, ... )
{
#ifndef LIFEENGINE_NO_LOGGING
	va_list					argList = {};
	std::stringstream		strStream;

	strStream << "[" << std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::steady_clock::now() - startLogging ).count() << " ms][" << InCategory << "]";

	switch ( InLogType )
	{
	case LT_Debug:		strStream << "[Debug] " << InMessage << "\n"; break;
	case LT_Info:		strStream << "[Info] " << InMessage << "\n"; break;
	case LT_Warning:	strStream << "[Warning] " << InMessage << "\n"; break;
	case LT_Error:		strStream << "[Error] " << InMessage << "\n"; break;
	case LT_Fail:		strStream << "[Fail] " << InMessage << "\n"; break;
	}

	// Print to console
	va_start( argList, InMessage );
	vfprintf( stdout, strStream.str().c_str(), argList );
	fflush( stdout );
	va_end( argList );

	// Print to file
	if ( !file ) return;
	va_start( argList, InMessage );
	vfprintf( file, strStream.str().c_str(), argList );
	fflush( file );
	va_end( argList );
#endif // !LIFEENGINE_NO_LOGGING
}

/**
 * Close file
 */
void le::Logger::CloseFile()
{
#ifndef LIFEENGINE_NO_LOGGING
	if ( !file )	return;
	
	Logf( LT_Info, "Engine", "-- Log file clossed --" );	
	fclose( file );
	file = nullptr;
#endif // !LIFEENGINE_NO_LOGGING
}

/**
 * Set file for logs
 */
void le::Logger::SetFile( const Path& InPath )
{
#ifndef LIFEENGINE_NO_LOGGING
	if ( file )			CloseFile();
	file = fopen( InPath.GetFullPath().c_str(), "w" );
	
	if ( file )		Logf( LT_Info, "Engine", "-- Log file openned --" );
	else			Logf( LT_Info, "Engine", "-- Log file [%s] failed open", InPath.GetFullPath().c_str() );
#endif // !LIFEENGINE_NO_LOGGING
}