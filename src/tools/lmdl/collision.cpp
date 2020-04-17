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
#include "collision.h"

#define PHY_ID			"LPHY"
#define PHY_VERSION		1

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Collision::Collision() :
	isLoaded( false )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Collision::~Collision()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Load collision
// ------------------------------------------------------------------------------------ //
void Collision::Load( const std::string& Path )
{
	std::cout << "Collision mesh loading\n";

	if ( Path.empty() )		throw std::runtime_error( "Path to collision mesh is empty" );

	// Loading collision mesh with help Assimp
	Assimp::Importer		import;
	const aiScene*			scene = import.ReadFile( Path.c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_LimitBoneWeights | aiProcess_Triangulate );

	// If collision mesh not loaded - exit,
	// and if collision mesh alredy loaded - clear
	if ( !scene )			throw std::runtime_error( import.GetErrorString() );
	if ( isLoaded )			Clear();

	std::unordered_map<le::UInt32_t, std::vector<aiMesh*>>			meshes;
	ProcessNode( scene->mRootNode, scene, meshes );
	if ( meshes.empty() )	throw std::runtime_error( "In file not found meshes" );

	// Getting array verteces and indeces from collision mesh
	PHYVertex					vertex;
	std::vector<PHYVertex>		vertexBuffer;

	for ( auto itRoot = meshes.begin(), itRootEnd = meshes.end(); itRoot != itRootEnd; ++itRoot )
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
		}

	import.FreeScene();
	isLoaded = true;

	std::cout << "Collision mesh loaded\n";
}

// ------------------------------------------------------------------------------------ //
// Save collision
// ------------------------------------------------------------------------------------ //
void Collision::Save( const std::string& Path )
{
	if ( !isLoaded )					throw std::runtime_error( "Collision mesh not loaded" );
	std::ofstream						file( Path + ".phy", std::ios::binary );
	if ( !file.is_open() )				throw std::runtime_error( "Failed create file" );

	// Write header fille
	PHYHeader			phyHeader;
	memcpy( phyHeader.strId, PHY_ID, sizeof( char ) * 4 );
	phyHeader.version = PHY_VERSION;
	file.write( ( char* ) &phyHeader, sizeof( PHYHeader ) );

	// Write null info lumps
	PHYLump				phyLumps[ PL_MAX_LUMPS ];
	le::UInt32_t		offsetToLumps = file.tellp();
	memset( phyLumps, 0, sizeof( PHYLump ) * PL_MAX_LUMPS );
	file.write( ( char* ) &phyLumps[ 0 ], sizeof( PHYLump ) * PL_MAX_LUMPS );

	// Write verteces
	WriteVerteces( file, phyLumps[ PL_VERTECES ] );

	// Write indeces
	WriteIndeces( file, phyLumps[ PL_INDECES ] );

	// Update lumps
	UpdateLumps( file, &phyLumps[ 0 ], offsetToLumps );

	std::cout << "Collision mesh saved\n";
}

// ------------------------------------------------------------------------------------ //
// Clear collision
// ------------------------------------------------------------------------------------ //
void Collision::Clear()
{
	verteces.clear();
	indices.clear();
	isLoaded = false;
}

// ------------------------------------------------------------------------------------ //
// Process node
// ------------------------------------------------------------------------------------ //
void Collision::ProcessNode( aiNode* Node, const aiScene* Scene, std::unordered_map<le::UInt32_t, std::vector<aiMesh*> >& Meshes )
{
	for ( le::UInt32_t index = 0; index < Node->mNumMeshes; ++index )
		Meshes[ Scene->mMeshes[ Node->mMeshes[ index ] ]->mMaterialIndex ].push_back( Scene->mMeshes[ Node->mMeshes[ index ] ] );

	for ( le::UInt32_t index = 0; index < Node->mNumChildren; ++index )
		ProcessNode( Node->mChildren[ index ], Scene, Meshes );
}

// ------------------------------------------------------------------------------------ //
// Update lump in file
// ------------------------------------------------------------------------------------ //
void Collision::UpdateLumps( std::ofstream& File, Collision::PHYLump* Lumps, le::UInt32_t OffsetToLumps )
{
	File.seekp( OffsetToLumps, std::ios::beg );
	File.write( ( char* ) &Lumps[ 0 ], sizeof( PHYLump ) * PL_MAX_LUMPS );
}

// ------------------------------------------------------------------------------------ //
// Write verteces in file
// ------------------------------------------------------------------------------------ //
void Collision::WriteVerteces( std::ofstream& File, Collision::PHYLump& Lump )
{
	le::UInt32_t		countVerteces = verteces.size();
	Lump.offset = File.tellp();
	Lump.length = countVerteces * sizeof( PHYVertex );

	File.write( ( char* ) &countVerteces, sizeof( le::UInt32_t ) );
	File.write( ( char* ) &verteces[ 0 ], countVerteces * sizeof( PHYVertex ) );
}

// ------------------------------------------------------------------------------------ //
// Write indeces in file
// ------------------------------------------------------------------------------------ //
void Collision::WriteIndeces( std::ofstream& File, Collision::PHYLump& Lump )
{
	le::UInt32_t		countIndeces = indices.size();
	Lump.offset = File.tellp();
	Lump.length = countIndeces * sizeof( le::UInt32_t );

	File.write( ( char* ) &countIndeces, sizeof( le::UInt32_t ) );
	File.write( ( char* ) &indices[ 0 ], countIndeces * sizeof( le::UInt32_t ) );
}

//------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Collision::PHYVertex::PHYVertex()
{
	position[ 0 ] = 0.f;
	position[ 1 ] = 0.f;
	position[ 2 ] = 0.f;
}

//------------------------------------------------------------------------------------ //
// Operator ==
// ------------------------------------------------------------------------------------ //
bool Collision::PHYVertex::operator==( const Collision::PHYVertex& Right )
{
	return position[ 0 ] == Right.position[ 0 ] &&
			position[ 1 ] == Right.position[ 1 ] &&
			position[ 2 ] == Right.position[ 2 ];
}
