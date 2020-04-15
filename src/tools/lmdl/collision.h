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

	enum PHY_LUMPS
	{
		PL_VERTECES,
		PL_INDECES,
		PL_MAX_LUMPS
	};

	struct PHYHeader
	{
		char				strId[ 4 ]; // Always 'LPHY'
		le::UInt32_t		version;
	};

	struct PHYLump
	{
		le::UInt32_t		offset;
		le::UInt32_t		length;
	};

	struct PHYVertex
	{
		PHYVertex();

		float		position[ 3 ];

		bool operator==( const PHYVertex& Right );
	};

	//---------------------------------------------------------------------//

	void			ProcessNode( aiNode* Node, const aiScene* Scene, std::unordered_map<le::UInt32_t, std::vector< aiMesh* > >& Meshes );
	void			UpdateLumps( std::ofstream& File, PHYLump* Lumps, le::UInt32_t OffsetToLumps );
	void			WriteVerteces( std::ofstream& File, PHYLump& Lump );
	void			WriteIndeces( std::ofstream& File, PHYLump& Lump );

	bool							isLoaded;
	std::vector< PHYVertex >		verteces;
	std::vector< le::UInt32_t >		indices;
};

//---------------------------------------------------------------------//

#endif // COLLISION_H
