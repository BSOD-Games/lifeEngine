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
#include <fstream>

#include "common/types.h"
#include "mdltypes.h"

//---------------------------------------------------------------------//

struct aiNode;
struct aiMesh;
struct aiScene;

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

	//---------------------------------------------------------------------//

	struct AIMesh
	{
		AIMesh() 
		{}

		AIMesh( const aiMatrix4x4& Transformation, aiMesh* Mesh ) :
			transformation( Transformation ),
			mesh( Mesh )
		{}

		aiMatrix4x4			transformation;
		aiMesh*				mesh;
	};

	//---------------------------------------------------------------------//

	void			ProcessNode( aiNode* Node, const aiScene* Scene, std::unordered_map<le::UInt32_t, std::vector<AIMesh>>& Meshes );

	bool							isLoaded;
	std::vector< MDLVertex >		verteces;
	std::vector< std::uint32_t >	vertexIndices;
	std::vector< MDLSurface >		surfaces;
	std::vector< std::string >		materials;
};

//---------------------------------------------------------------------//

#endif // !MESH_H
