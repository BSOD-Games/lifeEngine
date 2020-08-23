// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#include <stdarg.h>
#include <time.h>
#include <sstream>

#include "Logging/Logger.h"

/**
 * Constructor
 */
le::Logger::Logger() :
	file( nullptr )
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
	va_list					argList = {};
	std::stringstream		strStream;
	
	strStream << "[" << clock() / CLK_TCK << "][" << InCategory << "]";

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
}

/**
 * Close file
 */
void le::Logger::CloseFile()
{
	if ( !file )	return;
	
	Logf( LT_Info, "Core", "-- Log file clossed --" );	
	fclose( file );
	file = nullptr;
}

/**
 * Set file for logs
 */
void le::Logger::SetFile( const std::string& InPath )
{
	if ( file )			CloseFile();
	file = fopen( InPath.c_str(), "w" );
	
	if ( file )		Logf( LT_Info, "Core", "-- Log file openned --" );
	else			Logf( LT_Info, "Core", "-- Log file [%s] failed open", InPath.c_str() );
}