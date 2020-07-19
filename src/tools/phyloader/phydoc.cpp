//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "phydoc.h"

#define PHY_ID			"LPHY"
#define PHY_VERSION		2

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
PHYDoc::PHYDoc() :
	isStatic( false ),
	masa( 0.f ),
	inertia( 0.f, 0.f, 0.f )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
PHYDoc::~PHYDoc()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Load PHY
// ------------------------------------------------------------------------------------ //
bool PHYDoc::Load( const std::string& Path )
{
	std::ifstream				file( Path, std::ios::binary );
	if ( !file.is_open() )		return false;

	// Read header file
	PHYHeader			phyHeader;
	file.read( ( char* ) &phyHeader, sizeof( PHYHeader ) );
	if ( strncmp( phyHeader.strId, PHY_ID, 4 ) != 0 || phyHeader.version != PHY_VERSION )
		return false;

	// Read all lumps
	PHYLump				phyLumps[ PL_MAX_LUMPS ];
	file.read( ( char* ) &phyLumps[ 0 ], PL_MAX_LUMPS * sizeof( PHYLump ) );

	// Read parameters
	{
		PHYParameters		phyParameters;

		file.seekg( phyLumps[ PL_PARAMETERS ].offset, std::ios::beg );
		file.read( ( char* ) &phyParameters, sizeof( PHYParameters ) );

		isStatic = phyParameters.isStatic;
		masa = phyParameters.masa;
		inertia = phyParameters.inertia;
	}

	// Read verteces
	{
		std::uint32_t			countVerteces;

		file.seekg( phyLumps[ PL_VERTECES ].offset, std::ios::beg );
		file.read( ( char* ) &countVerteces, sizeof( std::uint32_t ) );

		verteces.resize( countVerteces );
		file.read( ( char* ) verteces.data(), countVerteces * sizeof( PHYVector3D ) );
	}

	// Read vertex indeces
	{
		std::uint32_t					countVertexIndeces;

		file.seekg( phyLumps[ PL_INDECES ].offset, std::ios::beg );
		file.read( ( char* ) &countVertexIndeces, sizeof( std::uint32_t ) );

		vertexIndeces.resize( countVertexIndeces );
		file.read( ( char* ) vertexIndeces.data(), countVertexIndeces * sizeof( std::uint32_t ) );
	}

	if ( verteces.empty() )		return false;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Save PHY
// ------------------------------------------------------------------------------------ //
bool PHYDoc::Save( const std::string& Path )
{
	std::ofstream						file( Path, std::ios::binary );
	if ( !file.is_open() )				return false;

	// Write header fille
	PHYHeader			phyHeader;
	memcpy( phyHeader.strId, PHY_ID, sizeof( char ) * 4 );
	phyHeader.version = PHY_VERSION;
	file.write( ( char* ) &phyHeader, sizeof( PHYHeader ) );

	// Write null info lumps
	PHYLump				phyLumps[ PL_MAX_LUMPS ];
	std::uint32_t		offsetToLumps = file.tellp();
	memset( phyLumps, 0, sizeof( PHYLump ) * PL_MAX_LUMPS );
	file.write( ( char* ) &phyLumps[ 0 ], sizeof( PHYLump ) * PL_MAX_LUMPS );

	// Write parameters
	WriteParameters( file, phyLumps[ PL_PARAMETERS ] );

	// Write verteces
	WriteVerteces( file, phyLumps[ PL_VERTECES ] );

	// Write indeces
	WriteIndeces( file, phyLumps[ PL_INDECES ] );

	// Update lumps
	UpdateLumps( file, &phyLumps[ 0 ], offsetToLumps );

	return true;
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void PHYDoc::Clear()
{
	verteces.clear();
	vertexIndeces.clear();
}

// ------------------------------------------------------------------------------------ //
// Add vertex
// ------------------------------------------------------------------------------------ //
void PHYDoc::AddVertex( const PHYVector3D& Vertex )
{
	verteces.push_back( Vertex );
}

// ------------------------------------------------------------------------------------ //
// Add vertex index
// ------------------------------------------------------------------------------------ //
void PHYDoc::AddVertexIndex( std::uint32_t VertexIndex )
{
	vertexIndeces.push_back( VertexIndex );
}

// ------------------------------------------------------------------------------------ //
// Remove vertex
// ------------------------------------------------------------------------------------ //
void PHYDoc::RemoveVertex( std::uint32_t Index )
{
	if ( Index >= verteces.size() ) return;
	verteces.erase( verteces.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Remove vertex index
// ------------------------------------------------------------------------------------ //
void PHYDoc::RemoveVertexIndex( std::uint32_t Index )
{
	if ( Index >= vertexIndeces.size() ) return;
	vertexIndeces.erase( vertexIndeces.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Set static model
// ------------------------------------------------------------------------------------ //
void PHYDoc::SetStatic( bool IsStatic )
{
	isStatic = IsStatic;
}

// ------------------------------------------------------------------------------------ //
// Set mass
// ------------------------------------------------------------------------------------ //
void PHYDoc::SetMasa( float Masa )
{
	masa = Masa;
}

// ------------------------------------------------------------------------------------ //
// Set inertia
// ------------------------------------------------------------------------------------ //
void PHYDoc::SetInertia( const PHYVector3D& Inertia )
{
	inertia = Inertia;
}

// ------------------------------------------------------------------------------------ //
// Set vertex
// ------------------------------------------------------------------------------------ //
void PHYDoc::SetVertex( const PHYVector3D& Vertex, std::uint32_t Index )
{
	if ( Index >= verteces.size() ) return;
	verteces[ Index ] = Vertex;
}

// ------------------------------------------------------------------------------------ //
// Set vertex index
// ------------------------------------------------------------------------------------ //
void PHYDoc::SetVertexIndex( std::uint32_t VertexIndex, std::uint32_t Index )
{
	if ( Index >= vertexIndeces.size() ) return;
	vertexIndeces[ Index ] = VertexIndex;
}

// ------------------------------------------------------------------------------------ //
// Set verteces
// ------------------------------------------------------------------------------------ //
void PHYDoc::SetVerteces( const std::vector<PHYVector3D>& Verteces )
{
	verteces = Verteces;
}

// ------------------------------------------------------------------------------------ //
// Set vertex indeces
// ------------------------------------------------------------------------------------ //
void PHYDoc::SetVertexIndeces( const std::vector<std::uint32_t>& VertexIndeces )
{
	vertexIndeces = VertexIndeces;
}

// ------------------------------------------------------------------------------------ //
// Is static model
// ------------------------------------------------------------------------------------ //
bool PHYDoc::IsStatic() const
{
	return isStatic;
}

// ------------------------------------------------------------------------------------ //
// Get masa
// ------------------------------------------------------------------------------------ //
float PHYDoc::GetMasa() const
{
	return masa;
}

// ------------------------------------------------------------------------------------ //
// Get inertia
// ------------------------------------------------------------------------------------ //
const PHYVector3D& PHYDoc::GetInertia() const
{
	return inertia;
}

// ------------------------------------------------------------------------------------ //
// Get count verteces
// ------------------------------------------------------------------------------------ //
std::uint32_t PHYDoc::GetCountVerteces() const
{
	return verteces.size();
}

// ------------------------------------------------------------------------------------ //
// Get count vertex indeces
// ------------------------------------------------------------------------------------ //
std::uint32_t PHYDoc::GetCountVertexIndeces() const
{
	return vertexIndeces.size();
}

// ------------------------------------------------------------------------------------ //
// Get vertex
// ------------------------------------------------------------------------------------ //
PHYVector3D PHYDoc::GetVertex( std::uint32_t Index ) const
{
	if ( Index >= verteces.size() ) return PHYVector3D();
	return verteces[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get vertex index
// ------------------------------------------------------------------------------------ //
std::uint32_t PHYDoc::GetVertexIndex( std::uint32_t Index ) const
{
	if ( Index >= vertexIndeces.size() ) return 0;
	return vertexIndeces[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get verteces
// ------------------------------------------------------------------------------------ //
const std::vector<PHYVector3D>& PHYDoc::GetVerteces() const
{
	return verteces;
}

// ------------------------------------------------------------------------------------ //
// Get vertex indeces
// ------------------------------------------------------------------------------------ //
const std::vector<std::uint32_t>& PHYDoc::GetVertexIndeces() const
{
	return vertexIndeces;
}

// ------------------------------------------------------------------------------------ //
// Update lumps in file
// ------------------------------------------------------------------------------------ //
void PHYDoc::UpdateLumps( std::ofstream& File, PHYLump* Lumps, std::uint32_t OffsetToLumps )
{
	File.seekp( OffsetToLumps, std::ios::beg );
	File.write( ( char* ) &Lumps[ 0 ], PL_MAX_LUMPS * sizeof( PHYLump ) );
}

// ------------------------------------------------------------------------------------ //
// Write parameters in file
// ------------------------------------------------------------------------------------ //
void PHYDoc::WriteParameters( std::ofstream& File, PHYLump& Lump )
{
	Lump.offset = File.tellp();
	Lump.length = sizeof( PHYParameters );

	PHYParameters			phyParameters;
	phyParameters.isStatic = isStatic;
	phyParameters.masa = masa;
	phyParameters.inertia = inertia;
	File.write( ( char* ) &phyParameters, sizeof( PHYParameters ) );
}

// ------------------------------------------------------------------------------------ //
// Write verteces in file
// ------------------------------------------------------------------------------------ //
void PHYDoc::WriteVerteces( std::ofstream& File, PHYLump& Lump )
{
	std::uint32_t		countVerteces = verteces.size();
	Lump.offset = File.tellp();
	Lump.length = countVerteces * sizeof( PHYVector3D );

	File.write( ( char* ) &countVerteces, sizeof( std::uint32_t ) );
	File.write( ( char* ) &verteces[ 0 ], countVerteces * sizeof( PHYVector3D ) );
}

// ------------------------------------------------------------------------------------ //
// Write indeces in file
// ------------------------------------------------------------------------------------ //
void PHYDoc::WriteIndeces( std::ofstream& File, PHYLump& Lump )
{
	std::uint32_t		countIndeces = vertexIndeces.size();
	Lump.offset = File.tellp();
	Lump.length = countIndeces * sizeof( std::uint32_t );

	File.write( ( char* ) &countIndeces, sizeof( std::uint32_t ) );
	File.write( ( char* ) &vertexIndeces[ 0 ], countIndeces * sizeof( std::uint32_t ) );
}