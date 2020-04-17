//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include "common/types.h"
#include "global.h"
#include "mesh.h"

#define MDL_ID			"LMDL"
#define MDL_VERSION		1

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Mesh::Mesh() :
	isLoaded( false )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Mesh::~Mesh()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Load mesh
// ------------------------------------------------------------------------------------ //
void Mesh::Load( const std::string& Path )
{
	std::cout << "Model loading\n";

	if ( Path.empty() )		throw std::runtime_error( "Path to mesh is empty" );

	// Loading mesh with help Assimp
	Assimp::Importer		import;
	const aiScene*			scene = import.ReadFile( Path.c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_LimitBoneWeights | aiProcess_Triangulate );

	// If mesh not loaded - exit,
	// and if mesh alredy loaded - clear
	if ( !scene )			throw std::runtime_error( import.GetErrorString() );
	if ( isLoaded )			Clear();

	std::unordered_map<le::UInt32_t, std::vector<aiMesh*>>			meshes;
	ProcessNode( scene->mRootNode, scene, meshes );
	if ( meshes.empty() )	throw std::runtime_error( "In file not found meshes" );

	// Go through the material ID, take the mesh and write its vertices, and indices
	// to the shared buffer
	MDLSurface					surface;
	MDLVertex					vertex;
	std::vector<MDLVertex>		vertexBuffer;
	for ( auto itRoot = meshes.begin(), itRootEnd = meshes.end(); itRoot != itRootEnd; ++itRoot )
	{
		surface.startVertexIndex = indices.size();
		surface.materialId = materials.size();

		for ( auto itMesh = itRoot->second.begin(), itMeshEnd = itRoot->second.end(); itMesh != itMeshEnd; ++itMesh )
		{
			aiMesh*			mesh = *itMesh;

			// Prepare the vertex buffer.
			// If the vertices of the mesh do not fit into the buffer, then
			// expand it
			if ( vertexBuffer.size() < mesh->mNumVertices )
				vertexBuffer.resize( vertexBuffer.size() + mesh->mNumVertices );

			// Read all verteces
			for ( le::UInt32_t index = 0; index < mesh->mNumVertices; ++index )
			{
				aiVector3D		tempVector = mesh->mVertices[ index ];
				vertex.position[ 0 ] = tempVector.x;
				vertex.position[ 1 ] = tempVector.y;
				vertex.position[ 2 ] = tempVector.z;

				tempVector = mesh->mNormals[ index ];
				vertex.normal[ 0 ] = tempVector.x;
				vertex.normal[ 1 ] = tempVector.y;
				vertex.normal[ 2 ] = tempVector.z;

				if ( mesh->mTangents )
				{
					tempVector = mesh->mTangents[ index ];
					vertex.tangent[ 0 ] = tempVector.x;
					vertex.tangent[ 1 ] = tempVector.y;
					vertex.tangent[ 2 ] = tempVector.z;
				}

				if ( mesh->mBitangents )
				{
					tempVector = mesh->mBitangents[ index ];
					vertex.bitangent[ 0 ] = tempVector.x;
					vertex.bitangent[ 1 ] = tempVector.y;
					vertex.bitangent[ 2 ] = tempVector.z;
				}

				if ( mesh->mTextureCoords[ 0 ] )
				{
					tempVector = mesh->mTextureCoords[ 0 ][ index ];
					vertex.texCoords[ 0 ] = tempVector.x;
					vertex.texCoords[ 1 ] = tempVector.y;
				}

				vertexBuffer[ index ] = vertex;
			}

			// Read all indeces
			for ( le::UInt32_t index = 0; index < mesh->mNumFaces; ++index )
			{
				aiFace*			face = &mesh->mFaces[ index ];

				for ( uint32_t indexVertex = 0; indexVertex < face->mNumIndices; ++indexVertex )
				{
					uint32_t		index = face->mIndices[ indexVertex ];
					auto			it = find( verteces.begin(), verteces.end(), vertexBuffer[ index ] );

					// Look for the vertex index in the shared vertex buffer,
					// if not found, add the vertex to the buffer,
					// and then write its index
					if ( it == verteces.end() )
					{
						indices.push_back( verteces.size() );
						verteces.push_back( vertexBuffer[ index ] );
					}
					else
						indices.push_back( it - verteces.begin() );
				}
			}

			// We process material
			if ( itRoot->first < scene->mNumMaterials )
			{
				aiMaterial*			material = scene->mMaterials[ itRoot->first ];

				aiString			tmp_nameMaterial;
				std::string			finelName = g_materialsDir + "/";
				material->Get( AI_MATKEY_NAME, tmp_nameMaterial );

				if ( tmp_nameMaterial.length > 0 )
					finelName += tmp_nameMaterial.C_Str();
				else
					finelName += std::string( "unknow" ) + std::to_string( materials.size() );

				finelName += ".lmt";
				materials.push_back( finelName );
			}
			else
			{
				std::cout << "Warning: material with id " << itRoot->first << " large. Surface not created\n";
				continue;
			}

			surface.countVertexIndex = indices.size() - surface.startVertexIndex;
			surfaces.push_back( surface );
		}
	}

	import.FreeScene();
	isLoaded = true;

	std::cout << "Model loaded\n";
}

// ------------------------------------------------------------------------------------ //
// Save mesh
// ------------------------------------------------------------------------------------ //
void Mesh::Save( const std::string& Path )
{
	if ( !isLoaded )					throw std::runtime_error( "Mesh not loaded" );
	std::ofstream						file( Path + ".mdl", std::ios::binary );
	if ( !file.is_open() )				throw std::runtime_error( "Failed create file" );

	// Write header fille
	MDLHeader			mdlHeader;
	memcpy( mdlHeader.strId, MDL_ID, sizeof( char ) * 4 );
	mdlHeader.version = MDL_VERSION;
	file.write( ( char* ) &mdlHeader, sizeof( MDLHeader ) );

	// Write null info lumps
	MDLLump				mdlLumps[ ML_MAX_LUMPS ];
	le::UInt32_t		offsetToLumps = file.tellp();
	memset( mdlLumps, 0, sizeof( MDLLump ) * ML_MAX_LUMPS );
	file.write( ( char* ) &mdlLumps[ 0 ], sizeof( MDLLump ) * ML_MAX_LUMPS );

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

	std::cout << "Model saved\n";
}

// ------------------------------------------------------------------------------------ //
// Clear mesh
// ------------------------------------------------------------------------------------ //
void Mesh::Clear()
{
	verteces.clear();
	indices.clear();
	materials.clear();
	surfaces.clear();
	isLoaded = false;
}

// ----------------------------------------------------------------------------------- //
// Process node scene in assimp
// ------------------------------------------------------------------------------------ //
void Mesh::ProcessNode( aiNode* Node, const aiScene* Scene, std::unordered_map<le::UInt32_t, std::vector< aiMesh* > >& Meshes )
{
	for ( le::UInt32_t index = 0; index < Node->mNumMeshes; ++index )
		Meshes[ Scene->mMeshes[ Node->mMeshes[ index ] ]->mMaterialIndex ].push_back( Scene->mMeshes[ Node->mMeshes[ index ] ] );

	for ( le::UInt32_t index = 0; index < Node->mNumChildren; ++index )
		ProcessNode( Node->mChildren[ index ], Scene, Meshes );
}

// ------------------------------------------------------------------------------------ //
// Update lumps in file
// ------------------------------------------------------------------------------------ //
void Mesh::UpdateLumps( std::ofstream& File, Mesh::MDLLump* Lumps, le::UInt32_t OffsetToLumps )
{
	File.seekp( OffsetToLumps, std::ios::beg );
	File.write( ( char* ) &Lumps[ 0 ], sizeof( MDLLump ) * ML_MAX_LUMPS );
}

// ------------------------------------------------------------------------------------ //
// Write materials to file
// ------------------------------------------------------------------------------------ //
void Mesh::WriteMaterials( std::ofstream& File, Mesh::MDLLump& Lump )
{
	le::UInt32_t			countMaterials = materials.size();
	le::UInt32_t			totalSizeLump = countMaterials * sizeof( le::UInt32_t );

	for ( le::UInt32_t index = 0; index < countMaterials; ++index )
		totalSizeLump += materials[ index ].size();

	Lump.offset = File.tellp();
	Lump.length = totalSizeLump;

	File.write( ( char* ) &countMaterials, sizeof( le::UInt32_t ) );
	for ( le::UInt32_t index = 0; index < countMaterials; ++index )
	{
		std::string&			path = materials[ index ];
		le::UInt32_t			sizePath = path.size();

		File.write( ( char* ) &sizePath, sizeof( le::UInt32_t ) );
		File.write( path.data(), sizePath * sizeof( char ) );
	}
}

// ------------------------------------------------------------------------------------ //
// Write verteces to file
// ------------------------------------------------------------------------------------ //
void Mesh::WriteVerteces( std::ofstream& File, Mesh::MDLLump& Lump )
{
	le::UInt32_t		countVerteces = verteces.size();
	Lump.offset = File.tellp();
	Lump.length = countVerteces * sizeof( MDLVertex );

	File.write( ( char* ) &countVerteces, sizeof( le::UInt32_t ) );
	File.write( ( char* ) &verteces[ 0 ], countVerteces * sizeof( MDLVertex ) );
}

// ------------------------------------------------------------------------------------ //
// Write indeces to file
// ------------------------------------------------------------------------------------ //
void Mesh::WriteIndeces( std::ofstream& File, Mesh::MDLLump& Lump )
{
	le::UInt32_t		countIndeces = indices.size();
	Lump.offset = File.tellp();
	Lump.length = countIndeces * sizeof( le::UInt32_t );

	File.write( ( char* ) &countIndeces, sizeof( le::UInt32_t ) );
	File.write( ( char* ) &indices[ 0 ], countIndeces * sizeof( le::UInt32_t ) );
}

// ------------------------------------------------------------------------------------ //
// Write surfaces to file
// ------------------------------------------------------------------------------------ //
void Mesh::WriteSurfaces( std::ofstream& File, Mesh::MDLLump& Lump )
{
	le::UInt32_t		countSurfaces = surfaces.size();
	Lump.offset = File.tellp();
	Lump.length = countSurfaces * sizeof( MDLSurface );

	File.write( ( char* ) &countSurfaces, sizeof( le::UInt32_t ) );
	File.write( ( char* ) &surfaces[ 0 ], countSurfaces * sizeof( MDLSurface ) );
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Mesh::MDLVertex::MDLVertex()
{
	position[ 0 ] = 0.f;
	position[ 1 ] = 0.f;
	position[ 2 ] = 0.f;

	normal[ 0 ] = 0.f;
	normal[ 1 ] = 0.f;
	normal[ 2 ] = 0.f;

	texCoords[ 0 ] = 0.f;
	texCoords[ 1 ] = 0.f;

	tangent[ 0 ] = 0.f;
	tangent[ 1 ] = 0.f;
	tangent[ 2 ] = 0.f;

	bitangent[ 0 ] = 0.f;
	bitangent[ 1 ] = 0.f;
	bitangent[ 2 ] = 0.f;
}

// ------------------------------------------------------------------------------------ //
// Operator ==
// ------------------------------------------------------------------------------------ //
bool Mesh::MDLVertex::operator==( const MDLVertex& Right )
{
	return
			position[ 0 ] == Right.position[ 0 ] &&
			position[ 1 ] == Right.position[ 1 ] &&
			position[ 2 ] == Right.position[ 2 ] &&

			normal[ 0 ] == Right.normal[ 0 ] &&
			normal[ 1 ] == Right.normal[ 1 ] &&
			normal[ 2 ] == Right.normal[ 2 ] &&

			texCoords[ 0 ] == Right.texCoords[ 0 ] &&
			texCoords[ 1 ] == Right.texCoords[ 1 ] &&

			tangent[ 0 ] == Right.tangent[ 0 ] &&
			tangent[ 1 ] == Right.tangent[ 1 ] &&
			tangent[ 2 ] == Right.tangent[ 2 ] &&

			bitangent[ 0 ] == Right.bitangent[ 0 ] &&
			bitangent[ 1 ] == Right.bitangent[ 1 ] &&
			bitangent[ 2 ] == Right.bitangent[ 2 ];
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Mesh::MDLSurface::MDLSurface() :
	materialId( 0 ),
	startVertexIndex( 0 ),
	countVertexIndex( 0 )
{}
