//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <fstream>

#include "mdldoc.h"

#define MDL_ID			"LMDL"
#define MDL_VERSION		1

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
MDLDoc::MDLDoc()
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
MDLDoc::~MDLDoc()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Load MDL
// ------------------------------------------------------------------------------------ //
bool MDLDoc::Load( const std::string& Path )
{
	std::ifstream				file( Path, std::ios::binary );
	if ( !file.is_open() )		return false;

	// Read header file
	MDLHeader			mdlHeader;
	file.read( ( char* ) &mdlHeader, sizeof( MDLHeader ) );
	if ( strncmp( mdlHeader.strId, MDL_ID, 4 ) != 0 || mdlHeader.version != MDL_VERSION )
		return false;

	// Read all lumps
	MDLLump				mdlLumps[ ML_MAX_LUMPS ];
	file.read( ( char* ) &mdlLumps[ 0 ], ML_MAX_LUMPS * sizeof( MDLLump ) );

	// Read materials
	{
		std::uint32_t			countMaterials;

		file.seekg( mdlLumps[ ML_MATERIALS ].offset, std::ios::beg );
		file.read( ( char* ) &countMaterials, sizeof( std::uint32_t ) );

		for ( std::uint32_t index = 0; index < countMaterials; ++index )
		{
			std::uint32_t		sizePath;
			std::string			path;

			file.read( ( char* ) &sizePath, sizeof( std::uint32_t ) );
			path.resize( sizePath );
			file.read( ( char* ) path.data(), sizePath );

			materials.push_back( path );
		}
	}

	// Read verteces
	{
		std::uint32_t			countVerteces;

		file.seekg( mdlLumps[ ML_VERTECES ].offset, std::ios::beg );
		file.read( ( char* ) &countVerteces, sizeof( std::uint32_t ) );

		verteces.resize( countVerteces );
		file.read( ( char* ) verteces.data(), countVerteces * sizeof( MDLVertex ) );
	}

	// Read indeces
	{
		std::uint32_t				countVertexIndeces;

		file.seekg( mdlLumps[ ML_INDECES ].offset, std::ios::beg );
		file.read( ( char* ) &countVertexIndeces, sizeof( std::uint32_t ) );

		vertexIndeces.resize( countVertexIndeces );
		file.read( ( char* ) vertexIndeces.data(), countVertexIndeces * sizeof( std::uint32_t ) );
	}

	// Read surfaces
	{
		std::uint32_t				countSurfaces;

		file.seekg( mdlLumps[ ML_SURFACES ].offset, std::ios::beg );
		file.read( ( char* ) &countSurfaces, sizeof( std::uint32_t ) );
		
		surfaces.resize( countSurfaces );
		file.read( ( char* ) surfaces.data(), countSurfaces * sizeof( MDLSurface ) );
	}

	// Find min xyz and max
	minXYZ = verteces[ 0 ].position;
	maxXYZ = verteces[ 0 ].position;

	for ( std::uint32_t index = 0; index < verteces.size(); ++index )
	{
		if ( minXYZ.x > verteces[ index ].position.x )			minXYZ.x = verteces[ index ].position.x;
		if ( minXYZ.y > verteces[ index ].position.y )			minXYZ.y = verteces[ index ].position.y;
		if ( minXYZ.z > verteces[ index ].position.z )			minXYZ.z = verteces[ index ].position.z;

		if ( maxXYZ.x < verteces[ index ].position.x )			maxXYZ.x = verteces[ index ].position.x;
		if ( maxXYZ.y < verteces[ index ].position.y )			maxXYZ.y = verteces[ index ].position.y;
		if ( maxXYZ.z < verteces[ index ].position.z )			maxXYZ.z = verteces[ index ].position.z;
	}

	return true;
}

// ------------------------------------------------------------------------------------ //
// Save MDL
// ------------------------------------------------------------------------------------ //
bool MDLDoc::Save( const std::string& Path )
{
	std::ofstream				file( Path, std::ios::binary );
	if ( !file.is_open() )		return false;

	// Write header fille
	MDLHeader			mdlHeader;
	memcpy( mdlHeader.strId, MDL_ID, sizeof( char ) * 4 );
	mdlHeader.version = MDL_VERSION;
	file.write( ( char* ) &mdlHeader, sizeof( MDLHeader ) );

	// Write null info lumps
	MDLLump				mdlLumps[ ML_MAX_LUMPS ];
	std::streampos		offsetToLumps = file.tellp();
	memset( mdlLumps, 0, ML_MAX_LUMPS * sizeof( MDLLump ) );
	file.write( ( char* ) &mdlLumps[ 0 ], ML_MAX_LUMPS * sizeof( MDLLump ) );

	// Write materials
	WriteMaterials( file, mdlLumps[ ML_MATERIALS ] );

	// Write verteces
	WriteVerteces( file, mdlLumps[ ML_VERTECES ] );

	// Write indeces
	WriteIndeces( file, mdlLumps[ ML_INDECES ] );

	// Write surfaces
	WriteSurfaces( file, mdlLumps[ ML_SURFACES ] );

	// Update lumps
	UpdateLumps( file, &mdlLumps[ 0 ], offsetToLumps );

	return true;
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void MDLDoc::Clear()
{
	minXYZ = MDLVector3D();
	maxXYZ = MDLVector3D();

	verteces.clear();
	vertexIndeces.clear();
	surfaces.clear();
	materials.clear();
}

// ------------------------------------------------------------------------------------ //
// Add vertex
// ------------------------------------------------------------------------------------ //
void MDLDoc::AddVertex( const MDLVertex& Vertex )
{
	verteces.push_back( Vertex );
}

// ------------------------------------------------------------------------------------ //
// Add vertex index
// ------------------------------------------------------------------------------------ //
void MDLDoc::AddVertexIndex( std::uint32_t VertexIndex )
{
	vertexIndeces.push_back( VertexIndex );
}

// ------------------------------------------------------------------------------------ //
// Add surface
// ------------------------------------------------------------------------------------ //
void MDLDoc::AddSurface( const MDLSurface& Surface )
{
	surfaces.push_back( Surface );
}

// ------------------------------------------------------------------------------------ //
// Add material
// ------------------------------------------------------------------------------------ //
void MDLDoc::AddMaterial( const std::string& MaterialPath )
{
	materials.push_back( MaterialPath );
}

// ------------------------------------------------------------------------------------ //
// Remove vertex
// ------------------------------------------------------------------------------------ //
void MDLDoc::RemoveVertex( std::uint32_t Index )
{
	if ( Index >= verteces.size() ) return;
	verteces.erase( verteces.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Remove vertex index
// ------------------------------------------------------------------------------------ //
void MDLDoc::RemoveVertexIndex( std::uint32_t Index )
{
	if ( Index >= vertexIndeces.size() ) return;
	vertexIndeces.erase( vertexIndeces.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Remove surface
// ------------------------------------------------------------------------------------ //
void MDLDoc::RemoveSurface( std::uint32_t Index )
{
	if ( Index >= surfaces.size() ) return;
	surfaces.erase( surfaces.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Remove material
// ------------------------------------------------------------------------------------ //
void MDLDoc::RemoveMaterial( std::uint32_t Index )
{
	if ( Index >= materials.size() ) return;
	materials.erase( materials.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Set min XYZ
// ------------------------------------------------------------------------------------ //
void MDLDoc::SetMinXYZ( const MDLVector3D& MinXYZ )
{
	minXYZ = MinXYZ;
}

// ------------------------------------------------------------------------------------ //
// Set max XYZ
// ------------------------------------------------------------------------------------ //
void MDLDoc::SetMaxXYZ( const MDLVector3D& MaxXYZ )
{
	maxXYZ = MaxXYZ;
}

// ------------------------------------------------------------------------------------ //
// Set vertex
// ------------------------------------------------------------------------------------ //
void MDLDoc::SetVertex( const MDLVertex& Vertex, std::uint32_t Index )
{
	if ( Index >= verteces.size() ) return;
	verteces[ Index ] = Vertex;
}

// ------------------------------------------------------------------------------------ //
// Set vertex index
// ------------------------------------------------------------------------------------ //
void MDLDoc::SetVertexIndex( std::uint32_t VertexIndex, std::uint32_t Index )
{
	if ( Index >= vertexIndeces.size() ) return;
	vertexIndeces[ Index ] = VertexIndex;
}

// ------------------------------------------------------------------------------------ //
// Set surface
// ------------------------------------------------------------------------------------ //
void MDLDoc::SetSurface( const MDLSurface& Surface, std::uint32_t Index )
{
	if ( Index >= surfaces.size() ) return;
	surfaces[ Index ] = Surface;
}

// ------------------------------------------------------------------------------------ //
// Set material
// ------------------------------------------------------------------------------------ //
void MDLDoc::SetMaterial( const std::string& MaterialPath, std::uint32_t Index )
{
	if ( Index >= materials.size() ) return;
	materials[ Index ] = MaterialPath;
}

// ------------------------------------------------------------------------------------ //
// Set verteces
// ------------------------------------------------------------------------------------ //
void MDLDoc::SetVerteces( const std::vector<MDLVertex>& Verteces )
{
	verteces = Verteces;
}

// ------------------------------------------------------------------------------------ //
// Set vertex indeces
// ------------------------------------------------------------------------------------ //
void MDLDoc::SetVertexIndeces( const std::vector<std::uint32_t>& VertexIndeces )
{
	vertexIndeces = VertexIndeces;
}

// ------------------------------------------------------------------------------------ //
// Set surfaces
// ------------------------------------------------------------------------------------ //
void MDLDoc::SetSurfaces( const std::vector<MDLSurface>& Surfaces )
{
	surfaces = Surfaces;
}

// ------------------------------------------------------------------------------------ //
// Set materials
// ------------------------------------------------------------------------------------ //
void MDLDoc::SetMaterials( const std::vector<std::string>& MaterialPaths )
{
	materials = MaterialPaths;
}

// ------------------------------------------------------------------------------------ //
// Get min XYZ
// ------------------------------------------------------------------------------------ //
const MDLVector3D& MDLDoc::GetMinXYZ() const
{
	return minXYZ;
}

// ------------------------------------------------------------------------------------ //
// Get max XYZ
// ------------------------------------------------------------------------------------ //
const MDLVector3D& MDLDoc::GetMaxXYZ() const
{
	return maxXYZ;
}

// ------------------------------------------------------------------------------------ //
// Get count verteces
// ------------------------------------------------------------------------------------ //
std::uint32_t MDLDoc::GetCountVerteces() const
{
	return verteces.size();
}

// ------------------------------------------------------------------------------------ //
// Get count vertex indeces
// ------------------------------------------------------------------------------------ //
std::uint32_t MDLDoc::GetCountVertexIndeces() const
{
	return vertexIndeces.size();
}

// ------------------------------------------------------------------------------------ //
// Get count surfaces
// ------------------------------------------------------------------------------------ //
std::uint32_t MDLDoc::GetCountSurfaces() const
{
	return surfaces.size();
}

// ------------------------------------------------------------------------------------ //
// Get count materials
// ------------------------------------------------------------------------------------ //
std::uint32_t MDLDoc::GetCountMaterials() const
{
	return materials.size();
}

// ------------------------------------------------------------------------------------ //
// Get vertex
// ------------------------------------------------------------------------------------ //
MDLVertex MDLDoc::GetVertex( std::uint32_t Index ) const
{
	if ( Index >= verteces.size() ) 
		return MDLVertex();

	return verteces[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get vertex index
// ------------------------------------------------------------------------------------ //
std::uint32_t MDLDoc::GetVertexIndex( std::uint32_t Index ) const
{
	if ( Index >= vertexIndeces.size() )
		return 0;

	return vertexIndeces[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get surface
// ------------------------------------------------------------------------------------ //
MDLSurface MDLDoc::GetSurface( std::uint32_t Index ) const
{
	if ( Index >= surfaces.size() )
		return MDLSurface();

	return surfaces[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get material
// ------------------------------------------------------------------------------------ //
std::string MDLDoc::GetMaterial( std::uint32_t Index ) const
{
	if ( Index >= materials.size() )
		return "";

	return materials[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get verteces
// ------------------------------------------------------------------------------------ //
const std::vector<MDLVertex>& MDLDoc::GetVerteces() const
{
	return verteces;
}

// ------------------------------------------------------------------------------------ //
// Get vertex indeces
// ------------------------------------------------------------------------------------ //
const std::vector<std::uint32_t>& MDLDoc::GetVertexIndeces() const
{
	return vertexIndeces;
}

// ------------------------------------------------------------------------------------ //
// Get surfaces
// ------------------------------------------------------------------------------------ //
const std::vector<MDLSurface>& MDLDoc::GetSurfaces() const
{
	return surfaces;
}

// ------------------------------------------------------------------------------------ //
// Get materials
// ------------------------------------------------------------------------------------ //
const std::vector<std::string>& MDLDoc::GetMaterials() const
{
	return materials;
}

// ------------------------------------------------------------------------------------ //
// Update lumps in file
// ------------------------------------------------------------------------------------ //
void MDLDoc::UpdateLumps( std::ofstream& File, MDLLump* Lumps, std::streampos OffsetToLumps )
{
	File.seekp( OffsetToLumps, std::ios::beg );
	File.write( ( char* ) &Lumps[ 0 ], sizeof( MDLLump ) * ML_MAX_LUMPS );
}

// ------------------------------------------------------------------------------------ //
// Write materials to file
// ------------------------------------------------------------------------------------ //
void MDLDoc::WriteMaterials( std::ofstream& File, MDLLump& Lump )
{
	std::uint32_t			countMaterials = materials.size();
	std::uint32_t			totalSizeLump = countMaterials * sizeof( std::uint32_t );

	for ( std::uint32_t index = 0; index < countMaterials; ++index )
		totalSizeLump += materials[ index ].size();

	Lump.offset = File.tellp();
	Lump.length = totalSizeLump;

	File.write( ( char* ) &countMaterials, sizeof( std::uint32_t ) );
	for ( std::uint32_t index = 0; index < countMaterials; ++index )
	{
		std::string&			path = materials[ index ];
		std::uint32_t			sizePath = path.size();

		File.write( ( char* ) &sizePath, sizeof( std::uint32_t ) );
		File.write( path.data(), sizePath * sizeof( char ) );
	}
}

// ------------------------------------------------------------------------------------ //
// Write verteces to file
// ------------------------------------------------------------------------------------ //
void MDLDoc::WriteVerteces( std::ofstream& File, MDLLump& Lump )
{
	std::uint32_t		countVerteces = verteces.size();
	Lump.offset = File.tellp();
	Lump.length = countVerteces * sizeof( MDLVertex );

	File.write( ( char* ) &countVerteces, sizeof( std::uint32_t ) );
	File.write( ( char* ) &verteces[ 0 ], countVerteces * sizeof( MDLVertex ) );
}

// ------------------------------------------------------------------------------------ //
// Write indeces to file
// ------------------------------------------------------------------------------------ //
void MDLDoc::WriteIndeces( std::ofstream& File, MDLLump& Lump )
{
	std::uint32_t		countIndeces = vertexIndeces.size();
	Lump.offset = File.tellp();
	Lump.length = countIndeces * sizeof( std::uint32_t );

	File.write( ( char* ) &countIndeces, sizeof( std::uint32_t ) );
	File.write( ( char* ) &vertexIndeces[ 0 ], countIndeces * sizeof( std::uint32_t ) );
}

// ------------------------------------------------------------------------------------ //
// Write surfaces to file
// ------------------------------------------------------------------------------------ //
void MDLDoc::WriteSurfaces( std::ofstream& File, MDLLump& Lump )
{
	std::uint32_t		countSurfaces = surfaces.size();
	Lump.offset = File.tellp();
	Lump.length = countSurfaces * sizeof( MDLSurface );

	File.write( ( char* ) &countSurfaces, sizeof( std::uint32_t ) );
	File.write( ( char* ) &surfaces[ 0 ], countSurfaces * sizeof( MDLSurface ) );
}