<<<<<<< HEAD
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
#include "mdldoc.h"

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
=======
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
#include "mdldoc.h"

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

	std::unordered_map<le::UInt32_t, std::vector<AIMesh>>			meshes;
	ProcessNode( scene->mRootNode, scene, meshes );
	if ( meshes.empty() )	throw std::runtime_error( "In file not found meshes" );

	// Go through the material ID, take the mesh and write its vertices, and indices
	// to the shared buffer
	MDLSurface					surface;
	MDLVertex					vertex;
	std::vector<MDLVertex>		vertexBuffer;
	for ( auto itRoot = meshes.begin(), itRootEnd = meshes.end(); itRoot != itRootEnd; ++itRoot )
	{
		surface.startVertexIndex = vertexIndices.size();
		surface.materialId = materials.size();

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
				vertex.position.x = tempVector.x;
				vertex.position.y = tempVector.y;
				vertex.position.z = tempVector.z;

				tempVector =  ( aiMatrix3x3 ) ( *itMesh ).transformation * mesh->mNormals[ index ];
				vertex.normal.x = tempVector.x;
				vertex.normal.y = tempVector.y;
				vertex.normal.z = tempVector.z;

				if ( mesh->mTangents )
				{
					tempVector = ( aiMatrix3x3 ) ( *itMesh ).transformation * mesh->mTangents[ index ];
					vertex.tangent.x = tempVector.x;
					vertex.tangent.y = tempVector.y;
					vertex.tangent.z = tempVector.z;
				}

				if ( mesh->mBitangents )
				{
					tempVector = ( aiMatrix3x3 ) ( *itMesh ).transformation * mesh->mBitangents[ index ];
					vertex.bitangent.x = tempVector.x;
					vertex.bitangent.y = tempVector.y;
					vertex.bitangent.z = tempVector.z;
				}

				if ( mesh->mTextureCoords[ 0 ] )
				{
					tempVector = mesh->mTextureCoords[ 0 ][ index ];
					vertex.texCoords.x = tempVector.x;
					vertex.texCoords.y = tempVector.y;
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
						vertexIndices.push_back( verteces.size() );
						verteces.push_back( vertexBuffer[ index ] );
					}
					else
						vertexIndices.push_back( it - verteces.begin() );
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

			surface.countVertexIndeces = vertexIndices.size() - surface.startVertexIndex;
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
	if ( !isLoaded )	throw std::runtime_error( "Mesh not loaded" );
	
	MDLDoc				mdlDoc;
	mdlDoc.SetVerteces( verteces );
	mdlDoc.SetVertexIndeces( vertexIndices );
	mdlDoc.SetSurfaces( surfaces );
	mdlDoc.SetMaterials( materials );

	if ( !mdlDoc.Save( Path + ".mdl" ) )	throw std::runtime_error( "Failed save model" );
	std::cout << "Model saved\n";
}

// ------------------------------------------------------------------------------------ //
// Clear mesh
// ------------------------------------------------------------------------------------ //
void Mesh::Clear()
{
	verteces.clear();
	vertexIndices.clear();
	materials.clear();
	surfaces.clear();
	isLoaded = false;
}

// ----------------------------------------------------------------------------------- //
// Process node scene in assimp
// ------------------------------------------------------------------------------------ //
void Mesh::ProcessNode( aiNode* Node, const aiScene* Scene, std::unordered_map<le::UInt32_t, std::vector<AIMesh>>& Meshes )
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
// operator == for MDLVertex
// ------------------------------------------------------------------------------------ //
bool operator==( const MDLVertex& Left, const MDLVertex& Right )
>>>>>>> remotes/origin/dev
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
		surface.startVertexIndex = vertexIndices.size();
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
				vertex.position.x = tempVector.x;
				vertex.position.y = tempVector.y;
				vertex.position.z = tempVector.z;

				tempVector = mesh->mNormals[ index ];
				vertex.normal.x = tempVector.x;
				vertex.normal.y = tempVector.y;
				vertex.normal.z = tempVector.z;

				if ( mesh->mTangents )
				{
					tempVector = mesh->mTangents[ index ];
					vertex.tangent.x = tempVector.x;
					vertex.tangent.y = tempVector.y;
					vertex.tangent.z = tempVector.z;
				}

				if ( mesh->mBitangents )
				{
					tempVector = mesh->mBitangents[ index ];
					vertex.bitangent.x = tempVector.x;
					vertex.bitangent.y = tempVector.y;
					vertex.bitangent.z = tempVector.z;
				}

				if ( mesh->mTextureCoords[ 0 ] )
				{
					tempVector = mesh->mTextureCoords[ 0 ][ index ];
					vertex.texCoords.x = tempVector.x;
					vertex.texCoords.y = tempVector.y;
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
						vertexIndices.push_back( verteces.size() );
						verteces.push_back( vertexBuffer[ index ] );
					}
					else
						vertexIndices.push_back( it - verteces.begin() );
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

			surface.countVertexIndeces = vertexIndices.size() - surface.startVertexIndex;
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
	if ( !isLoaded )	throw std::runtime_error( "Mesh not loaded" );
	
	MDLDoc				mdlDoc;
	mdlDoc.SetVerteces( verteces );
	mdlDoc.SetVertexIndeces( vertexIndices );
	mdlDoc.SetSurfaces( surfaces );
	mdlDoc.SetMaterials( materials );

	if ( !mdlDoc.Save( Path + ".mdl" ) )	throw std::runtime_error( "Failed save model" );
	std::cout << "Model saved\n";
}

// ------------------------------------------------------------------------------------ //
// Clear mesh
// ------------------------------------------------------------------------------------ //
void Mesh::Clear()
{
	verteces.clear();
	vertexIndices.clear();
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

// ----------------------------------------------------------------------------------- //
// operator == for MDLVertex
// ------------------------------------------------------------------------------------ //
bool operator==( const MDLVertex& Left, const MDLVertex& Right )
{
	return Left.position.x == Right.position.x &&
		Left.position.y == Right.position.y &&
		Left.position.z == Right.position.z &&

		Left.normal.x == Right.normal.x &&
		Left.normal.y == Right.normal.y &&
		Left.normal.z == Right.normal.z &&

		Left.texCoords.x == Right.texCoords.x &&
		Left.texCoords.y == Right.texCoords.y &&

		Left.bitangent.x == Right.bitangent.x &&
		Left.bitangent.y == Right.bitangent.y &&
		Left.bitangent.z == Right.bitangent.z &&

		Left.tangent.x == Right.tangent.x &&
		Left.tangent.y == Right.tangent.y &&
		Left.tangent.z == Right.tangent.z;
}