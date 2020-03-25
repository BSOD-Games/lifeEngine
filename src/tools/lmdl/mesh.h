//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <unordered_map>

#include "common/types.h"

//---------------------------------------------------------------------//

struct aiNode;
struct aiMesh;
struct aiScene;

//---------------------------------------------------------------------//

struct Vertex
{
	Vertex();

	float		position[ 3 ];
	float		normal[ 3 ];
	float		texCoords[ 2 ];
	float		tangent[ 3 ];
	float		bitangent[ 3 ];

	bool operator==( const Vertex& Right );
};

//---------------------------------------------------------------------//

struct Surface
{
	Surface();

	le::UInt32_t		materialId;
	le::UInt32_t        startVertexIndex;
	le::UInt32_t        countVertexIndex;
};

//---------------------------------------------------------------------//

class Mesh
{
public:
	// Mesh
	Mesh();
	~Mesh();

	void			Load( const std::string& Path );
	void			Save( const std::string& Path );
	void			Clear();

private:
	void			ProcessNode( aiNode* Node, const aiScene* Scene, std::unordered_map<le::UInt32_t, std::vector< aiMesh* > >& Meshes );

	bool							isLoaded;
	std::vector<Vertex>				verteces;
	std::vector<le::UInt32_t>		indices;
	std::vector<Surface>			surfaces;
	std::vector<std::string>		materials;
};

//---------------------------------------------------------------------//

#endif // !MESH_H
