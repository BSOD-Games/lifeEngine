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
#include <LinearMath/btConvexHull.h>

#include "common/types.h"
#include "global.h"
#include "collision.h"
#include "phydoc.h"

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

	std::unordered_map<le::UInt32_t, std::vector<AIMesh>>			meshes;
	ProcessNode( scene->mRootNode, scene, meshes );
	if ( meshes.empty() )	throw std::runtime_error( "In file not found meshes" );

	// Getting array verteces and indeces from collision mesh
	PHYVector3D						vertex;
	std::vector< PHYVector3D >		vertexBuffer;

	for ( auto itRoot = meshes.begin(), itRootEnd = meshes.end(); itRoot != itRootEnd; ++itRoot )
		for ( auto itMesh = itRoot->second.begin(), itMeshEnd = itRoot->second.end(); itMesh != itMeshEnd; ++itMesh )
		{
			aiMesh*			mesh = ( *itMesh ).mesh;

			// Prepare the vertex buffer.
			// If the vertices of the mesh do not fit into the buffer, then
			// expand it
			if ( vertexBuffer.size() < mesh->mNumVertices )
				vertexBuffer.resize( vertexBuffer.size() + mesh->mNumVertices );

			// Read all verteces
			for ( le::UInt32_t index = 0; index < mesh->mNumVertices; ++index )
			{
				aiVector3D		tempVector = ( *itMesh ).transformation * mesh->mVertices[ index ];
				vertex.x = tempVector.x;
				vertex.y = tempVector.y;
				vertex.z = tempVector.z;

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

	// Is nedd generate hull shape
	if ( g_isGenHullShape )
	{
		std::cout << "Generate hull shape\n";

		// Convert PHYVector3D to btVector3
		std::vector< btVector3 >		btVectors;
		for ( le::UInt32_t index = 0, count = indices.size(); index < count; ++index )
			btVectors.push_back( btVector3( verteces[ indices[ index ] ].x, verteces[ indices[ index ] ].y, verteces[ indices[ index ] ].z ) );

		HullLibrary		hullLibrary;
		HullResult		hullResult;
		HullDesc		hullDesc( QF_TRIANGLES, btVectors.size(), btVectors.data() );
		
		if ( hullLibrary.CreateConvexHull( hullDesc, hullResult ) != QE_OK )
		{
			std::cout << "Failed generate hull shape\n";
			return;
		}
		
		// Change array verteces and indeces
		verteces.clear();
		indices.clear();

		for ( le::UInt32_t index = 0; index < hullResult.mNumOutputVertices; ++index )
		{
			const btVector3&			vector = hullResult.m_OutputVertices[ index ];
			verteces.push_back( PHYVector3D( vector.getX(), vector.getY(), vector.getZ() ) );
		}

		for ( le::UInt32_t index = 0; index < hullResult.mNumIndices; ++index )
			indices.push_back( hullResult.m_Indices[ index ] );

		hullLibrary.ReleaseResult( hullResult );
		std::cout << "Generated hull shape\n";
	}
}

// ------------------------------------------------------------------------------------ //
// Save collision
// ------------------------------------------------------------------------------------ //
void Collision::Save( const std::string& Path )
{
	if ( !isLoaded )					throw std::runtime_error( "Collision mesh not loaded" );

	PHYDoc			phyDoc;
	phyDoc.SetStatic( g_isStaticCollision );
	phyDoc.SetMasa( g_masa );
	phyDoc.SetInertia( PHYVector3D( g_inertia.x, g_inertia.y, g_inertia.z ) );
	phyDoc.SetVerteces( verteces );
	phyDoc.SetVertexIndeces( indices );

	if ( !phyDoc.Save( Path + ".phy" ) )	throw std::runtime_error( "Failed save collision mesh" );
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
void Collision::ProcessNode( aiNode* Node, const aiScene* Scene, std::unordered_map<le::UInt32_t, std::vector<AIMesh>>& Meshes )
{
	for ( le::UInt32_t index = 0; index < Node->mNumMeshes; ++index )
	{
		aiMesh*			mesh = Scene->mMeshes[ Node->mMeshes[ index ] ];
		Meshes[ mesh->mMaterialIndex ].push_back( AIMesh( Node->mTransformation, mesh ) );
	}

	for ( le::UInt32_t index = 0; index < Node->mNumChildren; ++index )
		ProcessNode( Node->mChildren[ index ], Scene, Meshes );
}

// ----------------------------------------------------------------------------------- //
// operator == for PHYVector3D
// ------------------------------------------------------------------------------------ //
bool operator==( const PHYVector3D& Left, const PHYVector3D& Right )
{
	return Left.x == Right.x &&
		Left.y == Right.y &&
		Left.z == Right.z;
}