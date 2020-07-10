//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdexcept>
#include <iostream>
#include <assimp/Importer.hpp>

#include "common/types.h"
#include "global.h"
#include "mesh.h"
#include "collision.h"

#define GOLD_DATE				43379			// Oct 8 2019 (start coding lmdl)
#define LMDL_VERSION_MAJOR		0
#define LMDL_VERSION_MINOR		3
#define LMDL_VERSION_PATCH		0

// ------------------------------------------------------------------------------------ //
// Compute build number
// ------------------------------------------------------------------------------------ //
int ComputeBuildNumber( int GoldDate )
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
void PrintUsage()
{
	std::cout << "-------------------------\n"
			  << "lifeEngine Model " << LMDL_VERSION_MAJOR << "." << LMDL_VERSION_MINOR << "." << LMDL_VERSION_PATCH << " (build " << ComputeBuildNumber( GOLD_DATE ) << ") by Egor Pogulyaka\n"
			  << "Converter models to MDL format\n"
			  << "-------------------------\n\n"
			  << "Usage: lmdl -s <pathToSource> -o <pathToOutput> [other parameters]\n"
			  << "Output: model in format MDL (lifeEngine Model)\n"
			  << "Example: lmdl -mesh -matdir materials/axe -o axe -s axe.fbx\n\n"
			  << "-h | -help\t->\tshow this message\n"
			  << "-s | -source\t->\tpath to source file\n"
			  << "-o | -output\t->\tpath to output file\n"
			  << "-matdir\t->\tpath to materials (relative to the game directory)\n"
			  << "-c | -collision\t->\set convert type: collision mesh\n"
			  << "-m | -mesh\t->\tset convert type: mesh. This is default type\n"
			  << "-masa\t->\tset masa for collision mesh\n"
			  << "-i | -inertia\t->\tset inertia for collision mesh. ex: -inertia 0 10 0\n"
			  << "-ghs | -genhullshape\t->\tgenerate hull shape for collision mesh\n"
			  << "-static\t->\tset static collision mesh\n"
			  << "-------------------------\n\n"
			  << "Supported formats:\n"
			  << "-------------------------\n";

	Assimp::Importer		import;
	std::string				extensionsList;
	import.GetExtensionList( extensionsList );

	if ( extensionsList.empty() )
		std::cout << "not supported formats, this is bug? :/\n";
	else
		std::cout << extensionsList << std::endl;
}

// ------------------------------------------------------------------------------------ //
// Parse arguments start
// ------------------------------------------------------------------------------------ //
void ParseArgs( int argc, char** argv )
{
	// Parse arguments
	for ( le::UInt32_t index = 1; index < argc; ++index )
	{
		// Help info
		if ( !strcmp( argv[ index ], "-h" ) || !strcmp( argv[ index ], "-help" ) )
		{
			PrintUsage();
			exit( 0 );
		}

		// Set convert type: mesh
		else if ( !strcmp( argv[ index ], "-m" ) || !strcmp( argv[ index ], "-mesh" ) )
		{
			g_convertType = CT_MESH;
		}

		// Set convert type: collision
		else if ( !strcmp( argv[ index ], "-c" ) || !strcmp( argv[ index ], "-collision" ) )
		{
			g_convertType = CT_COLLISION;
		}

		// Set output file
		else if ( ( !strcmp( argv[ index ], "-o" ) || !strcmp( argv[ index ], "-output" ) ) && index + 1 < argc )
		{
			g_outputFile = argv[ index + 1 ];
			++index;
		}

		// Set source file
		else if ( ( !strcmp( argv[ index ], "-s" ) || !strcmp( argv[ index ], "-source" ) ) && index + 1 < argc )
		{
			g_sourceFile = argv[ index + 1 ];
			++index;
		}
		
		// Set directory with materials
		else if ( !strcmp( argv[ index ], "-matdir" ) && index + 1 < argc )
		{
			g_materialsDir = argv[ index + 1 ];
			++index;
		}

		// Set mass for collision mesh
		else if ( !strcmp( argv[ index ], "-masa" ) && index + 1 < argc )
		{
			g_masa = atof( argv[ index + 1 ] );
			++index;
		}

		// Set inertia for collision mesh
		else if ( ( !strcmp( argv[ index ], "-i" ) || !strcmp( argv[ index ], "-inertia" ) ) && index + 3 < argc )
		{
			g_inertia.x = atof( argv[ index + 1 ] );
			g_inertia.y = atof( argv[ index + 2 ] );
			g_inertia.z = atof( argv[ index + 3 ] );
			index += 3;
		}

		// Is need generate hull shape
		else if ( !strcmp( argv[ index ], "-ghs" ) || !strcmp( argv[ index ], "-genhullshape" ) )
		{
			g_isGenHullShape = true;
		}

		// Set static collision mesh
		else if ( !strcmp( argv[ index ], "-static" ) )
		{
			g_isStaticCollision = true;
		}
	}
}

// ------------------------------------------------------------------------------------ //
// Main function
// ------------------------------------------------------------------------------------ //
int main( int argc, char** argv )
{
	if ( argc > 1 )
	{
		// Parse arguments
		ParseArgs( argc, argv );

		try
		{
			if ( g_sourceFile.empty() )			throw std::runtime_error( "Source file not set. Use argument -s <path>" );
			if ( g_outputFile.empty() )			throw std::runtime_error( "Output file not set. Use argument -o <path>" );

			switch ( g_convertType )
			{
			case CT_MESH:
			{
				std::cout << "Model: " << g_sourceFile << std::endl;

				if ( g_materialsDir.empty() )
				{
					std::cout << "Warning: material directory not set, set by default directory: materials. Use -matdir <name_dir> for set this dir\n";
					g_materialsDir = "materials/";
				}

				Mesh		mesh;
				mesh.Load( g_sourceFile );
				mesh.Save( g_outputFile );
				break;
			}

			case CT_COLLISION:
			{
				std::cout << "Collision: " << g_sourceFile << std::endl;

				Collision		collision;
				collision.Load( g_sourceFile );
				collision.Save( g_outputFile );
				break;
			}
			}
		}
		catch ( const std::exception& Exception )
		{
			std::cerr << "Error: " <<  Exception.what() << std::endl;
			return 1;
		}
	}
	else
		PrintUsage();

	return 0;
}

//---------------------------------------------------------------------//
