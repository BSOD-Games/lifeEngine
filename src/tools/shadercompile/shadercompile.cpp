//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdexcept>
#include <exception>
#include <iostream>
#include <fstream>

#include "common/types.h"
#include "global.h"

#define GOLD_DATE						43553			// Mar 31 2020 (start coding shadercompile)
#define SHADERCOMPILE_VERSION_MAJOR		0
#define SHADERCOMPILE_VERSION_MINOR		1
#define SHADERCOMPILE_VERSION_PATCH		0

// ------------------------------------------------------------------------------------ //
// Compute build number
// ------------------------------------------------------------------------------------ //
int Application_ComputeBuildNumber( int GoldDate )
{
	const char* date = __DATE__;

	const char* month[ 12 ] =
	{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	const char		month_days[ 12 ] =
	{ 31,    28,    31,    30,    31,    30,    31,    31,    30,    31,    30,    31 };

	int				buildNumber = 0;
	int				months = 0;
	int				days = 0;
	int				years = 0;

	for ( months = 0; months < 11; ++months )
	{
		if ( strncmp( &date[ 0 ], month[ months ], 3 ) == 0 )
			break;

		days += month_days[ months ];
	}

	days += atoi( &date[ 4 ] ) - 1;
	years = atoi( &date[ 7 ] ) - 1900;

	buildNumber = days + static_cast< int >( ( years - 1 ) * 365.25f );

	if ( ( years % 4 == 0 ) && months > 1 )
		++buildNumber;

	buildNumber -= GoldDate;
	return buildNumber;
}

// ------------------------------------------------------------------------------------ //
// Print help
// ------------------------------------------------------------------------------------ //
void Application_Help()
{
	std::cout << "-------------------------\n"
			  << "Shadercompile " << SHADERCOMPILE_VERSION_MAJOR << "." << SHADERCOMPILE_VERSION_MINOR << "." << SHADERCOMPILE_VERSION_PATCH << " (build " << Application_ComputeBuildNumber( GOLD_DATE ) << ") by Egor Pogulyaka\n"
			  << "Shader compiler to generate header files (*.h) from GLSL sources for including in C++ code\n"
			  << "-------------------------\n"
			  << "Usage: shadercompile -s <pathToSource> -o <pathToOutput> [other parameters]\n"
			  << "Output: shader in header file (*.h) for including in C++ code\n"
			  << "Example: shadercompile -s light.glsl -o light\n\n"
			  << "-h | -help\t->\tshow this message\n"
			  << "-s | -source\t->\tpath to source file\n"
			  << "-o | -output\t->\tpath to output file\n\n";
}

// ------------------------------------------------------------------------------------ //
// Parse arguments start
// ------------------------------------------------------------------------------------ //
void Application_ParseArgs( int argc, char** argv )
{
	// Parse arguments
	for ( le::UInt32_t index = 1; index < argc; ++index )
	{
		// Help info
		if ( strstr( argv[ index ], "-h" ) || strstr( argv[ index ], "-help" ) )
		{
			Application_Help();
			exit( 0 );
		}

		// Set output file
		else if ( ( strstr( argv[ index ], "-o" ) || strstr( argv[ index ], "-output" ) ) && index + 1 < argc )
		{
			g_outputFile = argv[ index + 1 ];
			++index;
		}

		// Set source file
		else if ( ( strstr( argv[ index ], "-s" ) || strstr( argv[ index ], "-source" ) ) && index + 1 < argc )
		{
			g_sourceFile = argv[ index + 1 ];
			++index;
		}
	}
}

// ------------------------------------------------------------------------------------ //
// Get name file from path
// ------------------------------------------------------------------------------------ //
std::string Application_GetNameFile( const std::string& Path )
{
	std::string			nameFile = Path;

	// Change '\' on '/' if such exist
	if ( nameFile.find( '\\' ) != std::string::npos )
		for ( le::UInt32_t index = 0, count = nameFile.size(); index < count; ++index )
			if ( nameFile[ index ] == '\\' )
				nameFile[ index ] = '/';

	// Remove all '/' and '.' if such exist
	auto			lastIndexSlash = nameFile.find_last_of( '/' );
	if ( lastIndexSlash != std::string::npos )
		nameFile.erase( 0, lastIndexSlash+1 );

	auto			lastIndexDot = nameFile.find_last_of( '.' );
	if ( lastIndexDot != std::string::npos )
		nameFile.erase( lastIndexDot, nameFile.size() );

	return nameFile;
}

// ------------------------------------------------------------------------------------ //
// Generate *.h file from source file
// ------------------------------------------------------------------------------------ //
void Application_GenerateHFile( const std::string& PathIn, const std::string& PathOut )
{
	std::ifstream		fileIn( PathIn );
	if ( !fileIn.is_open() )		throw std::runtime_error( "Failed to open or find source file" );

	std::ofstream		fileOut( PathOut + ".h" );
	if ( !fileOut.is_open() )		throw std::runtime_error( "Could not open output file for writing" );

	// Getting output file name for seting string name in header file
	std::string			outputFileName = Application_GetNameFile( PathOut );
	if ( outputFileName.empty() )				throw std::runtime_error( "Woops, output file name is empty" );

	const char*		shader_endline = "\\n\\";

	// Print info by shadercompiler and #ifndef
	fileOut << "//\n"
			<< "// Generated by shadercompiler " << SHADERCOMPILE_VERSION_MAJOR << "." << SHADERCOMPILE_VERSION_MINOR << "." << SHADERCOMPILE_VERSION_PATCH << " (build " << Application_ComputeBuildNumber( GOLD_DATE ) << ")\n"
			<< "//\n\n";

	fileOut << "#ifndef SHADER_" << outputFileName << "_INC\n"
			<< "#define SHADER_" << outputFileName << "_INC\n\n";

	// Generate array C-string in output file
	fileOut << "const char* shader_" << outputFileName << " = " << std::endl
			<< "\"";

	// Converting code
	while ( true )
	{
		std::string			lineShader;
		std::getline( fileIn, lineShader );
		fileOut << lineShader;

		if ( !fileIn.eof() )
			fileOut << shader_endline << std::endl;
		else break;
	}

	// Print end file
	fileOut << "\";\n\n";
	fileOut << "#endif";

	// Closing files
	fileIn.close();
	fileOut.close();
}

// ------------------------------------------------------------------------------------ //
// Main function
// ------------------------------------------------------------------------------------ //
int main( int argc, char** argv )
{
	if ( argc > 1 )
	{
		// Parse arguments
		Application_ParseArgs( argc, argv );

		try
		{
			if ( g_sourceFile.empty() )			throw std::runtime_error( "Source file not set. Use argument -s <path>" );
			if ( g_outputFile.empty() )			throw std::runtime_error( "Output file not set. Use argument -o <path>" );
			Application_GenerateHFile( g_sourceFile, g_outputFile );
		}
		catch ( const std::exception& Exception )
		{
			std::cerr << "Error: " << Exception.what() << std::endl;
			return 1;
		}
	}
	else
		Application_Help();

	return 0;
}
