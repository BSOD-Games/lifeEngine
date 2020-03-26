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

#define LMD_ID			"LMD"
#define LMD_VERSION		2

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
	Surface					surface;
	Vertex					vertex;
	std::vector<Vertex>		vertexBuffer;
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

	isLoaded = true;
	std::cout << "Model loaded\n";
}

// ------------------------------------------------------------------------------------ //
// Save mesh
// ------------------------------------------------------------------------------------ //
void Mesh::Save( const std::string& Path )
{
	std::ofstream				file( Path + ".lmd", std::ios::binary );
	if ( !file.is_open() )		throw std::runtime_error( "Failed create file" );

	// Write header fille
	const le::UInt16_t			version = LMD_VERSION;
	file.write( LMD_ID, 3 );
	file.write( ( char* ) &version, sizeof( le::UInt16_t ) );

	// Write materials in mesh
	le::UInt32_t			countMaterials = materials.size();
	file.write( ( char* ) &countMaterials, sizeof( le::UInt32_t ) );
	for ( uint32_t index = 0; index < countMaterials; ++index )
	{
		std::string*		nameMaterial = &materials[ index ];
		le::UInt32_t		sizeString = 0;

		sizeString = nameMaterial->size();
		file.write( ( char* ) &sizeString, sizeof( le::UInt32_t ) );
		file.write( nameMaterial->data(), sizeString );
	}

	// Write verteces
	le::UInt32_t					countVerteces = verteces.size();
	file.write( ( char* ) &countVerteces, sizeof( le::UInt32_t ) );
	file.write( ( char* ) &verteces[ 0 ], countVerteces * sizeof( Vertex ) );

	// Write indeces
	le::UInt32_t					countIndeces = indices.size();
	file.write( ( char* ) &countIndeces, sizeof( le::UInt32_t ) );
	file.write( ( char* ) &indices[ 0 ], countIndeces * sizeof( le::UInt32_t ) );

	// Write surfaces
	Surface*				surface;
	le::UInt32_t			countSurfaces = surfaces.size();
	file.write( ( char* ) &countSurfaces, sizeof( le::UInt32_t ) );
	for ( le::UInt32_t index = 0; index < countSurfaces; ++index )
	{
		surface = &surfaces[ index ];

		file.write( ( char* ) &surface->materialId, sizeof( le::UInt32_t ) );
		file.write( ( char* ) &surface->startVertexIndex, sizeof( le::UInt32_t ) );
		file.write( ( char* ) &surface->countVertexIndex, sizeof( le::UInt32_t ) );
	}

	std::cout << "Model saved\n";
}

// ------------------------------------------------------------------------------------ //
// Clear mesh
// ------------------------------------------------------------------------------------ //
void Mesh::Clear()
{
	verteces.clear();
	indices.clear();
	surfaces.clear();
	materials.clear();
	isLoaded = false;
}

// ------------------------------------------------------------------------------------ //
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
// Constructor
// ------------------------------------------------------------------------------------ //
Vertex::Vertex()
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
bool Vertex::operator==( const Vertex& Right )
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
Surface::Surface() :
	materialId( 0 ),
	startVertexIndex( 0 ),
	countVertexIndex( 0 )
{}
