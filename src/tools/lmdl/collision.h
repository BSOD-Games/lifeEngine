//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef COLLISION_H
#define COLLISION_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "common/types.h"
#include "phytypes.h"

//---------------------------------------------------------------------//

struct aiNode;
struct aiMesh;
struct aiScene;

//---------------------------------------------------------------------//

class Collision
{
public:
	// Collision
	Collision();
	~Collision();

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
	std::vector< PHYVector3D >		verteces;
	std::vector< le::UInt32_t >		indices;
};

//---------------------------------------------------------------------//

#endif // COLLISION_H
